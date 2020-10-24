#ifndef _HEX_H_
#define _HEX_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAILLE_MAX_OPERATEUR 6
#define TAILLE_MAX_REGISTE 2
#define TAILLE_BIT_OPERATION 32
#define TAILLE_OPCODE 6
#define NB_OPERATIONS 24


typedef struct instruction instruction;

struct instruction {
  /* Nécessaicité de suivantColonne et suivantLigne ? Un pourrait suffire */
  char nom[TAILLE_MAX_OPERATEUR]; /* En ascii */
  int opcode; /* ecrit sous forme binaire */
  char typeInstruction; /* 'R' || 'I' || 'J' */
  int ordreBits; /* Cas en fonction du type */
  int styleRemplissage; /* SG en fonction du type */
};


void remplissageStructInstruction(instruction *instructions[], const char* fichier);
void afficheStructInstruction(instruction *instructions[]);
void afficheBin(int* bin);
char* enleveEspaces(char *s);
char* enleveCommentaires(char *s);
int nombreOperande(char *s);
int valeurDecimale(char *s);
void decToBinary(int n, int offset, int* bin);
void rempliBinTabBin(char* cBin, int offset, int* bin);
void parseR(char *ope, int offset);
void parseOperation(char *ope);
void parseFichier(char *nomFichier);
#endif
