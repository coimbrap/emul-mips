#ifndef _TOOLS_H_
#define _TOOLS_H_

/* Import des entêtes du module hex (define & structure) */
#include "../module_hex/hex.h"

#define TAILLE_MAX_INT 12 /* Pour la transformation en chaine d'un int, maximum 11 caractères */

/* OUTILS GENERAUX */

/* Vide le buffer stdin */
void clean_stdin();

/* Retourne le complément à deux de value */
/* Prend en entrée un entier et le nombre de bits */
int complementInt(int value, int bits);

/* Prend en entrée une chaine signé */
/* Retourne un entier signé correspondant à l'entier stocké dans la chaine */
int valeurDecimale(char *s);

/* Prend en entrée un entier signé et une chaine */
/* transforme l'entier en une chaine de caractère et la retourne */
char* intVersChaine(int num, char* s);

/* Prend en entrée la valeur hexadécimal dans un tableau de char */
/* Retourne la valeur décimale associé sous forme d'un entier */
long int hexToDec(char* hex);

/* AFFICHAGE */

/* Affiche la valeur binaire d'un nombre decimal */
void decToBin(long int dec);

/* Affiche les informations contenu dans une structure de stockage */
void afficheInstruction(instruction *instruction);

/* Affiche toutes les structures du tableau de stockage */
void afficheStructInstruction(instruction *instructions[]);

#endif
