CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXEC = emul-mips
LDFLAGS =

all: $(EXEC)

$(EXEC): $(EXEC).o hex.o
	$(CC) -o $@ $^ $(LDFLAGS)

debug: $(EXEC)Debug.o hexDebug.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

$(EXEC).o: $(EXEC).c $(EXEC).h
	$(CC) -c $< $(CFLAGS) -o $@

$(EXEC)Debug.o: $(EXEC).c $(EXEC).h
	$(CC) -c -D DEBUG $< $(CFLAGS) -o $@

hex.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c $< $(CFLAGS) -o $@

hexDebug.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c -D DEBUG $< $(CFLAGS) -o $@

clean:
	rm -rf *.o
