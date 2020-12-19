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
void liberationInstruction(instruction** instructions);

/* Prend en entrée la structure de stockage et le nom d'une instruction */
/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
instruction* trouveOperation(instruction** instructions, char* nom);

/* Prend en entrée la structure de stockage un opcode et un type d'opération */
/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
instruction* trouveOpcode(instruction** instructions, int opcode, char type);

/* PARSAGE */

/* prend en entrée le checksum calculé et le checksum théorique pour les registres et les immédiats */
/* les compare est dit ou sont les différences et donc ou sont les erreurs */
int compareChecksum(char* ligne, int checksumCalcR,int checksumTheorR, int checksumCalcI,int checksumTheorI, int type);

/* prend une ligne du fichier (src) un pointeur vers la chaine que l'on va parser */
/* ainsi que la taille du tableau opérandes et le nombre d'opérande que nous allons écrire par adresse */
/* met au propre de l'expression, chaque partie est séparée de la précédente par un espace */
/* retourne 0 -> echec | 1 -> label | 2 -> instruction */
int nettoyageInstruction(char *src, char **parse, int *tailleTab,int *nbOpe);

/* prend en entrée la ligne d'entrée, la table des symboles et la mémoire des instructions */
/* ainsi que des variables par adresses que la fonction va modifier lors du calcul des checksums */
/* pratique, de la mise en forme de l'expression et de l'identification de l'instruction */
/* retourne : 1 c'est bon on a calculé || 0 opération invalide */
int calculChecksum(instruction **instr, char *parse, char *out, int **incremOpe, registre **registres, int* nbOpe, int* nbImm, int* nbReg, instruction** instructions, symtable *symbols);

/* prend en entrée la mémoire des instruction, les registres, la mémoire, la table des symboles */
/* la ligne non édité, la ligne de sortie, un pointeur vers le tableau des opérandes et un pointeur vers la taille de ce tableau */
/* s'occupe de parser l'expression c'est à dire d'identifier toutes les erreur possible et de remplir le tableau des opérandes */
/* return 2: Expression valide || 1: Label || 0: Erreur */
int parsageInstruction(instruction **instr, instruction **instructions,registre** registres, symtable *symbols, char *s, char *out, int** operandes, int* tailleTab);

/* prend en entrée un nombre ainsi qu'une borne min et une borne max */
/* vérifie que num appartienne bien à l'intervale [min,max] */
/* retourne 1 si le numéro est valide, 0 sinon */
int check(char *ligne, int num, int min, int max);

/* on prend en entrée l'ensemble des mémoires (reg, symtable, instr...), la ligne pure, un pointeur vers la ligne de sortie */
/* Traduit une ligne passé en argument (*ligne) en un pointeur vers la valeur hexadécimale que l'on calculé (*instructionHex) */
/* Retourne 0 si l'operation n'existe pas, est invalide ou que les valeurs sont out of range 1 si elle est valide 2 si c'est une label */
int hexLigne(char *ligne, int pc, char **ligneParse, unsigned long int *instructionHex, symtable *symbols, instruction **instructions, registre **registres);
#endif
