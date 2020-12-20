CC = gcc
CFLAGS = -Wall -pedantic -g -fsanitize=address
EXEC = emul-mips
LDFLAGS = -g -fsanitize=address

all: $(EXEC)

#Cible principale
$(EXEC): $(EXEC).o hex.o tools.o registres.o memoire.o calcul.o console.o table.o
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

console.o: module_console/console.c module_console/console.h
	$(CC) -c $< $(CFLAGS) -o $@

table.o: module_table/table.c module_table/table.h
	$(CC) -c $< $(CFLAGS) -o $@

tools.o: module_tools/tools.c module_tools/tools.h
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rf *.o

rendu:
	tar -zcf Rendu_Final_Barnel_Coimbra.tgz module_* src unit_tests tests_emul resultats_tests outputs emul-mips* Makefile
