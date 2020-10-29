#include "module_hex/hex.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *file;
  printf("---------- MIPS Emulator ----------\n  Pierre Coimbra & Thibaut Barnel\n\n");
  if(argc!=3) {
    printf("Mauvaise utilisation : ./emul-mips asm.txt hex.txt\n\n");
    printf("Autres options possible : \nTest parseur : make test\nTests des fonctions : make debug\n");
  }
  else if ((file=fopen(argv[1], "r"))) {
    fclose(file);
    printf("Lecture du ficher : %s\n\n", argv[1]);
    parseFichier(argv[1],argv[2]);
    printf("\nFormes hexadécimale écrites dans '%s'\n", argv[2]);
  }
  else {
    printf("Le fichier d'entrée '%s' est introuvable\n\n",argv[1]);
  }
  return 0;
}
