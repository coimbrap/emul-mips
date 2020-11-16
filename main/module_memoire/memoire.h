#ifndef _MEMOIRE_H_
#define _MEMOIRE_H_

#define NB_BIT_MEMOIRE 32

#include "../module_tools/tools.h"

typedef struct element {
  int adresse;
  long int valeur;
  struct element* suivant;
} element;

typedef element* memoire;

/* Fonctions principale */
void insertion(int adresse, long int mot, memoire *m);
void suppression(int adresse, memoire *m);
long int valeurMemoire(int adresse, memoire *m);
void liberation(memoire *m);

/* Affichage */
void afficherMemoires(memoire *m);
void afficherMemoire(memoire slot);

#endif
