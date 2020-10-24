#include "hex.h"
#include <string.h>


int main() {
/*  instruction *instructions[NB_OPERATIONS+1];
  char* listeope="listeOpe.txt";
  char *nom="ADD";
  remplissageStructInstruction(instructions,listeope);
  afficheStructInstruction(instructions);
  instruction *found=NULL;
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }*/
  int i=0;
  char *ligne="ADD $6,$31,$1";
  char **registres=NULL;
  int offset=4;
  registres=parseRegistres(ligne, registres, &offset);
  printf("%d|%d\n", nombreOperande(ligne),offset);
  for(i=0;i<nombreOperande(ligne);i++) {
    printf("%s\n", registres[i]);
  }
  return 0;
}
