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
  printf("---------- MIPS Emulator ----------\n Thibaut Barnel & Pierre Coimbra\n\n");
  if(argc<3) {
    printf("Mauvaise utilisation :\nAutomatique : ./emul-mips asm.txt hex.txt\nPas à pas   : ./emul-mips -pas asm.txt hex.txt\nIntéractif  : ./emul-mips -pas hex.txt\n");
  }
  else if (argc==4 && strcmp(argv[1],"-pas")==0) {
    printf("Mode pas à pas non-intéractif\n\n");
    parseFichier(argv[2],argv[3],0,instructions,registres,&mem,&segments,&symbols);
  }
  else if(argc==3 && (strcmp(argv[1],"-pas")==0)) {
    printf("Mode intéractif\n\n");
    parseFichier("/dev/null",argv[2],2,instructions,registres,&mem,&segments,&symbols);
  }
  else if ((file=fopen(argv[1], "r"))) {
    fclose(file);
    printf("Mode automatique non-intéractif\n\n");
    parseFichier(argv[1],argv[2],1,instructions,registres,&mem,&segments,&symbols);
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
