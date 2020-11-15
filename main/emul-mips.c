#include "module_hex/hex.h"
#include "module_calcul/calcul.h"
#include "module_registres/registres.h"
#include "module_memoire/memoire.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *file;
  instruction* instructions[NB_OPERATIONS+1];
  char *listeope="src/listeOpe.txt";
  registre* registres[NB_REGISTRE];
  memoire mem=NULL;
  char *listereg="src/listeReg.txt";
  remplissageStructRegistre(registres,listereg);
  remplissageStructInstruction(instructions,listeope);
  int in=1,out=2,mode=1;
  printf("---------- MIPS Emulator ----------\n  Pierre Coimbra & Thibaut Barnel\n\n");
  if(argc<3) {
    printf("Mauvaise utilisation : ./emul-mips asm.txt hex.txt\n./emul-mips -pas hex.txt\n");
    printf("Autre option possible : \nTest parseur : make test\n");
  }
  /* Mode pas à pas */
  if (strcmp(argv[1],"-pas")==0) {
    in=2;
    out=3;
    mode=0;
    printf("Mode pas à pas\n\n");
  }
  if ((file=fopen(argv[in], "r"))) {
    fclose(file);
    printf("Lecture du ficher : %s\n\n", argv[in]);
    parseFichier(argv[in],argv[out],mode);
    printf("\nFormes hexadécimale écrites dans '%s'\n", argv[out]);
    /*  execProgramme(&mem,registres,instructions,argv[2]); */
    printf("\n------ Registres ------\n");
    afficheRegistres(registres);
    printf("\n------- Mémoire -------\n");
    afficherMemoires(&mem);
  }
  else {
    printf("Erreur sur le fichier d'entrée '%s'\n\n",argv[in]);
  }
  return 0;
}
