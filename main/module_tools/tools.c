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

/* Fonctionne uniquement avec des tableaux de 32 cases */
int decValue(int* binTab, int size) {
  int tailleHex=size/4;
  char *hexTab=NULL;
  hexTab=calloc(tailleHex+1,sizeof(char));
  binaryToHex(binTab,hexTab,size);
  return hexToDec(hexTab);
}

unsigned numDigits(int n) {
  if (n < 10) return 1;
  return 1 + numDigits(n / 10);
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

/* Prend en entrée la valeur hexadécimal dans un tableau de char */
/* Retourne la valeur décimale associé sous forme d'un entier */
int hexToDec(char* hex) {
  int i=0;
  int dec=0,base=1;
  int len=strlen(hex);
  if (len<=8) {
    for(i=len-1;i>=0;i--){
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

char* decToHex(int dec) {
  int i=0,j=0,tmp=0,k=0;
  char *hex=NULL;
  char *ret=NULL;
  if((hex=malloc(numDigits(dec)*sizeof(char)))==NULL){exit(1);};
  if((ret=malloc((numDigits(dec)+1)*sizeof(char)))==NULL){exit(1);};
  while (dec!=0) {
    tmp=dec%16;
    if (tmp<10) {
      hex[i]=tmp+'0';
      i++;
    }
    else {
      hex[i]=tmp+55;
      i++;
    }
    dec/=16;
  }
  for(j=i-1;j>=0;j--) {
    ret[k++]=hex[j];
  }
  ret[k]='\0';
  free(hex);
  return ret;
}

/* size comptant de 1 */
long tabToInt(int* tab,int size) {
  int i=0,ret=0;
  ret=tab[0];
  for (i=1;i<size;i++) {
    ret*=10;
    ret+=tab[i];
  }
  return ret;
}

int binToDec(int *bin, int size) {
  int dec=0,i=0,tmp=0;
  long binI;
  binI=tabToInt(bin,size);
  while (binI!= 0) {
    tmp=binI%10;
    binI/=10;
    dec+=tmp*puissance(2, i);
    i++;
  }
  return dec;
}

int* decToBin(int dec, int binSize) {
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

/* Fonction permettant de traduire des valeurs immédiate hexadécimale en valeur décimale pour la fonction parseOperandes */
/* Prend en entrée la valeur hexadécimal dans un tableau de char */
/* Ecrit la valeur décimale dans ce même tableau (pas de problème de taille pour un usage normal) */
void operandesHextoDec(char* hex) {
  int i=0,dec=0,inv=0;
  dec=hexToDec(hex);
  /* On inverse l'ordre du nombre */
  while (dec!=0) {
    inv*=10;
    inv+=dec%10;
    dec/=10;
  }
  /* On écrit le nombre sous ça forme décimale dans le tableau hex */
  while (inv>0) {
    hex[i]=(inv%10)+'0';
    inv/=10;
    i++;
  }
  /* On marque la fin de la chaine de caractères */
  hex[i]='\0';
}

void hexToBin(char* hex, int* bin) {
  int i=0;
  int j=0;
  int k=0;
  int temp=0;
  for(k=0;k<TAILLE_BIT_OPERATION;k++){
    bin[k]=0;
  }
  for(j=0;j<TAILLE_HEX_OPERATION;j++){
    if(hex[j]=='A'){
      temp=10;
    }
    else if(hex[j]=='B'){
      temp=11;
    }
    else if(hex[j]=='C'){
      temp=12;
    }
    else if(hex[j]=='D'){
      temp=13;
    }
    else if(hex[j]=='E'){
      temp=14;
    }
    else if(hex[j]=='F'){
      temp=15;
    }
    else{
      temp=hex[j]-48;
    }
    while(i<((j+1)*4)){
      if((temp/8)==1){
        bin[i]=1;
        temp=temp-8;
      }
      i=i+1;
      if((temp/4)==1){
        bin[i]=1;
        temp=temp-4;
      }
      i=i+1;
      if((temp/2)==1){
        bin[i]=1;
        temp=temp-2;
      }
      i=i+1;
      if((temp/1)==1){
        bin[i]=1;
        temp=temp-1;
     }
      i=i+1;
    }
  }
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
