#include "calcul.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../module_tools/tools.h"

int validHex(char* hex) {
  int len=strlen(hex);
  int valid=0,i=0,ret=-1;
  for (i=0;i<len;i++) {
    if ((hex[i]>='0' && hex[i]<='9') || (hex[i]>='a' && hex[i]<='f') || (hex[i]>='A' && hex[i]<='F'))  {
      valid++;
    }
  }
  if (valid==len) {
    ret=1;
  }
  else {
    ret=0;
  }
  return ret;
}

int checkBinVal(int* bin, int* offset, char* valeur) {
  int i=0,ret=1;
  while(ret && valeur[i]!='\0') {
    if (valeurDecimale(&valeur[i])!=bin[i]) {
      ret=0;
    }
    i++;
  }
  return ret;
}

/* Prend en entrée une instruction hexadécimale (demandé dans les specifications) */
void traduitHex(char* hex) {
  int len=strlen(hex);
  int i=0,offset=0;
  int bin[TAILLE_BIT_OPERATION];
  instruction* instructions[NB_OPERATIONS+1];
  instruction* found;
  char *listeope="src/listeOpe.txt";

  remplissageStructInstruction(instructions,listeope);

  if (len==8 && validHex(hex)) {
    hexToBin(hex,bin);
    afficheBin(bin,TAILLE_BIT_OPERATION);
    if (checkBinVal(bin,&offset,"000000")) {
      printf("instruction de type R\n");
      printf("Opcode : ");
      /* Opcode à partir de la case 26 */
      afficheBin(&bin[26],6);
      if ((found=trouveOpcode(instructions, &bin[26]))!=NULL) {
        /* Opération trouvé */
        printf("Traduction %s\n", found->nom);

      }
    }
    else {
      printf("Opcode : ");
      afficheBin(bin,6);
      if ((found=trouveOpcode(instructions, bin))!=NULL) {
        /* Opération trouvé */
        printf("Traduction %s\n", found->nom);
      }
    }
  }
  else {
    printf("Format de l'instruction incorrect %s\n", hex);
  }
  printf("\n");
}
