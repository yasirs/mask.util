mask.util
=========

Combines two fasta files with identical sequences but different parts masked
to produce a fasta file with the combined masking. 

Example:
--------
```
$ make
g++ mask.cpp fasta_record.cpp -o mask -I.

$ ./mask example1.fa example2.fa 
>seq1
XXXXTAGAGAGAGAGAGTAGATACAGTAGAGAGAGAGAGTXXXXACAGTAG
AGAGAGAGAGTAGAXXCAGTAGAGAGAGAGAGTAGATACAGTAGAGAGAGA
GAGTAGATACAGTAGAGAGAGAGAGTAGTTAACACACA
>seq2
CAGTAGAGAGAGAGAGTAGATACAGTAGAGAGAGAGAGTAGTTCAGTAGAG
AGAGAGAGXXXXTACAGTAGAGAGAGAGAGTAGTTCAGTAGAGAGAXXXXX
XXXATACAGTAGAGAGAGAGAGTAGTTCAGTAGAGAGAGAGAGTAGATACA
GTAGAGAGAGAGAGTAGTT
>seq32
CAGTAGAGAGAGAGAGTAGATACAGTAGAGAGAGAGAGTAGTTCAGTAGAG
AGXXXXXXXXGATACAGTAGAGAGAGAGAGTAGTTCAGTAGAGAGAGAGAG
TAGATACAGTAGAGAGAGAGAGTACAGTAGAGAGAGAGAGTAGATACAGTA
GAGAGAGAGAGTAGTT
```
Here, any nucleotide masked in either file is masked with an X.


Acknowlegement:
---------------
Some code borrowed from https://github.com/tdoug870/fasta-parser

[![githalytics.com alpha](https://cruel-carlota.pagodabox.com/c9551e7d097e99ee2a9bff56fc9de800 "githalytics.com")](http://githalytics.com/yasirs/mask.util)
