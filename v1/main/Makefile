CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXEC = emul-mips
LDFLAGS =

all: $(EXEC)

$(EXEC): $(EXEC).o hex.o tools.o
	$(CC) -o $@ $^ $(LDFLAGS)

test: $(EXEC).o test.o tools.o hexDebug.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC).o: $(EXEC).c
	$(CC) -c $< $(CFLAGS) -o $@

hex.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c $< $(CFLAGS) -o $@

tools.o: module_tools/tools.c
	$(CC) -c $< $(CFLAGS) -o $@

test.o: test.c
	$(CC) -c $< $(CFLAGS) -o $@

hexDebug.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c -D DEBUG -D VERBEUX $< $(CFLAGS) -o $@

clean:
	rm -rf *.o
