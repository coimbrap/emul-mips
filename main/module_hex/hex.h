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
  unsigned int opcode; /* sous forme hexadécimale */
  char typeInstruction; /* 'R' || 'I' || 'J' */
  int ordreBits; /* Cas en fonction du type */
  int styleRemplissage; /* SG en fonction du type */
  int nbOperande; /* Nombre d'opérandes requis pour fonctionner */
} instruction;

/* MEMOIRE INSTRUCTIONS */
void remplissageStructInstruction(instruction *instructions[], const char* fichier);
instruction* trouveOperation(instruction* instructions[], char* nom);
instruction* trouveOpcode(instruction* instructions[], int opcode, char type);

/* PARSSAGE DE L'INSTRUCTION */
int nombreOperande(char *s);
void uniformisationInstruction(char *s, char *out);
int* parseOperandes(char *ligne, int* offset, registre** registres);
void parseOperation(char *ligne, char* operation, int* offset);
int parseLigne(char *ligne, long int* hex, instruction* instructions[], registre* registres[]);
void parseFichier(char *input, char* output, int mode);

#endif
