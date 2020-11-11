#include "module_hex/hex.h"
#include "module_calcul/calcul.h"
#include "module_registres/registres.h"
#include "module_memoire/memoire.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *file;
  instruction* instructions[NB_OPERATIONS+1];
  instruction* found;
  char *listeope="src/listeOpe.txt";
  registre* registres[NB_REGISTRE];
  registre* foundReg=NULL;
  memoire mem=NULL;
  char *listereg="src/listeReg.txt";
  remplissageStructRegistre(registres,listereg);
  remplissageStructInstruction(instructions,listeope);

  printf("---------- MIPS Emulator ----------\n  Pierre Coimbra & Thibaut Barnel\n\n");
  if(argc!=3) {
    printf("Mauvaise utilisation : ./emul-mips asm.txt hex.txt\n\n");
    printf("Autre option possible : \nTest parseur : make test\n");
  }
  else if ((file=fopen(argv[1], "r"))) {
    fclose(file);
    printf("Lecture du ficher : %s\n\n", argv[1]);
    parseFichier(argv[1],argv[2]);
    printf("\nFormes hexadécimale écrites dans '%s'\n", argv[2]);
    printf("\n------ Registres ------\n");
    afficheRegistres(registres);
    printf("\n------- Mémoire -------\n");
    insertion(0x4,decToBin(2220,NB_BIT_MEMOIRE),&mem);
    insertion(0x0,decToBin(11202,NB_BIT_MEMOIRE),&mem);
    insertion(0xffec,decToBin(4203,NB_BIT_MEMOIRE),&mem);
    insertion(0xfffc,decToBin(21003,NB_BIT_MEMOIRE),&mem);
    insertion(0xfff8,decToBin(4294967295,NB_BIT_MEMOIRE),&mem);
    insertion(0xfff4,decToBin(30001,NB_BIT_MEMOIRE),&mem);
    insertion(0x101c,decToBin(20140,NB_BIT_MEMOIRE),&mem);
    afficherMemoire(&mem);
  }
  else {
    printf("Le fichier d'entrée '%s' est introuvable\n\n",argv[1]);
  }
  return 0;
}
