#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* OUTILS GENERAUX */

void clean_stdin() {
  int c;
  do {
    c=getchar();
  } while (c!='\n' && c!=EOF);
}

int complementInt(int value, int bits) {
  if ((value & (1 << (bits-1))) !=0) {
    value=value-(1 << bits);
  }
  return value;
}

/* Calcul de la puissance */
int puissance(int d, int n){
  int i=0;
  int mul=1;
  for(i=0;i<n;i++){
    mul*=d;
  }
  return mul;
}

/* Change le sens d'un tableau */
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
  if (s!=NULL) {
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
  }
  return sign*num;
}

/* Int vers string de l'int */
char* intVersChaine(int num) {
  int tmp=0,sign=0;
  char *s=NULL;
  if((s=malloc(sizeof(int)*sizeof(char)))==NULL){exit(1);};
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

/* MANIPILATION BINAIRE */

/* Prend en entrée un tableau d'entier représentant un nombre binaire ainsi que ça taille */
/* Inverse la valeur (en binaire) de chaque case du tableau passé en entrée dans le tableau binO */
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

/* Prend en entrée deux tableaux d'entier représentant deux nombre binaire et leur taille (la même) size */
/* Place la somme de ces deux nombres binaire dans le tableau binR passé en argument */
void addBin(int* binA, int* binB, int* binR, int size) {
  int i=0,ret=0;
  /* On renverse les tableaux pour la retenue */
  inverseTab(binA,size);
  inverseTab(binB,size);
  for (i=size-1;i>=0;i--) {
    /* Si on a les deux à 1 on met la retenu dans le tableau résultat et la retenue à 1 */
    if (binA[i] && binB[i]) {
      binR[i]=ret;
      ret=1;
    }
    /* Si les deux sont à 0 en met la retenu dans le tableau résultat et la retenue à 0 */
    else if (!binA[i] && !binB[i]){
      binR[i]=ret;
      ret=0;
    }
    /* Sinon un des deux vaut 1 */
    else {
      /* Si la retenu est à 1 on met 0 dans le tableau résultat et la retenue à 1 */
      if(ret==1) {
        binR[i]=0;
        ret=1;
      }
      /* Sinon on met 1 dans le tableau résulat et la retenu à 0 */
      else {
        binR[i]=1;
        ret=0;
      }
    }
  }
  /* On remet dans le bon sens */
  inverseTab(binR,size);
  inverseTab(binA,size);
  inverseTab(binB,size);
}

/* Prend en entrée un tableau d'entier représentant un nombre binaire ainsi que ça taille */
/* Ecrit le complément à deux dans le tableau binO aussi passé en entréé */
void complementADeux(int* binI, int* binO, int size) {
  int *binTmp=NULL;
  int *binOne=NULL;
  int i=0;
  /* Allocation dynamique des tableaux temporaire */
  if((binTmp=malloc(sizeof(int)*size))==NULL){exit(1);};
  if((binOne=malloc(sizeof(int)*size))==NULL){exit(1);};
  /* Initialisation du tableau contenant 1 */
  binOne[0]=1;
  for (i=1;i<size;i++) {
    binOne[i]=0;
  }
  /* On inverse le tableau les 0 deviennent 1 et inversement */
  inverseBin(binI,binTmp,size);
  /* On ajoute un (def du complément à deux) */
  addBin(binTmp,binOne,binO,size);
  /* On libère les tableaux temporaire */
  free(binTmp);
  free(binOne);
}

/* MANIPULATION HEXADECIMALE */

/* GARDE */
/* Prend en entrée la valeur hexadécimal dans un tableau de char */
/* Retourne la valeur décimale associé sous forme d'un entier */
long int hexToDec(char* hex) {
  int i=0;
  long int dec=0,base=1;
  int len=strlen(hex)-1;
  if (len<=8) {
    for(i=len;i>=0;i--){
      if (hex[i]>='0' && hex[i]<='9') {
        /* Si c'est un nombre on le transforme en entier et on le multiplie par la base */
        dec+=(hex[i]-'0')*base;
        base*=16; /* On passe à la puissance d'après */
      }
      /* Pour le cas d'une lettre on trouve ça valeur décimale avec une manipulation ascii */
      else if (hex[i]>='a' && hex[i]<='f') {
        dec+=(hex[i]-'a'+10)*base;
        base*=16;
      }
      else if (hex[i]>='A' && hex[i]<='F') {
        dec+=(hex[i]-'A'+10)*base;
        base*=16;
      }
    }
    /* Gère les valeurs négative pour des nombre de moins de 8octets et donc le nombre d'octet est une mutlitple de 4*/
    if (len!=8 && len%4==0) {
      dec=complementInt(dec,len*4);
    }
  }
  else {
    printf("Nombre trop grand\n");
  }
  return dec;
}


/* GARDE */
int* decToBin(long int dec, int binSize) {
  int *bin=NULL,*binTmp=NULL;
  int i=0,neg=0;
  bin=calloc(binSize,sizeof(int));
  binTmp=calloc(binSize,sizeof(int));
  if (dec<0) {
    neg=1;
    dec=-dec;
  }
  while (dec>0) {
    bin[i]=dec%2;
    dec/=2;
    i++;
  }
  if (neg) {
    complementADeux(bin,binTmp,binSize);
    bin=binTmp;
  }
  inverseTab(bin,binSize);
  return bin;
}


/* AFFICHAGE */

/* Affiche les informations contenu dans une structure de stockage */
void afficheInstruction(instruction *instruction) {
  printf("Nom : %s\n", instruction->nom);
  printf("opcode : %06d\n", instruction->opcode);
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
