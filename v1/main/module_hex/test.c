#include "hex.h"

instruction* trouveOperation(instruction* instructions[], char* nom) {
  int i=0, nonTrouvee=1;
  instruction *ret=NULL;
  while (nonTrouvee && i<NB_OPERATIONS) {
    if (instructions[i]->nom==nom) {
      ret=instructions[i];
      nonTrouvee=0;
    }
    i++;
  }

  afficheInstruction(instructions[i-1]);
  printf("Passed\n");
  return ret;
}

void afficheInstruction(instruction *instruction) {
  int i=0;
  printf("Case %i\n", i);
  printf("Nom : %s\n", instruction->nom);
  printf("opcode : %d\n", instruction->opcode);
  printf("Type d'instruction : %c\n", instruction->typeInstruction);
  printf("Ordre bits : %d\n", instruction->ordreBits);
  printf("Style de remplissage : %d\n", instruction->styleRemplissage);
  printf("\n");
}

int main() {
  instruction *instructions[NB_OPERATIONS+1];
  instruction *found=NULL;
  char* listeope="listeOpe.txt";
  char *nom="SUB";
  remplissageStructInstruction(instructions,listeope);
  afficheStructInstruction(instructions);
  found=trouveOperation(instructions,nom);
  afficheInstruction(found);
  return 0;
}
