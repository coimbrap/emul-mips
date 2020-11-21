#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* OUTILS GENERAUX */

/* Vide le buffer stdin */
void clean_stdin() {
  char c;
  do {
    c=getchar();
  } while (c!='\n' && c!=EOF);
}

/* Retourne le complément à deux de value */
/* Prend en entrée un entier et le nombre de bits */
int complementADeux(int value, int bits) {
  if ((value & (1<<(bits-1)))!=0) {
    value=value-(1<<bits);
  }
  return value;
}

/* Prend en entrée une chaine signé */
/* Retourne un entier signé correspondant à l'entier stocké dans la chaine */
int valeurDecimale(char* s) {
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

/* Prend en entrée un entier signé et une chaine */
/* transforme l'entier en une chaine de caractère et la retourne */
char* intVersChaine(int num, char* s) {
  /* Si la chaine est allouée */
  if (s!=NULL) {
    sprintf(s,"%d",num); /* On stocke la chaine num formaté en entier dans s */
  }
  return s; /* On retourne la "nouvelle" valeur du pointeur */
}

/* Prend en entrée la valeur hexadécimal dans un tableau de char */
/* Retourne la valeur décimale associé sous forme d'un entier */
unsigned long int hexToDec(char* hex) {
  int i=0;
  unsigned long int dec=0,base=1;
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
      dec=complementADeux(dec,len*4);
    }
  }
  else {
    printf("Nombre trop grand\n");
  }
  return dec;
}

/* AFFICHAGE */

/* Affiche la valeur binaire d'un nombre decimal */
void decToBin(unsigned long int dec) {
  int p=0,b=0;
  for (p=31;p>=0;p--) {
    b=dec>>p;
    if (b&1)
      printf("1");
    else
      printf("0");
  }
  printf("\n");
}
