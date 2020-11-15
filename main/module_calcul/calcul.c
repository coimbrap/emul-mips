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
void traduitHex(char* hex, registre** registres, instruction** instructions, memoire *mem, int* pc) {
  int len=strlen(hex);
  int offset=0;
  int ope=0;
  int bin[TAILLE_BIT_OPERATION];
  int* opeHex=NULL;
  instruction *found=NULL;
  int imm=0;
  registre *rs=NULL;
  registre *rt=NULL;
  registre *rd=NULL;
  int intOpcode=0;
  int value=0;

  if (len==8 && validHex(hex)) {
    hexToBin(hex,bin);
    #ifdef DEBUG
    afficheBin(bin,TAILLE_BIT_OPERATION);
    #endif
    /* PAS OPTI DUTOUT */
    if (strcmp(hex,"00000000")==0) {
      #ifdef DEBUG
      printf("NOPP\n");
      #endif
      *pc+=4;
    }
    else if (checkBinVal(bin,0,"000000")) {
      #ifdef DEBUG
      printf("instruction de type R\n");
      printf("Opcode : ");
      /* Opcode à partir de la case 26 */
      afficheBin(&bin[26],6);
      #endif
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
            #ifdef DEBUG
            for (ope=0;ope<4;ope++) {
              printf("Ope R1 : %d\n", opeHex[ope]);
            }
            #endif
            intOpcode=valeurDecimale(found->opcode);
            value=0;
            /* ADD/AND/XOR/OR/SLT/SUB */
            if (found->styleRemplissage==1) {
              rs=trouveRegistre(registres,intVersChaine(opeHex[0]));
              rt=trouveRegistre(registres,intVersChaine(opeHex[1]));
              rd=trouveRegistre(registres,intVersChaine(opeHex[2]));
              #ifdef DEBUG
              afficheRegistre(rs);
              afficheRegistre(rt);
              afficheRegistre(rd);
              #endif
              if (rs!=NULL && rt!=NULL && rd!=NULL) {
                /* ADD */
                if (intOpcode==100000) {
                  value=decValue(rs->valeur,NB_BIT_REGISTRE)+decValue(rt->valeur,NB_BIT_REGISTRE);
                  if (value<=0xffffffff) {
                    changeRegistre(rd,decToBin(value,NB_BIT_REGISTRE));
                    #ifdef DEBUG
                    afficheRegistre(rd);
                    #endif
                  }
                  else {
                    printf("Exception : Overflow\nNo changes\n");
                  }
                }
                /* AND */
                else if (intOpcode==100100) {
                  value=decValue(rs->valeur,NB_BIT_REGISTRE)&decValue(rt->valeur,NB_BIT_REGISTRE);
                  changeRegistre(rd,decToBin(value,NB_BIT_REGISTRE));
                  #ifdef DEBUG
                  afficheRegistre(rd);
                  #endif
                }
                /* XOR */
                else if (intOpcode==100110) {
                  value=decValue(rs->valeur,NB_BIT_REGISTRE)^decValue(rt->valeur,NB_BIT_REGISTRE);
                  changeRegistre(rd,decToBin(value,NB_BIT_REGISTRE));
                  #ifdef DEBUG
                  afficheRegistre(rd);
                  #endif
                }
                /* OR */
                else if (intOpcode==100101) {
                  value=decValue(rs->valeur,NB_BIT_REGISTRE)|decValue(rt->valeur,NB_BIT_REGISTRE);
                  changeRegistre(rd,decToBin(value,NB_BIT_REGISTRE));
                  #ifdef DEBUG
                  afficheRegistre(rd);
                  #endif
                }
                else if (intOpcode==101010) {
                  if ((value=decValue(rs->valeur,NB_BIT_REGISTRE)<decValue(rt->valeur,NB_BIT_REGISTRE))) {
                    changeRegistre(rd,decToBin(1,NB_BIT_REGISTRE));
                  }
                  else {
                    changeRegistre(rd,decToBin(0,NB_BIT_REGISTRE));
                  }
                }
                else if (intOpcode==100010) {
                  value=decValue(rs->valeur,NB_BIT_REGISTRE)-decValue(rt->valeur,NB_BIT_REGISTRE);
                  changeRegistre(rd,decToBin(value,NB_BIT_REGISTRE));
                  #ifdef DEBUG
                  afficheRegistre(rd);
                  #endif
                }
              }
            }
            /*NOP déjà fait*/
            /* SLL */
            else if (found->styleRemplissage==3) {
              rt=trouveRegistre(registres,intVersChaine(opeHex[1]));
              rd=trouveRegistre(registres,intVersChaine(opeHex[2]));
              value=decValue(rt->valeur,NB_BIT_REGISTRE)>>opeHex[3];
              changeRegistre(rd,decToBin(value,NB_BIT_REGISTRE));
              #ifdef DEBUG
              afficheRegistre(rd);
              #endif
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
            for (ope=ope;ope<5;ope++) {
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
            #ifdef DEBUG
            for (ope=0;ope<5;ope++) {
              printf("Ope R2 : %d\n", opeHex[ope]);
            }
            #endif
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
            #ifdef DEBUG
            for (ope=0;ope<3;ope++) {
              printf("Ope R3 : %d\n", opeHex[ope]);
            }
            #endif
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
            #ifdef DEBUG
            for (ope=0;ope<3;ope++) {
              printf("Ope R4 : %d\n", opeHex[ope]);
            }
            #endif
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
            #ifdef DEBUG
            for (ope=0;ope<3;ope++) {
              printf("Ope R5 : %d\n", opeHex[ope]);
            }
            #endif
          }
          else if (found->ordreBits==6) {
            /* 1 de 20 */
            if((opeHex=calloc(1,sizeof(int)))==NULL){exit(1);};
            ope=0;
            opeHex[ope++]=binToDec(&bin[offset],20);
            offset+=20;
            #ifdef DEBUG
            for (ope=0;ope<1;ope++) {
              printf("Ope R6 : %d\n", opeHex[ope]);
            }
            #endif
          }
          *pc+=4;
        }
        #ifdef DEBUG
        /* Opération trouvé */
        printf("Traduction %s\n", found->nom);
        #endif
        free(opeHex);

      }
    }
    else {
      #ifdef DEBUG
      printf("Opcode : ");
      afficheBin(bin,6);
      #endif
      if ((found=trouveOpcode(instructions, bin, 'I'))!=NULL) {
        int intOpcode=valeurDecimale(found->opcode);
        int value=0;
        offset=6;
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
            opeHex[ope++]=decValue(&bin[offset],16);
            offset+=16;
            #ifdef DEBUG
            afficheBin(&bin[offset],16);
            printf("LUUT : %d\n",opeHex[2]);
            for (ope=0;ope<3;ope++) {
              printf("Ope R5 : %d\n", opeHex[ope]);
            }
            printf("REMP : %d\n", found->styleRemplissage);
            #endif
            if (found->styleRemplissage==1) {
              rs=trouveRegistre(registres,intVersChaine(opeHex[0]));
              rt=trouveRegistre(registres,intVersChaine(opeHex[1]));
              imm=opeHex[2];
              #ifdef DEBUG
              afficheRegistre(rs);
              afficheRegistre(rt);
              #endif
              /* ADDI */
              if (intOpcode==1000) {
                value=decValue(rs->valeur,NB_BIT_REGISTRE)+imm;
                if (value<=0xffffffff) {
                  changeRegistre(rt,decToBin(value,NB_BIT_REGISTRE));
                  #ifdef DEBUG
                  afficheRegistre(rs);
                  afficheRegistre(rt);
                  #endif
                }
                else {
                  printf("Exception : Overflow\nNo changes\n");
                }
                *pc+=4;
              }
            }
          }
        }
        /* Opération trouvé */
        #ifdef DEBUG
        printf("Traduction %s\n", found->nom);
        #endif
        free(opeHex);
      }
    }
  }
  else {
    printf("Format de l'instruction incorrect %s\n", hex);
  }
  #ifdef DEBUG
  printf("\n");
  #endif
}


