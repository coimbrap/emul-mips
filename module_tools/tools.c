#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

/* prend en entrée une chaine de caractère */
/* strip tout les caractères non alphanumérique */
void stripAlphanum(char *s) {
    int i=0,j=0;
    char c='0';
    while ((c=s[i++])!='\0') {
        if (isalnum(c)) {s[j++]=c;}
    }
    s[j]='\0';
}

/* Prend en entrée une chaine signé */
/* Retourne un entier signé correspondant à l'entier stocké dans la chaine */
int valeurDecimale(char* s) {
  int num=-1,i=0,sign=1;
  char *buff=NULL,*tmp=NULL;
  char *tampon=NULL; /* tampon pour le stockage de strtok_r permet de faire du threading entre les deux fonctions */
  if (s!=NULL) {
    if (s[i]=='-') {sign=-1;};
    if ((strchr(s,'x'))!=NULL) { /* Si hexadécimal on convertit avec une autre fonction */
      /* Pour ne pas altérer la chaine de départ */
      if((buff=malloc(strlen(s)+1))==NULL){exit(0);};
      strcpy(buff,s);
      tmp=buff;
      num=hexToDec(strtok_r(tmp,"(",&tampon)); /* En cas d'offset imm(reg) on prend tout avant la première parenthèse car lecture dans le sens inverse */
      free(buff);
    }
    else {
      while (s[i]!='\0' && s[i]!='(') {
        if (s[i]=='-') {sign=-1;}
        else if (s[i]>='0' && s[i]<='9') {
          if (num==-1) {num=0;};
          num=num*10+(s[i]-'0');
        }
        i++;
      }
    }
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
    for(i=len;i>=0;i--) {
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
    /* Gère les valeurs négative pour des nombre de moins de 8octets et donc le nombre d'octet est un multiple de 4*/
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
