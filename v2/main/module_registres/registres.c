#include "registres.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* traduitRegiste(registre* registres[], char* nom) {
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

void changeRegistre(registre* registre, int valeur[NB_BIT_REGISTRE]) {
  int i=0;
  for (i=0;i<NB_BIT_REGISTRE;i++) {
    registre->valeur[i]=valeur[i];
  }
}

/* Retourne un entier signé correspondant à un entier stocké dans un string */
int valeurDecimale(char *s) {
  int num=-1,i=0,sign=1;
  /* On détermine le signe */
  if(s[i]=='-'){
    sign=-1;
    i++;
  }
  if (s[i]>='0' && s[i]<='9') {
    num=0;
  }
  /* Pas de for pour déterminer le signe et pour s'arreter dès la fin du nombre */
  while (s[i]>='0' && s[i]<='9'){
    num=num*10+(s[i]-'0'); /* Manipulation ASCII */
    i++;
  }
  /* On retourne l'entier avec le bon signe */
  return sign*num;
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

char* intVersChaine(int num) {
  int tmp=0,sign=0;
  char *s=malloc(sizeof(int)*sizeof(char));
  *s='\0';
  --s;
  if (num==0) {
    *s='0';
  }
  if (num<0) {
    sign=1;
    num*=-1;
  }
  for (tmp=num;tmp>0;tmp/=10) {
    --s;
    *s=tmp%10+'0';
  }
  if (sign) {
    s--;
    *s='-';
  }
  return s;
}
