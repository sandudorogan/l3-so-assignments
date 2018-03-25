CC=gcc
CFLAGS=-Wall

build: tema1 

all: tema1.exec

tema1.exec: tema1.o list.o utils.o hashtable.o
	$(CC) $(CFLAGS) $^ -o $@ 

tema1: tema1.o list.o utils.o hashtable.o
	$(CC) $(CFLAGS) $^ -o $@ -lhash -L.

.PHONY: clean

clean:
	rm -rf *.o tema1 tema1.exec
