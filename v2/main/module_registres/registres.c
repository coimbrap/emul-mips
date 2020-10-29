#include "registres.h"
#include <stdio.h>
#include <stdlib.h>

/* Remplit la structure des registres à l'aide d'un fichier */
void remplissageStructRegiste(registre *registres[], const char* fichier) {
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

/* AFFICHAGE */

/* Affiche le tableau binaire de l'instruction */
void afficheBin(int* bin, int size) {
  int i=0;
  for (i=0;i<size;i++) {
    printf("%d",bin[i]);
  }
  printf("\n");
}

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
