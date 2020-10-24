#include "module_hex/hex.h"

int main() {
  int l=0;
  char *nomFichier="tests_emul/test.txt";
  char *binNum="010101001";
  char* listeope="module_hex/listeOpe.txt";
  instruction *instructions[NB_OPERATIONS+1];
  
  int bin[TAILLE_BIT_OPERATION];
  for (l=0;l<TAILLE_BIT_OPERATION;l++) {
    bin[l]=0;
  }
  printf("\nDébut : \n");
  printf("\n----------------------------------\nTest binTabBin\n----------------------------------\n");
  rempliBinTabBin(binNum,7,(int*) &bin);
  afficheBin((int *) &bin);
  printf("\n----------------------------------\nTest Dec2Bin\n----------------------------------\n");
  decToBinary(26,3,(int*) &bin);
  afficheBin((int *) &bin);
  printf("\n----------------------------------\nTest du Parseur\n----------------------------------\n");

  parseFichier(nomFichier);
  printf("\n----------------------------------\nTest Remplissage Structure\n----------------------------------\n");

  remplissageStructInstruction(instructions,listeope);
  afficheStructInstruction(instructions);
  return 0;
}
