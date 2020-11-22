#ifndef _TOOLS_H_
#define _TOOLS_H_

#define TAILLE_MAX_INT 12 /* Pour la transformation en chaine d'un int, maximum 11 caractères */

/* OUTILS GENERAUX */

/* Vide le buffer stdin */
void clean_stdin();

/* Retourne le complément à deux de value */
/* Prend en entrée un entier et le nombre de bits */
int complementADeux(int value, int bits);

/* Prend en entrée une chaine signé */
/* Retourne un entier signé correspondant à l'entier stocké dans la chaine */
int valeurDecimale(char *s);

/* Prend en entrée un entier signé et une chaine */
/* transforme l'entier en une chaine de caractère et la retourne */
char* intVersChaine(int num, char* s);

/* Prend en entrée la valeur hexadécimal dans un tableau de char */
/* Retourne la valeur décimale associé sous forme d'un entier */
unsigned long int hexToDec(char* hex);

/* AFFICHAGE */

/* Affiche la valeur binaire d'un nombre decimal */
void decToBin(unsigned long int dec);

#endif
