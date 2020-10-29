#ifndef _HEX_H_
#define _HEX_H_

#define TAILLE_MAX_OPERATEUR 9 /* SYSCALL est le plus long */
#define TAILLE_MAX_REGISTE 2
#define TAILLE_BIT_OPERATION 32
#define TAILLE_HEX_OPERATION 8
#define TAILLE_OPCODE 7
#define NB_OPERATIONS 25
#define TAILLE_REG 5
#define TAILLE_IMM 16


typedef struct instruction instruction;

struct instruction {
  /* Nécessaicité de suivantColonne et suivantLigne ? Un pourrait suffire */
  char nom[TAILLE_MAX_OPERATEUR]; /* En ascii */
  char opcode[TAILLE_OPCODE+1]; /* ecrit sous forme binaire dans un char pour les 0 */
  char typeInstruction; /* 'R' || 'I' || 'J' */
  int ordreBits; /* Cas en fonction du type */
  int styleRemplissage; /* SG en fonction du type */
};

/* MEMOIRE INSTRUCTIONS */
void remplissageStructInstruction(instruction *instructions[], const char* fichier);
instruction* trouveOperation(instruction* instructions[], char* nom);

/* UNIFORMISATION DE L'INSTRUCTION */
void uniformisationInstruction(char *s, char *out);

/* PARTIE BINAIRE */
void decToBinary(int n, int* offset, int* bin);
void decToBinaryImm(int n, int* offset, int* bin);
void rempliBinTabBin(char* cBin, int* offset, int* bin);
void binaryToHex(int* bin, char* hex);
void ecrireHex(char* hex, char *fichier);

/* PARSSAGE DE L'INSTRUCTION */
int nombreOperande(char *s);
char** parseOperandes(char *ligne, char* operandes[], int* offset);
void parseOperation(char *ligne, char* operation, int* offset);
void parseLigne(char *ligne, int* bin);
void parseFichier(const char *nomFichier);

#endif
