CC = gcc
CFLAGS = -Wall -pedantic
EXEC = emul-mips
LDFLAGS =

all: $(EXEC)

#Cible principale
$(EXEC): $(EXEC).o hex.o registres.o tools.o
	$(CC) -o $@ $^ $(LDFLAGS)

#Fichiers objets

$(EXEC).o: $(EXEC).c
	$(CC) -c $< $(CFLAGS) -o $@

hex.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c $< $(CFLAGS) -o $@

registres.o: module_registres/registres.c module_registres/registres.h
	$(CC) -c $< $(CFLAGS) -o $@

tools.o: module_tools/tools.c module_tools/tools.h
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rf *.o
