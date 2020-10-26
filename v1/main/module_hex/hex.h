#ifndef _HEX_H_
#define _HEX_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAILLE_MAX_OPERATEUR 9 /* SYSCALL est le plus long */
#define TAILLE_MAX_REGISTE 2
#define TAILLE_BIT_OPERATION 32
#define TAILLE_HEX_OPERATION 8
#define TAILLE_OPCODE 7
#define NB_OPERATIONS 24
#define TAILLE_REG 5


typedef struct instruction instruction;

struct instruction {
  /* Nécessaicité de suivantColonne et suivantLigne ? Un pourrait suffire */
  char nom[TAILLE_MAX_OPERATEUR]; /* En ascii */
  char opcode[TAILLE_OPCODE+1]; /* ecrit sous forme binaire dans un char pour les 0 */
  char typeInstruction; /* 'R' || 'I' || 'J' */
  int ordreBits; /* Cas en fonction du type */
  int styleRemplissage; /* SG en fonction du type */
};

int puissance(int d, int n);
void binaryToHex(int* bin, char* hex);
void afficheBin(int* bin);
void afficheHex(char* hex);
void ecrireHex(char* hex, char *fichier);
void parseLigne(char *ligne, int* bin);
void remplissageStructInstruction(instruction *instructions[], const char* fichier);
instruction* trouveOperation(instruction* instructions[], char* nom);
void afficheStructInstruction(instruction *instructions[]);
void afficheInstruction(instruction *instruction);
char** parseRegistres(char *ligne, char* registres[], int* offset);
void afficheBin(int* bin);
char* enleveEspaces(char *s);
char* enleveCommentaires(char *s);
int nombreOperande(char *s);
int valeurDecimale(char *s);
void decToBinary(int n, int* offset, int* bin);
void rempliBinTabBin(char* cBin, int* offset, int* bin);
void parseR(char *ope, int offset);
void parseOperation(char *ligne, char* operation, int* offset); /* Retourne un string de l'opération (ADD...) */
void parseFichier(const char *nomFichier);
#endif
