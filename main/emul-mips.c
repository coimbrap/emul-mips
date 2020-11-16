#include "module_hex/hex.h"
#include "module_registres/registres.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *file;
  instruction* instructions[NB_OPERATIONS+1];
  char *listeope="src/listeOpe.txt";
  registre* registres[NB_REGISTRE];
  char *listereg="src/listeReg.txt";
  int in=1,out=2,mode=1;

  remplissageStructRegistre(registres,listereg);
  remplissageStructInstruction(instructions,listeope);
  printf("---------- MIPS Emulator ----------\n  Pierre Coimbra & Thibaut Barnel\n\n");
  if(argc<3) {
    printf("Mauvaise utilisation : ./emul-mips asm.txt hex.txt\n./emul-mips -pas hex.txt\n");
    printf("Autre option possible : \nTest parseur : make test\n");
  }
  /* Mode pas à pas */
  if ((file=fopen(argv[in], "r"))) {
    fclose(file);
    printf("Lecture du ficher : %s\n\n", argv[in]);
    parseFichier(argv[in],argv[out],mode);
    printf("\nFormes hexadécimale écrites dans '%s'\n", argv[out]);
  }
  else {
    printf("Erreur sur le fichier d'entrée '%s'\n\n",argv[in]);
  }
  return 0;
}
