CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXEC = emul-mips
LDFLAGS =

all: $(EXEC)

$(EXEC): $(EXEC).o hex.o tools.o
	$(CC) -o $@ $^ $(LDFLAGS)

debug: $(EXEC)Debug.o hexDebug.o tools.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

test: $(EXEC)Test.o hexVerbeux.o tools.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

$(EXEC).o: $(EXEC).c
	$(CC) -c $< $(CFLAGS) -o $@

$(EXEC)Debug.o: $(EXEC).c
	$(CC) -c -D DEBUG $< $(CFLAGS) -o $@

$(EXEC)Test.o: $(EXEC).c
	$(CC) -c -D TEST $< $(CFLAGS) -o $@

hex.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c $< $(CFLAGS) -o $@

hexDebug.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c -D DEBUG -D VERBEUX $< $(CFLAGS) -o $@

hexVerbeux.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c -D VERBEUX $< $(CFLAGS) -o $@

tools.o: module_tools/tools.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rf *.o
