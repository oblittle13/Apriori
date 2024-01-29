CC = g++
CFLAGS = -std=c++11

all: GenDatabase apriori idea1 help

GenDatabase: Generator.o
	$(CC) $(CFLAGS) -o GenDatabase Generator.o

apriori: Apriori.o
	$(CC) $(CFLAGS) -o apriori Apriori.o

Generator.o: Generator.cpp Generator.h
	$(CC) $(CFLAGS) -c Generator.cpp

Apriori.o: Apriori.cpp Apriori.h
	$(CC) $(CFLAGS) -c Apriori.cpp

idea1:
	chmod +x idea1.py

clean:
	rm -f *.o GenDatabase apriori

help:
	@echo "To generate databases: ./GenDatabase"
	@echo "To run apriori standard: ./apriori {Database w/o extension} {min_supp}"
	@echo "To run idea1: ./idea1.py {Database w/o extension} {minn_supp}"
