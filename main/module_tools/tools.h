#ifndef _TOOLS_H_
#define _TOOLS_H_

/* Import des entêtes du module hex (define & structure) */
#include "../module_hex/hex.h"

/* OUTILS GENERAUX */
unsigned numDigits(int n);
int puissance(int d, int n);
void inverseTab(int *tab, int n);
int valeurDecimale(char* m);
char* intVersChaine(int num);
long tabToInt(int* tab,int size);

/* MANIPILATION BINAIRE */
void inverseBin(int* binS, int* binO, int size);
void addBin(int* binA, int* binB, int* binR, int size);
void complementADeux(int* binI, int* binO, int size);
int binToDec(int *bin, int size);
int* decToBin(long int dec, int binSize);

/* MANIPULATION HEXADECIMALE */
long int hexToDec(char* hex);
char* decToHex(int dec);
void operandesHextoDec(char* hex);
void hexToBin(char* hex, int* bin);

/* AFFICHAGE */
void afficheInstruction(instruction *instruction);
void afficheStructInstruction(instruction *instructions[]);
void afficheBin(int* bin, int size);
void afficheBinNoEnter(int* bin, int size);
void afficheHex(char* hex);
void afficheHexNoEnter(char* hex);

#endif