int chargeProgramme(memoire *mem, const char* progHex) {
  FILE *prog=NULL;
  char* instruction=NULL;
  int i=0;
  size_t len=0;
  int instructionBin[TAILLE_BIT_OPERATION];
  int pc=INIT_PC;

  prog=fopen(progHex,"r");
  if(NULL==prog){
    printf("Erreur d'ouverture du fichier\n");
    exit(-1);
  }
  while (getline(&instruction,&len,prog)!=-1) {
    hexToBin(instruction,instructionBin);
    insertion(pc,instructionBin,mem);
    pc+=4;
  }
  free(instruction);
  fclose(prog);
  return (pc-4);
}

void execProgramme(memoire *mem, registre** registres, instruction** instructions, char* prog) {
  char hex[TAILLE_HEX_OPERATION];
  int* bin=NULL;
  int pcMax=0,pc=INIT_PC;
  registre *PC=trouveRegistre(registres,"PC");
  pcMax=chargeProgramme(mem,prog);
  while(pc<=pcMax) {
    bin=valeurMemoire(pc,mem);
    #ifdef DEBUG
    afficheBin(bin,32);
    #endif
    binaryToHex(bin,hex,NB_BIT_MEMOIRE);
    traduitHex(hex,registres,instructions,mem,&pc); /* S'occupe d'incrémenter le PC */
    changeRegistre(PC,decToBin(pc,NB_BIT_REGISTRE));
  }
  #ifdef DEBUG
  afficheRegistres(registres);
  #endif
}
