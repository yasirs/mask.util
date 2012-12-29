all:
	g++ mask.cpp fasta_record.cpp -o mask -I.
debug:
	g++ -g mask.cpp fasta_record.cpp -o mask -I.
