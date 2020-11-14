#ifndef _HEX_H_
#define _HEX_H_

#define TAILLE_MAX_OPERATEUR 9 /* SYSCALL est le plus long */
#define TAILLE_MAX_REGISTE 2
#define TAILLE_BIT_OPERATION 32
#define TAILLE_HEX_OPERATION 8
#define TAILLE_OPCODE 7
#define TAILLE_REG 5
#define TAILLE_IMM 16
#define INIT_PC 0xDDE0;

#define NB_OPERATIONS 26

#define NB_REGISTRE 35
#define TAILLE_MAX_MNEMO 5
#define NB_BIT_REGISTRE 32

/* Module registres */
/* Ce module permet la traduction des mnémonique */
#include "../module_registres/registres.h"

typedef struct instruction {
  char nom[TAILLE_MAX_OPERATEUR]; /* En ascii */
  char opcode[TAILLE_OPCODE+1]; /* ecrit sous forme binaire dans un char pour les 0 | Pas int* pour fscanf */
  char typeInstruction; /* 'R' || 'I' || 'J' */
  int ordreBits; /* Cas en fonction du type */
  int styleRemplissage; /* SG en fonction du type */
} instruction;

/* MEMOIRE INSTRUCTIONS */
void remplissageStructInstruction(instruction *instructions[], const char* fichier);
instruction* trouveOperation(instruction* instructions[], char* nom);
instruction* trouveOpcode(instruction* instructions[], int* bin, char type);

/* UNIFORMISATION DE L'INSTRUCTION */
void uniformisationInstruction(char *s, char *out);

/* PARTIE BINAIRE */
void decToBinary(int n, int* offset, int* bin);
void decToBinaryImm(int n, int* offset, int* bin);
void rempliBinTabBin(char* cBin, int* offset, int* bin);
void binaryToHex(int* bin, char* hex, int size);
void ecrireHex(char* hex, char *fichier);

/* PARSSAGE DE L'INSTRUCTION */
int nombreOperande(char *s);
void traduitOperandes(registre* registres[], char* operandes[], int nbOperande);
char** parseOperandes(char *ligne, char* operandes[], int* offset);
void parseOperation(char *ligne, char* operation, int* offset);
int parseLigne(char *ligne, int* bin, instruction* instructions[], registre* registres[]);
void parseFichier(char *input, char* output, int mode);

#endif
