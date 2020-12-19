#ifndef _HEX_H_
#define _HEX_H_

#include "../module_tools/tools.h"
#include "../module_registres/registres.h" /* Ce module permet la traduction des mnémonique */
#include "../module_memoire/memoire.h" /* Ce module permet la traduction des mnémonique */
#include "../module_table/table.h"

#define TAILLE_MAX_OPERATEUR 9 /* SYSCALL est le plus long */
#define DEBUT_PROG 0xDDDC
#define NB_OPERATIONS 26

typedef struct instruction {
  char nom[TAILLE_MAX_OPERATEUR]; /* en ascii */
  unsigned int opcode; /* entier de l'instruction */
  char typeInstruction; /* 'R' || 'I' || 'J' */
  int ordreBits; /* Ordre des bits */
  int styleRemplissage; /* Style de remplissage des champs */
  int nbOperande; /* Nombre d'opérandes requis pour fonctionner */
  int checksumReg; /* Checksum théorique des registres */
  int checksumImm; /* Checksum théorique des immédiats */
} instruction;

/* MEMOIRE INSTRUCTIONS */

/* Prend en entrée la structure de stockage vide et le fichier de stockage */
/* Remplit la structure de stockage à l'aide du fichier contenant les opérations */
void remplissageStructInstruction(instruction **instructions, const char* fichier);

/* Affiche les informations contenu dans une structure de stockage */
void afficheInstruction(instruction *instruction);

/* Affiche toutes les structures du tableau de stockage */
void afficheStructInstruction(instruction **instructions);

/* Prend en entrée la structure de stockage et la libère */
void liberationInstruction(instruction **instructions);

/* Prend en entrée la structure de stockage et le nom d'une instruction */
/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
instruction* trouveOperation(instruction **instructions, char* nom);

/* Prend en entrée la structure de stockage un opcode et un type d'opération */
/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
instruction* trouveOpcode(instruction **instructions, int opcode, char type);

/* PARSAGE */

/* prend en entrée le checksum calculé et le checksum théorique pour les registres et les immédiats */
/* les compare est dit ou sont les différences et donc ou sont les erreurs */
int compareChecksum(char* ligne, int checksumCalcR,int checksumTheorR, int checksumCalcI,int checksumTheorI, int type);

/* Prend en entrée un pointeur vers une chaine non uniforisé et une pointeur pour stocké la chaine uniformisé */
/* S'occupe d'uniformiser la chaine */
int parsageInstruction(instruction **instr, instruction **instructions,registre** registres, symtable *symbols, char *s, char *out, int** operandes, int* tailleTab);

/* Retourne 1 si le numéro est valide, 0 sinon */
/* Vérifie que num appartienne bien à l'intervale [min,max] */
int check(char* ligne, int num, int min, int max);

int nettoyageInstruction(char *s, char **parse, int *tailleTab,int *nbOpe);

int calculChecksum(instruction **instr, char *parse, char *out, int **incremOpe, registre **registres, int* nbOpe, int* nbImm, int* nbReg, instruction** instructions, symtable *symbols);

/* Traduit une ligne passé en argument (*ligne) en une valeur hexadécimale stockée dans *instructionHex (passé par adresse) */
/* Retourne 0 si l'operation n'existe pas, est invalide ou que les valeurs sont out of range 1 sinon */
int hexLigne(char *ligne, int pc, char **ligneParse, unsigned long int *instructionHex, symtable *symbols, instruction **instructions, registre **registres);

#endif
