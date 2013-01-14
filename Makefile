all:
	g++ -std=c++0x mask.cpp fasta_record.cpp -o mask -I.
debug:
	g++ -std=c++0x -g mask.cpp fasta_record.cpp -o mask -I.
