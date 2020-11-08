CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXEC = emul-mips
LDFLAGS =

all: $(EXEC)

#Cibles des tests
test: test.o tools.o hexDebug.o registres.o
	$(CC) -o $@ $^ $(LDFLAGS)

test.o: test.c
	$(CC) -c $< $(CFLAGS) -o $@

#Cibles principale
$(EXEC): $(EXEC).o hex.o tools.o registres.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC).o: $(EXEC).c
	$(CC) -c $< $(CFLAGS) -o $@

hex.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c $< $(CFLAGS) -o $@

registres.o: module_registres/registres.c module_registres/registres.h
	$(CC) -c $< $(CFLAGS) -o $@

tools.o: module_tools/tools.c module_tools/tools.h
	$(CC) -c $< $(CFLAGS) -o $@

#Version debug de hex
hexDebug.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c -D DEBUG -D VERBEUX $< $(CFLAGS) -o $@

clean:
	rm -rf *.o
