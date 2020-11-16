#include "registres.h"
#include "../module_tools/tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Remplit la structure des registres à l'aide d'un fichier */
void remplissageStructRegistre(registre *registres[], const char* fichier) {
  FILE *freg=fopen(fichier,"r");
  registre *tmp=NULL;
  int i=0;
  if(freg==NULL) {
    printf("Erreur lors de l'ouverture du fichier");
    exit(-1);
  }
  fseek(freg,0,SEEK_SET);
  for(i=0;i<NB_REGISTRE;i++) {
    registres[i]=malloc(sizeof(registre));
    tmp=registres[i];
    fscanf(freg,"%d,%[^\n]",&tmp->numero,tmp->nom);
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

void traduitRegistre(registre* registres[], char* nom) {
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
  strcpy(nom,ret);
}
