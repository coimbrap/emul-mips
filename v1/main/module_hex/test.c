#include "hex.h"
#include <string.h>


int main() {
  instruction *instructions[NB_OPERATIONS+1];
  char* listeope="listeOpe.txt";
  char *nom="NOP";
  remplissageStructInstruction(instructions,listeope);
  afficheStructInstruction(instructions);
  instruction *found=NULL;
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }
/*  int i=0;
  char *ligne="LW $6,10($12)";
  char **registres=NULL;
  int offset=0;



  printf("Offset : %d\n", offset);

  registres=parseRegistres(ligne, registres, &offset);
  printf("%d|%d\n", nombreOperande(ligne),offset);
  for(i=0;i<nombreOperande(ligne);i++) {
    printf("%s\n", registres[i]);
  }*/
  return 0;
}
