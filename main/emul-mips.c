#include "module_console/console.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *file;
  instruction* instructions[NB_OPERATIONS+1];
  char *listeope="src/listeOpe.txt";
  registre* registres[NB_REGISTRE];
  memoire mem=NULL;
  symtable symbols=NULL;
  char *listereg="src/listeReg.txt";
  prog segments=NULL;
  remplissageStructRegistre(registres,listereg);
  remplissageStructInstruction(instructions,listeope);
  printf("---------- MIPS Emulator ----------\n  Pierre Coimbra & Thibaut Barnel\n\n");
  if(argc<3) {
    printf("Mauvaise utilisation : ./emul-mips asm.txt hex.txt\n./emul-mips -pas hex.txt\n");
  }
  if (argc==4 && strcmp(argv[1],"-pas")==0) {
    printf("Mode pas à pas non-intéractif\n\n");
    parseFichier(argv[2],argv[3],0,instructions,registres,&symbols,&mem,&segments);
  }
  else if(argc==3 && (strcmp(argv[1],"-pas")==0)) {
    printf("Mode intéractif\n\n");
    parseFichier("/dev/null",argv[2],2,instructions,registres,&symbols,&mem,&segments);
  }
  else if ((file=fopen(argv[1], "r"))) {
    fclose(file);
    printf("Mode automatique non-intéractif\n\n");
    parseFichier(argv[1],argv[2],1,instructions,registres,&symbols,&mem,&segments);
  }
  else {
    printf("Erreur sur le fichier d'entrée\n");
  }
  liberationRegistres(registres);
  liberationInstruction(instructions);
  liberation(&mem);
  libereTable(&symbols);
  return 0;
}
