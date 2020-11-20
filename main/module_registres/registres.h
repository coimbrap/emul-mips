#ifndef _REGISTRES_H_
#define _REGISTRES_H_

#define INIT_VALUE 0
#define TAILLE_MAX_MNEMO 5
#define NB_BIT_REGISTRE 32
#define NB_REGISTRE 35

typedef struct registre {
  int numero;
  char nom[TAILLE_MAX_MNEMO];
  long int valeur;
} registre;

/* Fonctions principale */
void remplissageStructRegistre(registre *registre[], const char* fichier);
void liberationRegistres(registre** registres);
registre* trouveRegistre(registre* registres[], char* nom);
long int valeurRegistre(registre* registres[], char* nom);
void traduitRegistre(registre* registres[], char* registre);

/* Affichage */
void afficheRegistre(registre *registre);
void afficheRegistres(registre *registres[]);
#endif
