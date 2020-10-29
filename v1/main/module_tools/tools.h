#ifndef _TOOLS_H_
#define _TOOLS_H_

/* Import des entêtes du module hex (define & structure) */
#include "../module_hex/hex.h"

/* OUTILS GENERAUX */
int puissance(int d, int n);
void inverseTab(int *tab, int n);
int valeurDecimale(char* m);

/* MANIPILATION BINAIRE */
void inverseBin(int* binS, int* binO, int size);
void addBin(int* binA, int* binB, int* binR, int size);
void complementADeux(int* binI, int* binO, int size);

/* AFFICHAGE */
void afficheInstruction(instruction *instruction);
void afficheStructInstruction(instruction *instructions[]);
void afficheBin(int* bin, int size);
void afficheHex(char* hex);
void afficheHexNoEnter(char* hex);

#endif
