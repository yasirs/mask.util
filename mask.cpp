#include "fasta_record.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <exception>
#include <string>
#include <sstream>


using namespace std;

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

int main(int argc, char* argv[]) {
    fasta::Record r1, r2;
    ifstream if1(argv[1], ifstream::in);
    ifstream if2(argv[2], ifstream::in);
    long int recn = 0;
    while (((if1.peek()!=EOF))or((if2.peek()!=EOF))) {
        /*if (ir1!=e) {
            cout << argv[1] << " ended first\n"; break;
        }
        if (ir2==e) {
            cout << argv[2] << " ended first\n"; break;
        }*/
        ++recn;
        if1 >> r1; if2 >> r2;
        if (r1.name() != r2.name()) {
            throw nameExc(r1.name(), r2.name(), recn);
        }
        string s1 = r1.sequence(); string s2 = r2.sequence();
        if (s1.length()!=s2.length()) {
            throw lengthExc(s1.length(), s2.length(), recn, r1.name());
        }
        cout << ">"<<r1.name()<<endl;
        for (int i=0;i<s1.length();i++) {
            if (s1[i]!=s2[i]) {
                if (ismasked(s1[i]) or ismasked(s2[i])) {
                    cout << 'X';
                } else {
                    throw seqExc(s1[i],s2[i],recn,i,r1.name());
                }
            } else {
                cout << s1[i];
            }
        }
        cout << endl;
    }        
}