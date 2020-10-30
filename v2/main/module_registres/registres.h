#ifndef _REGISTRES_H_
#define _REGISTRES_H_

#define INIT_VALUE 0
#define TAILLE_MAX_MNEMO 5
#define NB_BIT_REGISTRE 32
#define NB_REGISTRE 35

typedef struct registre {
  int numero;
  char nom[TAILLE_MAX_MNEMO];
  int valeur[NB_BIT_REGISTRE];
} registre;


void remplissageStructRegiste(registre *registre[], const char* fichier);
void afficheRegistre(registre *registre);
void afficheRegistres(registre *registres[]);
void afficheBin(int* bin, int size);
int valeurDecimale(char *s);
registre* trouveRegistre(registre* registres[], char* nom);
void changeRegistre(registre* registre, int valeur[NB_BIT_REGISTRE]);

#endif
