#include "fasta_record.h"
#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;


inline bool ismasked(const char c) {
    return ((c=='n')or(c=='N')or(c=='X')or(c=='x'));
}

int main(int argc, char* argv[]) {
    fasta::Record r1, r2;
    ifstream if1(argv[1], ifstream::in);
    ifstream if2(argv[2], ifstream::in);
    //istream_iterator<fasta::Record> ir1(if1), ir2(if2), e;
    while (((if1.peek()!=EOF))or((if2.peek()!=EOF))) {
        /*if (ir1!=e) {
            cout << argv[1] << " ended first\n"; break;
        }
        if (ir2==e) {
            cout << argv[2] << " ended first\n"; break;
        }*/
 
        if1 >> r1; if2 >> r2;
        cout << "Read "<<r1.name()<<" and "<<r2.name()<<endl;
        if (r1.name() != r2.name()) {
            cout << "Names "<<r1.name() << " and " << r2.name() <<" are different.\n";
        }
        string s1 = r1.sequence(); string s2 = r2.sequence();
        if (s1.length()!=s2.length()) {
            cout << "Lengths "<<s1.length()<<" and "<<s2.length()<<" are different for "<<r1.name()<<endl;
        }
        for (int i=0;i<s1.length();i++) {
            if (s1[i]!=s2[i]) {
                if (ismasked(s1[i]) or ismasked(s2[i])) {
                    cout << 'X';
                } else {
                    cout << "Different sequences\n";
                }
            } else {
                cout << s1[i];
            }
        }
    }        
}
