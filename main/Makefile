CC = gcc
CFLAGS = -Wall -pedantic
EXEC = emul-mips
LDFLAGS =

all: $(EXEC)

#Cible des tests
test: test.o tools.o hexDebug.o registres.o memoire.o calcul.o
	$(CC) -o $@ $^ $(LDFLAGS)

#Cible principale
$(EXEC): $(EXEC).o hex.o tools.o registres.o memoire.o calcul.o
	$(CC) -o $@ $^ $(LDFLAGS)

#Fichiers objets

$(EXEC).o: $(EXEC).c
	$(CC) -c $< $(CFLAGS) -o $@

hex.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c $< $(CFLAGS) -o $@

registres.o: module_registres/registres.c module_registres/registres.h
	$(CC) -c $< $(CFLAGS) -o $@

memoire.o: module_memoire/memoire.c module_memoire/memoire.h
	$(CC) -c $< $(CFLAGS) -o $@

calcul.o: module_calcul/calcul.c module_calcul/calcul.h
	$(CC) -c $< $(CFLAGS) -o $@

tools.o: module_tools/tools.c module_tools/tools.h
	$(CC) -c $< $(CFLAGS) -o $@

#Version debug de hex
hexDebug.o: module_hex/hex.c module_hex/hex.h
	$(CC) -c -D DEBUG -D VERBEUX $< $(CFLAGS) -o $@

calculDebug.o: module_calcul/calcul.c module_calcul/calcul.h
	$(CC) -c -D DEBUG $< $(CFLAGS) -o $@

test.o: test.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rf *.o
