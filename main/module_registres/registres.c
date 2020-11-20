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
    if((registres[i]=malloc(sizeof(registre)))==NULL){exit(1);};
    tmp=registres[i];
    fscanf(freg,"%d,%[^\n]",&tmp->numero,tmp->nom);
    tmp->valeur=INIT_VALUE;
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
long int valeurRegistre(registre* registres[], char* nom) {
  int i=0, nonTrouvee=1, special=1,find=-2;
  registre *ret=NULL;
  traduitRegistre(registres,nom);
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

void traduitRegistre(registre* registres[], char* nom) {
  char *ret=NULL;
  if((ret=(char *)calloc(TAILLE_MAX_INT,sizeof(char)))==NULL){exit(1);};
  registre *found=NULL;
  if (valeurDecimale(nom)!=-1) {
    strcpy(ret,nom);
  }
  else {
    found=trouveRegistre(registres,nom);
    if (found!=NULL) {
      intVersChaine(found->numero,ret);
    }
  }
  if (valeurDecimale(ret)==-1) {
    strcpy(ret,nom);
  }
  strcpy(nom,ret);
  free(ret);
}

void liberationRegistres(registre** registres) {
  int i=0;
  for (i=0;i<NB_REGISTRE;i++) {
    free(registres[i]);
  }
}

/* AFFICHAGE */

/* Affiche les informations contenu dans une structure de stockage */
void afficheRegistre(registre *registre) {
  if (strcmp(registre->nom,"zero")==0) {
    printf("$%d ($%s)  %-11d   0x%08x   ",registre->numero,registre->nom,(int)registre->valeur,(int)registre->valeur);
  }
  else if (registre->numero==-1) {
    printf("$%s         %-11d   0x%08x   ",registre->nom,(int)registre->valeur,(int)registre->valeur);

  }
  else if (registre->numero<10) {
    printf("$%d ($%s)    %-11d   0x%08x   ",registre->numero,registre->nom,(int)registre->valeur,(int)registre->valeur);
  }
  else {
    printf("$%d ($%s)   %-11d   0x%08x   ",registre->numero,registre->nom,(int)registre->valeur,(int)registre->valeur);
  }
  decToBin(registre->valeur);
}

/* Affiche toutes les structures du tableau de stockage */
void afficheRegistres(registre *registres[]) {
  int i=0;
  printf("\nRegistre    Décimal      Hex          Binaire\n--------------------------------------------------------------------------\n");
  for(i=0;i<NB_REGISTRE;i++) {
    afficheRegistre(registres[i]);
  }
}
