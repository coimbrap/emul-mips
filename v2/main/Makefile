CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
EXEC = test
LDFLAGS = -g

all: $(EXEC)

$(EXEC): $(EXEC).o registres.o memoire.o
	$(CC) -o $@ $^ $(LDFLAGS)

registres.o: module_registres/registres.c module_registres/registres.h
	$(CC) -c $< $(CFLAGS) -o $@

memoire.o: module_memoire/memoire.c module_memoire/memoire.h
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rf *.o
