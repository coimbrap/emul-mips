#ifndef _TOOLS_H_
#define _TOOLS_H_

/* Import des entêtes du module hex (define & structure) */
#include "../module_hex/hex.h"

#define TAILLE_MAX_INT 12

/* OUTILS GENERAUX */
unsigned numDigits(int n);
void clean_stdin();
int complementInt(int value, int bits);
int valeurDecimale(char *s);
char* intVersChaine(int num, char* s);
long int hexToDec(char* hex);

/* AFFICHAGE */
void decToBin(long int dec);
void afficheInstruction(instruction *instruction);
void afficheStructInstruction(instruction *instructions[]);

#endif
