all:
	g++ -std=c++0x mask.cpp fasta_record.cpp -o mask -I.
	g++ -std=c++0x polyA.cpp fasta_record.cpp -o polyA -I.
debug:
	g++ -std=c++0x -g mask.cpp fasta_record.cpp -o mask -I.
	g++ -std=c++0x -g -O0 polyA.cpp fasta_record.cpp -o polyA -I.
