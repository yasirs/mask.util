#include "fasta_record.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <exception>
#include <string>
#include <sstream>
#include <stdexcept>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

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

fasta::Record* combineRecords(fasta::Record r1, fasta::Record r2, int recn) {
    auto pr = new fasta::Record;
    std::string name, sequence;
    std::stringstream seqstream;


    if (r1.name() != r2.name()) {
        throw nameExc(r1.name(), r2.name(), recn);
    }
    pr->setName(r1.name());

    string s1 = r1.sequence(); string s2 = r2.sequence();
    if (s1.length()!=s2.length()) {
        throw lengthExc(s1.length(), s2.length(), recn, r1.name());
    }
    //cout << ">"<<r1.name()<<endl;
    for (int i=0;i<s1.length();i++) {
        if (s1[i]!=s2[i]) {
            if (ismasked(s1[i]) or ismasked(s2[i])) {
               seqstream << 'X';
            } else {
                throw seqExc(s1[i],s2[i],recn,i,r1.name());
            }
        } else {
            seqstream << s1[i];
        }
        //if ((++cno) > linesize) {
        //    cout << '\n'; cno = 0; ++lno;
        //}
    }
    seqstream >> sequence;
    pr->setSequence(sequence);
    
    return pr;


}

int main(int argc, char* argv[]) {

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("linewidth", po::value<int>(), "set max line width for output FASTA")
        ("maskchar", po::value<char>(), "masking character")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    int width = 50;
    if (vm.count("linewdith")) {
        width = vm["linewidth"].as<int>();
        cout << "width set to "<<width<<endl;
    }
    

    fasta::Record r1, r2;
    fasta::Record* pr3;
    int linesize;
    if (argc<3) throw(invalid_argument("Usage: mask input1.fasta input2.fasta [n] > combined.fasta\n where n denotes line wrap length for output fasta sequences."));
    ifstream if1(argv[1], ifstream::in);
    ifstream if2(argv[2], ifstream::in);
    if (argc==3) linesize=50;
    else linesize = a_to_i(argv[3]);
    long int recn = 0;
    int lno; int cno;
    while (((if1.peek()!=EOF))or((if2.peek()!=EOF))) {
        /*if (ir1!=e) {
            cout << argv[1] << " ended first\n"; break;
        }
        if (ir2==e) {
            cout << argv[2] << " ended first\n"; break;
        }*/
        ++recn;
        if1 >> r1; if2 >> r2;
        pr3 = combineRecords(r1, r2, recn);
        //cout << (*pr3);
        pr3->printFasta(cout, 40);
        delete pr3;
    }        
}
