#include "tools.h"
#include <stdio.h>
#include <stdlib.h>

/* OUTILS GENERAUX */

/* Calcul de la puissance */
int puissance(int d, int n){
  int i=0;
  int mul=1;
  for(i=0;i<n;i++){
    mul*=d;
  }
  return mul;
}

/* Inverse tout les éléments d'un tableau */
void inverseTab(int *tab, int n) {
  int i=0,j=n-1,tmp=0;
  while(i<j) {
    tmp=tab[i];
    tab[i++]=tab[j];
    tab[j--]=tmp;
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
  /* Pas de for pour déterminer le signe et pour s'arreter dès la fin du nombre */
  while (s[i]>='0' && s[i]<='9'){
    num=num*10+(s[i]-'0'); /* Manipulation ASCII */
    i++;
  }
  /* On retourne l'entier avec le bon signe */
  return sign*num;
}

/* MANIPILATION BINAIRE */

void inverseBin(int* binS, int* binO, int size) {
  int i=0;
  for (i=0;i<size;i++) {
    if (binS[i]) {
      binO[i]=0;
    }
    else {
      binO[i]=1;
    }
  }
}

void addBin(int* binA, int* binB, int* binR, int size) {
  int i=0,ret=0;
  inverseTab(binA,size);
  inverseTab(binB,size);
  for (i=size-1;i>=0;i--) {
    if (binA[i] && binB[i]) {
      binR[i]=ret;
      ret=1;
    }
    else if (!binA[i] && !binB[i]){
      binR[i]=ret;
      ret=0;
    }
    else {
      if(ret==1) {
        binR[i]=0;
        ret=1;
      }
      else {
        binR[i]=1;
        ret=0;
      }
    }
  }
  inverseTab(binR,size);
  /* On remet dans le bon sens */
  inverseTab(binA,size);
  inverseTab(binB,size);
}

void complementADeux(int* binI, int* binO, int size) {
  int *binTmp=NULL;
  int *binOne=NULL;
  int i=0;
  /* Allocation dynamique des tableaux temporaire */
  if((binTmp=malloc(sizeof(int)*size))==NULL){exit(1);};
  if((binOne=malloc(sizeof(int)*size))==NULL){exit(1);};
  binOne[0]=1;
  for (i=1;i<size;i++) {
    binOne[i]=0;
  }
  inverseBin(binI,binTmp,size);
  addBin(binTmp,binOne,binO,size);
  free(binTmp);
  free(binOne);
}

/* AFFICHAGE */

/* Affiche les informations contenu dans une structure de stockage */
void afficheInstruction(instruction *instruction) {
  printf("Nom : %s\n", instruction->nom);
  printf("opcode : %s\n", instruction->opcode);
  printf("Type d'instruction : %c\n", instruction->typeInstruction);
  printf("Ordre bits : %d\n", instruction->ordreBits);
  printf("Style de remplissage : %d\n", instruction->styleRemplissage);
  printf("\n");
}

/* Affiche toutes les structures du tableau de stockage */
void afficheStructInstruction(instruction *instructions[]) {
  int i=0;
  for(i=0;i<NB_OPERATIONS;i++) {
    printf("--%d--\n", i);
    afficheInstruction(instructions[i]);
  }
}

/* Affiche le tableau binaire de l'instruction */
void afficheBin(int* bin, int size) {
  int i=0;
  for (i=0;i<size;i++) {
    printf("%d",bin[i]);
  }
  printf("\n");
}

/* Affiche le tableau hexadécimal de l'instruction */
void afficheHex(char* hex) {
  int i=0;
  for (i=0;i<TAILLE_HEX_OPERATION;i++) {
    printf("%c",hex[i]);
  }
  printf("\n");
}

/* Affiche le tableau hexadécimal de l'instruction sans le \n */
void afficheHexNoEnter(char* hex) {
  int i=0;
  for (i=0;i<TAILLE_HEX_OPERATION;i++) {
    printf("%c",hex[i]);
  }
}
