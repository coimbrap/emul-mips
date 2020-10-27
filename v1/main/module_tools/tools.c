#include "tools.h"
#include <stdio.h>

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
  int i=0,j=TAILLE_BIT_OPERATION-1,tmp=0;
  while(i<j) {
    tmp=tab[i];
    tab[i++]=tab[j];
    tab[j--]=tmp;
  }
}

/* Retourne un entier correspondant à un entier stocké dans un string */
int valeurDecimale(char* m) {
  int num=0,i=0;
  while(m[i]>='0' && m[i] <='9') {
    num=num*10+(m[i]-'0');
    i++;
  }
  return num;
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
void afficheBin(int* bin) {
  int i=0;
  for (i=0;i<TAILLE_BIT_OPERATION;i++) {
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
