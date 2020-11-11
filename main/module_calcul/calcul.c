#include "calcul.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../module_tools/tools.h"
#include "../module_registres/registres.h"

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

int checkBinVal(int* bin, int offset, char* valeur) {
  int i=offset,ret=1,j=0;
  while(ret && valeur[j]!='\0') {
    if ((valeur[j++]-'0')!=bin[i]) {
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
  int ope=0;
  int bin[TAILLE_BIT_OPERATION];
  int* opeHex=NULL;
  instruction* instructions[NB_OPERATIONS+1];
  instruction* found;
  char *listeope="src/listeOpe.txt";
  registre* registres[NB_REGISTRE];
  registre* foundReg=NULL;
  char *listereg="src/listeReg.txt";
  remplissageStructRegistre(registres,listereg);
  remplissageStructInstruction(instructions,listeope);
  if (len==8 && validHex(hex)) {
    hexToBin(hex,bin);
    afficheBin(bin,TAILLE_BIT_OPERATION);
    /* PAS OPTI DUTOUT */
    if (checkBinVal(bin,0,"00000000000000000000000000000000")) {
      printf("NOP\n");
    }
    if (checkBinVal(bin,0,"000000")) {
      printf("instruction de type R\n");
      printf("Opcode : ");
      /* Opcode à partir de la case 26 */
      afficheBin(&bin[26],6);
      /*if (checkBinVal(bin,26,"000010")) {
        printf("Cas particulier\n");
      }*/
      offset+=6;
      if ((found=trouveOpcode(instructions, &bin[26], 'R'))!=NULL) {
        /* Instruction de type R */
        if (found->typeInstruction=='R') {
          if (found->ordreBits==1) {
            /* On a 4 champs de 5bits */
            if((opeHex=calloc(4,sizeof(int)))==NULL){exit(1);};
            for (ope=0;ope<4;ope++) {
              opeHex[ope]=binToDec(&bin[offset],5);
              offset+=5;
            }
            for (ope=0;ope<4;ope++) {
              printf("Ope R1 : %d\n", opeHex[ope]);
            }
            /* ADD/AND/XOR/OR/SLT/SUB */
            if (found->styleRemplissage==1) {

            }
          }
          else if (found->ordreBits==2) {
            /* Particulier : ROTR SLR */
            /* On a 1 champ de 4bits, 1 de 1bit puis 3 de 5bits */
            if((opeHex=calloc(5,sizeof(int)))==NULL){exit(1);};
            ope=0;
            opeHex[ope++]=binToDec(&bin[offset],4);
            offset+=4;
            opeHex[ope++]=binToDec(&bin[offset],1);
            offset+=1;
            for (ope;ope<5;ope++) {
              opeHex[ope]=binToDec(&bin[offset],5);
              offset+=5;
            }
            /* On selectionne la bonne structure mémoire (utile pour la suite) */
            if (opeHex[1]==0) {
              found=trouveOperation(instructions,"SRL");
            }
            else if (opeHex[1]==1) {
              found=trouveOperation(instructions,"ROTR");
            }
            for (ope=0;ope<5;ope++) {
              printf("Ope R2 : %d\n", opeHex[ope]);
            }
          }
          else if (found->ordreBits==3) {
            /* 2 champs de 5 bits, 1 de 10bits */
            if((opeHex=calloc(3,sizeof(int)))==NULL){exit(1);};
            ope=0;
            opeHex[ope++]=binToDec(&bin[offset],5);
            offset+=5;
            opeHex[ope++]=binToDec(&bin[offset],5);
            offset+=5;
            opeHex[ope++]=binToDec(&bin[offset],10);
            offset+=10;
            for (ope=0;ope<3;ope++) {
              printf("Ope R3 : %d\n", opeHex[ope]);
            }
          }
          else if (found->ordreBits==4) {
            /* 1 de 5bits 1 de bits 1 de 5bits */
            if((opeHex=calloc(3,sizeof(int)))==NULL){exit(1);};
            ope=0;
            opeHex[ope++]=binToDec(&bin[offset],5);
            offset+=5;
            opeHex[ope++]=binToDec(&bin[offset],10);
            offset+=10;
            opeHex[ope++]=binToDec(&bin[offset],5);
            offset+=5;
            for (ope=0;ope<3;ope++) {
              printf("Ope R4 : %d\n", opeHex[ope]);
            }
          }
          else if (found->ordreBits==5) {
            /* 1 de 10 2 de 5 */
            if((opeHex=calloc(3,sizeof(int)))==NULL){exit(1);};
            ope=0;
            opeHex[ope++]=binToDec(&bin[offset],10);
            offset+=10;
            opeHex[ope++]=binToDec(&bin[offset],5);
            offset+=5;
            opeHex[ope++]=binToDec(&bin[offset],5);
            offset+=5;
            for (ope=0;ope<3;ope++) {
              printf("Ope R5 : %d\n", opeHex[ope]);
            }
          }
          else if (found->ordreBits==6) {
            /* 1 de 20 */
            if((opeHex=calloc(1,sizeof(int)))==NULL){exit(1);};
            ope=0;
            opeHex[ope++]=binToDec(&bin[offset],20);
            offset+=20;
            for (ope=0;ope<1;ope++) {
              printf("Ope R6 : %d\n", opeHex[ope]);
            }
          }
        }
        /* Opération trouvé */
        printf("Traduction %s\n", found->nom);
        free(opeHex);

      }
    }
    else {
      printf("Opcode : ");
      afficheBin(bin,6);
      if ((found=trouveOpcode(instructions, bin, 'I'))!=NULL) {
        /* Instruction de type I */
        if (found->typeInstruction=='I') {
          if (found->ordreBits==1) {
            /* 2*5bits puis 16bits */
            if((opeHex=calloc(3,sizeof(int)))==NULL){exit(1);};
            ope=0;
            opeHex[ope++]=binToDec(&bin[offset],5);
            offset+=5;
            opeHex[ope++]=binToDec(&bin[offset],5);
            offset+=5;
            opeHex[ope++]=binToDec(&bin[offset],16);
            offset+=16;
            for (ope=0;ope<3;ope++) {
              printf("Ope R5 : %d\n", opeHex[ope]);
            }
          }
        }
        /* Opération trouvé */
        printf("Traduction %s\n", found->nom);
        free(opeHex);
      }
    }
  }
  else {
    printf("Format de l'instruction incorrect %s\n", hex);
  }
  printf("\n");
}
