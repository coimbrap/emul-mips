#ifndef _REGISTRES_H_
#define _REGISTRES_H_

#define TAILLE_MAX_MNEMO 5
#define NB_BIT_REGISTRE 32
#define NB_REGISTRE 35

typedef struct registre {
  int numero;
  char nom[TAILLE_MAX_MNEMO];
  int valeur[NB_BIT_REGISTRE];
} registre;

void remplissageStructRegistre(registre *registre[], const char* fichier);
registre* trouveRegistre(registre* registres[], char* nom);
char* traduitRegistre(registre* registres[], char* registre);

/* Affichage */
void afficheRegistre(registre *registre);
void afficheRegistres(registre *registres[]);


#endif
