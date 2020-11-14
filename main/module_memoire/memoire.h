#ifndef _MEMOIRE_H_
#define _MEMOIRE_H_

#define NB_BIT_MEMOIRE 32

#include "../module_tools/tools.h"

typedef struct element {
  int adresse;
  int valeur[NB_BIT_MEMOIRE];
  struct element* suivant;
} element;

typedef element* memoire;

int estDivisiblePar4(int n);
void insertion(int adresse, int mot[NB_BIT_MEMOIRE], memoire *m);
void suppression(int adresse, memoire *m);
void afficherMemoires(memoire *m);
void afficherMemoire(memoire slot);
void liberation(memoire *m);
int* valeurMemoire(int adresse, memoire *m);

#endif
