#include "module_hex/hex.h"

int main() {
  int l=0;
  char *nomFichier="tests_emul/test.txt";
  char *binNum="11101001";
  char *listeope="module_hex/listeOpe.txt";
  char *nom="ADD";
  instruction *instructions[NB_OPERATIONS+1];
  instruction *found=NULL;
  int offsetBin=0;

  int bin[TAILLE_BIT_OPERATION];
  for (l=0;l<TAILLE_BIT_OPERATION;l++) {
    bin[l]=0;
  }
  printf("\nDébut : \n");
  printf("\n----------------------------------\nTest binTabBin\n----------------------------------\n");
  offsetBin=8;
  rempliBinTabBin(binNum,&offsetBin,(int*) &bin);
  afficheBin((int *) &bin);
  printf("\n----------------------------------\nTest Dec2Bin\n----------------------------------\n");
  offsetBin=3; /*Offset de 3*/
  decToBinary(6,&offsetBin,(int*) &bin);
  afficheBin((int *) &bin);
  /*printf("\n----------------------------------\nTest du Parseur\n----------------------------------\n");
  parseFichier(nomFichier);*/

  printf("\n----------------------------------\nTest Remplissage Structure\n----------------------------------\n");
  remplissageStructInstruction(instructions,listeope);
  afficheStructInstruction(instructions);

  printf("\n----------------------------------\nTest recheche Structure\n----------------------------------\n");
  printf("On cherche %s\n", nom);
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }
  nom="JUL";
  printf("On cherche %s\n", nom);
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }
  nom="JAL";
  printf("On cherche %s\n", nom);
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }
  nom="SYSCALL";
  printf("On cherche %s\n", nom);
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }
  nom="SLL";
  printf("On cherche %s\n", nom);
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }

  printf("\n----------------------------------\nTest parseur ligne\n----------------------------------\n");
  char *ligne="SLL $10,$23,24";/*"ADD $6,$31,$1";*/
  printf("%s\n", ligne);
  parseLigne(ligne);
  return 0;
}
