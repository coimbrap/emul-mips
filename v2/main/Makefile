CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXEC = test
LDFLAGS =

all: $(EXEC)

$(EXEC): $(EXEC).o registres.o
	$(CC) -o $@ $^ $(LDFLAGS)

registres.o: module_registres/registres.c module_registres/registres.h
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rf *.o
