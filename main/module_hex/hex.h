#ifndef _HEX_H_
#define _HEX_H_

#define TAILLE_MAX_OPERATEUR 9 /* SYSCALL est le plus long */
#define DEBUT_PROG 0xDDDC
#define NB_OPERATIONS 26

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

/* PARSSAGE */

/* Prend en entrée un chaine s (l'instruction) */
/* Retourne le nombre d'opérande dans un opération passée en entrée */
int nombreOperande(char* s);

/* Prend en entrée un pointeur vers une chaine non uniforisé et une pointeur pour stocké la chaine uniformisé */
/* S'occupe d'uniformiser la chaine */
void uniformisationInstruction(char* s, char* out);

/* Retourne 1 si le numéro est valide, 0 sinon */
/* Vérifie que num appartienne bien à l'intervale [min,max] */
int check(int num, int min, int max);

/* Retourne un tableau d'int contenant la valeur de toutes les operandes */
/* Si l'opérande est passé en hexadécimal elle est traduite en décimal */
/* Offset représente l'avancement dans le tableau de char ligne */
int* parseOperandes(char* ligne, int* offset, registre **registres);

/* Stocke dans le tableau de char operation l'opération assembleur de l'instruction de la ligne (ADD...) */
/* Offset représente l'avancement dans le tableau de char ligne */
void parseOperation(char* ligne, char* operation, int* offset);

/* Traduit une ligne passé en argument (*ligne) en une valeur hexadécimale stockée dans *instructionHex (passé par adresse) */
/* Retourne 0 si l'operation n'existe pas, est invalide ou que les valeurs sont out of range 1 sinon */
int parseLigne(char* ligne, unsigned long int* instructionHex, instruction **instructions, registre **registres);

/* Lit le fichier d'instruction assembleur ligne par ligne */
/* Parse l'expression et appele la fonction de traduction hexadécimale */
/* Mode défini si on est en mode auto (1) ou pàp (0) */
/* Prend les fichiers input et output, la memoire des registres et des instructions en entrée */
void parseFichier(char* input, char* output, int mode, instruction **instructions, registre **registres) ;

#endif
