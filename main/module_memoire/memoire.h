#ifndef _MEMOIRE_H_
#define _MEMOIRE_H_

typedef struct element{
  int adresse;
  int valeur;
  struct element* suivant;
} element;

typedef element* memoire;

int estDivisiblePar4(int n);
void insertion(int adresse, int mot, memoire *m);
void suppression(int adresse, memoire *m);
void afficherMemoire(memoire *m);
void liberation(memoire *m);

#endif
