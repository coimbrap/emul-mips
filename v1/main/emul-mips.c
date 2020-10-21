#include "module_hex/hex.h"

int main() {
  int l=0;
  char nomFichier[]="tests_emul/test.txt";
  int bin[TAILLE_BIT_OPERATION];
  for (l=0;l<TAILLE_BIT_OPERATION;l++) {
    bin[l]=0;
  }
  printf("\nDébut : \n");
  decToBinary(26,3,(int*) &bin);
  afficheBin((int *) &bin);
  parseFichier((char *) &nomFichier);
  return 0;
}
