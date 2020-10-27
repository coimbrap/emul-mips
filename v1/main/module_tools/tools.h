#ifndef _TOOLS_H_
#define _TOOLS_H_

/* Import des entêtes du module hex (define & structure) */
#include "../module_hex/hex.h"

/* OUTILS GENERAUX */
int puissance(int d, int n);
void inverseTab(int *tab, int n);
int valeurDecimale(char* m);

/* AFFICHAGE */
void afficheInstruction(instruction *instruction);
void afficheStructInstruction(instruction *instructions[]);
void afficheBin(int* bin);
void afficheHex(char* hex);

#endif
