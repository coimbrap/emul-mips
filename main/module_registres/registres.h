#ifndef _REGISTRES_H_
#define _REGISTRES_H_

#define INIT_VALUE 0
#define TAILLE_MAX_MNEMO 5
#define NB_BIT_REGISTRE 32
#define NB_REGISTRE 35

/* structure contenant toute les informations sur un registre */
typedef struct registre {
  int numero; /* Numéro du registre -1 si c'est un registre spécial (HI,LO,PC)*/
  char nom[TAILLE_MAX_MNEMO]; /* mnémonique du registre */
  long int valeur; /* Valeur non signée (complément à 2) du registre */
} registre;

/* MEMOIRE REGISTRES */

/* prend en entrée un tableau de pointeur vers la structure registre et le nom du fichier source de remplissage */
/* remplit le tableau à l'aide du fichier source de remplissage */
/* on part du principe que le fichier source est correctement renseigné */
void remplissageStructRegistre(registre *registre[], const char* fichier);

/* prend en entrée un tableau de pointeur vers la structure registre et une chaine représentant le nom du registre */
/* retourne un pointeur vers la structure contenant toutes les informations du registre en question */
registre* trouveRegistre(registre* registres[], char* nom);

/* prend en entrée un tableau de pointeur vers la structure registre et une chaine représentant le nom du registre */
/* retourne la valeur de la case mémoire sous forme de long int */
long int valeurRegistre(registre* registres[], char* nom);

/* prend en entrée un tableau de pointeur vers la structure registre et une chaine représentant le nom du registre */
/* si il y a lieu remplace le mnémonique du registre par sa valeur entière dans nom */
void traduitRegistre(registre* registres[], char* registre);

/* prend en entrée un tableau de pointeur vers la structure registre */
/* libère chaque case du tableau pour éviter les fuites en mémoires */
void liberationRegistres(registre** registres);

/* AFFICHAGE */

/* prend en entrée une structure d'un registre */
/* affiche la valeur décimale (sauf pour les registres spéciaux) ainsi que la mnémonique */
/* affiche aussi la valeur sous forme décimale signé, hexadécimale et binaire */
void afficheRegistre(registre *registre);

/* prend en entrée un tableau de pointeur vers la structure registre */
/* affiche toutes les structures du tableau */
void afficheRegistres(registre **registres);
#endif
