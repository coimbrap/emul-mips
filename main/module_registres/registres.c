#include "registres.h"
#include "../module_tools/tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Remplit la structure des registres à l'aide d'un fichier */
void remplissageStructRegistre(registre *registres[], const char* fichier) {
  FILE *freg=fopen(fichier,"r");
  registre *tmp=NULL;
  int i=0,j=0;
  if(freg==NULL) {
    printf("Erreur lors de l'ouverture du fichier");
    exit(-1);
  }
  fseek(freg,0,SEEK_SET);
  for(i=0;i<NB_REGISTRE;i++) {
    registres[i]=malloc(sizeof(registre));
    tmp=registres[i];
    fscanf(freg,"%d,%[^\n]",&tmp->numero,tmp->nom);
    for(j=0;j<NB_BIT_REGISTRE;j++) {
      tmp->valeur[j]=INIT_VALUE;
    }
    fgetc(freg); /* Enlève \n */
  }
  fclose(freg);
}

/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
registre* trouveRegistre(registre* registres[], char* nom) {
  int i=0, nonTrouvee=1, special=1,find=-2;
  registre *ret=NULL;
  if (valeurDecimale(nom)!=-1) {
    special=0;
    find=valeurDecimale(nom);
  }
  while (nonTrouvee && i<NB_REGISTRE) {
    if (special && strcmp(registres[i]->nom,nom)==0) {
      ret=registres[i];
      nonTrouvee=0;
    }
    else if (!special && find==registres[i]->numero) {
      ret=registres[i];
      nonTrouvee=0;
    }
    i++;
  }
  return ret;
}

/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
int* valeurRegistre(registre* registres[], char* nom) {
  int i=0, nonTrouvee=1, special=1,find=-2;
  registre *ret=NULL;
  if (valeurDecimale(nom)!=-1) {
    special=0;
    find=valeurDecimale(nom);
  }
  while (nonTrouvee && i<NB_REGISTRE) {
    if (special && strcmp(registres[i]->nom,nom)==0) {
      ret=registres[i];
      nonTrouvee=0;
    }
    else if (!special && find==registres[i]->numero) {
      ret=registres[i];
      nonTrouvee=0;
    }
    i++;
  }
  return ret->valeur;
}

char* traduitRegistre(registre* registres[], char* nom) {
  char *ret=NULL;
  registre *found=NULL;
  if (valeurDecimale(nom)!=-1) {
    ret=nom;
  }
  else {
    found=trouveRegistre(registres,nom);
    if (found!=NULL) {
      ret=intVersChaine(found->numero);
    }
  }
  if (valeurDecimale(ret)==-1) {
    ret=nom;
  }
  return ret;
}

void changeRegistre(registre* registre, int valeur[NB_BIT_REGISTRE]) {
  int i=0;
  for (i=0;i<NB_BIT_REGISTRE;i++) {
    registre->valeur[i]=valeur[i];
  }
}

/* AFFICHAGE */

/* Affiche les informations contenu dans une structure de stockage */
void afficheRegistre(registre *registre) {
  printf("Numéro : %d\n", registre->numero);
  printf("Nom : %s\n", registre->nom);
  afficheBin(registre->valeur,NB_BIT_REGISTRE);
}

/* Affiche toutes les structures du tableau de stockage */
void afficheRegistres(registre *registres[]) {
  int i=0;
  for(i=0;i<NB_REGISTRE;i++) {
    printf("--%d--\n", i);
    afficheRegistre(registres[i]);
  }
}
