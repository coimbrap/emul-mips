#ifndef _MEMOIRE_H_
#define _MEMOIRE_H_

/* La mémoire est de la forme suivante :
0xFFFF +------------------------+
       |                        |
       |       Programme        |
       |                        |
0xDDDC +------------------------+
       |                        |
       |          Pile          |
       |                        |
0xAAAC +------------------------+
       |                        |
       |        Mémoire         |
       |                        |
0x0000 +------------------------+*/

#define DEBUT_MEMOIRE 0x0000
#define DEBUT_PILE 0xAAAC
#define DEBUT_PROG 0xDDDC
#define FIN_MEM 0xFFFF

#include "../module_tools/tools.h"

/* structure de type liste chainée contenant une valeur de type unsigned long int */
/* cette implémentation à l'avantage de supporté l'adressage à l'octet et l'adressage au mot */
typedef struct element {
  int adresse; /* Adresse entre 0x0000 et 0xFFFF */
  unsigned long int valeur; /* Valeur contenu dans la case mémoire */
  struct element* suivant; /* Adresse de l'élément suivant NULL si dernier */
} element;

typedef element* memoire;

/* MEMOIRE */

/* prend en entrée un pointeur vers la mémoire, une adresse et un mot à écrire */
/* écrit dans le mémoire à l'adresse donnée le mot donné en argument */
/* si il y a déjà une valeur on la remplace */
/* on garde la liste triéée par adresse */
/* vérifie que l'adresse et bien celle du premier octet d'un mot */
void insertion(int adresse, unsigned long int mot, memoire *m);

/* prend en entrée un pointeur vers la mémoire et une adresse */
/* supprime la case mémoire associé à l'adresse */
/* vérifie que l'adresse et bien celle du premier octet d'un mot */
void suppression(int adresse, memoire *m);

/* prend en entrée un pointeur vers la mémoire et une adresse */
/* va chercher la valeur en mémoire correspondant à cette adresse et la retourne */
/* si elle n'est pas présente retourne 0 */
/* vérifie que l'adresse et bien celle du premier octet d'un mot */
unsigned long int valeurMemoire(int adresse, memoire *m);

/* prend en entrée un pointeur vers la mémoire */
/* libère toute les cases mémoire dans l'ordre de parcourt pour éviter les fuites mémoire */
/* *m pointera donc vers NULL à la fin */
void liberation(memoire *m);

/* AFFICHAGE */

/* prend en entrée un pointeur vers la mémoire une adresse de début et une de fin de lecture */
/* si la mémoire est vide affiche une message d'avertissement */
/* sinon affiche toute le mémoire entre adresseMin inclu et adresseMax exclu*/
void afficherMemoires(memoire *m, int adresseMin, int adresseMax);

/* prend en entré un slot de la mémoire */
/* affiche l'adresse en hexa ainsi que les représentation décimal hexadécimale et binaire de la valeur en mémoire */
void afficherMemoire(memoire slot);

#endif
