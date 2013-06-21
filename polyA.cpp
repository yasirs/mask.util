#include "fasta_record.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <exception>
#include <string>
#include <sstream>
#include <stdexcept>



using namespace std;

int a_to_i(const char *s) {
    stringstream ss;
    ss << s;
    int i;
    ss >> i;
    return(i);
}


class nameExc : public exception {
    public:
        string name1, name2;
        long int recnum;
        nameExc(string n1, string n2, long int rn): name1(n1), name2(n2), recnum(rn) {}
        virtual const char* what() const throw()
        {
            ostringstream out;
            out << "Record names "<<name1<<" and "<<name2<<" are different at record number "<< recnum;
            return out.str().c_str();
        }
        ~nameExc() throw() {}
};


class lengthExc : public exception {
    public:
        long int length1, length2;
        long int recnum;
        string name;
        lengthExc(long int n1, long int n2, long int rn, string name): length1(n1), length2(n2), recnum(rn), name(name) {}
        virtual const char* what() const throw()
        {
            ostringstream out;
            out << "Record lengths "<<length1<<" and "<<length2<<" are different at record "<<name<<" at number "<< recnum;
            return out.str().c_str();
        }
        ~lengthExc() throw() {}
};

class seqExc : public exception {
    public:
        char c1, c2;
        long int recnum;
        long int snum;
        string name;
        seqExc(char n1, char n2, long int rn, long int sn, string nam): c1(n1), c2(n2), recnum(rn), snum(sn), name(nam) {}
        virtual const char* what() const throw()
        {
            ostringstream out;
            out << "Record "<<name<<" number "<<recnum<<" is divergent at index "<<snum<<": "<<c1<<" vs. "<<c2;
            return out.str().c_str();
        }
        ~seqExc() throw() {}
};

inline bool ismasked(const char c) {
    return ((c=='n')or(c=='N')or(c=='X')or(c=='x'));
}

int filterTail(fasta::Record& r, int limit) {
	int nChanges = 0;
	std::string name, sequence;
	string s = r.sequence();
	int sLength = s.length();
	int nlen=0;
	if ((s[0]=='a')or(s[0]=='A')) {
		nlen=1;
		for(int i=1; i<s.length();i++) {
			if ((s[i]=='A')or(s[i]=='a')) {
				nlen++;
			} else {
				break;
			}
		}
	} else if ((s[0]=='t')or(s[0]=='T')) {
		nlen=1;
		for(int i=1; i<s.length();i++) {
			if ((s[i]=='T')or(s[i]=='t')) {
				nlen++;
			} else {
				break;
			}
		}
	}
	if (nlen>=limit) {
		for (int i=0;i<nlen;i++) {
			s[i]='X';
		}
		nChanges += nlen;
	} //else the length of the sequence is too short
	// do the same for the 5' end
	nlen=0;
	if ((s[sLength-1]=='a')or(s[sLength-1]=='A')) {
		nlen=1;
		for(int i=sLength-2; i>=0;i--) {
			if ((s[i]=='A')or(s[i]=='a')) {
				nlen++;
			} else {
				break;
			}
		}
	} else if ((s[sLength-1]=='t')or(s[sLength-1]=='T')) {
		nlen=1;
		for(int i=sLength-2; i>=0;i--) {
			if ((s[i]=='T')or(s[i]=='t')) {
				nlen++;
			} else {
				break;
			}
		}
	}
	if (nlen>=limit) {
		for (int i=sLength-1;i>sLength-1-nlen;i--) {
			s[i]='X';
		}
		nChanges += nlen;
	} //else the length of the sequence is too short
	r.setSequence(s);
	return nChanges;
}


int main(int argc, char* argv[]) {
    fasta::Record r;
    int linesize;
    if (argc<3) throw(invalid_argument("Usage: mask input.fasta m [n] > combined.fasta\n where m is the minimum length to consider a polyA tail\n and n denotes line wrap length for output fasta sequences."));
    ifstream if1(argv[1], ifstream::in);
    int m = a_to_i(argv[2]);
    if (argc==3) linesize=50;
    else linesize = a_to_i(argv[3]);
    long int recn = 0;
    int lno; int cno;
    while (if1.peek()!=EOF) {
        ++recn;
        if1 >> r;
	filterTail(r, m);

        r.printFasta(cout, linesize);
    }        
}
