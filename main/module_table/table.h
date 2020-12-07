#ifndef _TABLE_H_
#define _TABLE_H_

typedef struct symbol {
  char *nom;
  int valeur;
  struct symbol* suivant;
} symbol;

typedef symbol* symtable;

//#include "../module_hex/hex.h"

void insertionTete(symtable *tableorigine, char* nom, int valeur);
void insertionQueue(symtable *tableorigine, char* nom, int valeur);
void supressionTete(symtable *tableorigine);
void tableAffiche(symtable ptr);
void libereTable(symtable *symbols);
int foundSymbol(symtable *symbols, char* symbol);
//void fillSymbols(char *input, instruction **instructions, registre** registres, symtable *symbols);

#endif
