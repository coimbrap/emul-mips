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
  if (valid==len) {ret=1;}
  else {ret=0;}
  return ret;
}

int checkBinVal(int* bin, int offset, char* valeur) {
  int i=offset,ret=1,j=0;
  while(ret && valeur[j]!='\0') {
    if ((valeur[j++]-'0')!=bin[i]) {ret=0;}
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
  registre *hi=NULL;
  registre *lo=NULL;

  int intOpcode=0;
  long int value=0;
  long int tmp=0;


  if (len<=8 && validHex(hex)) {
    hexToBin(hex,bin);
    afficheBin(bin,TAILLE_BIT_OPERATION);
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
          intOpcode=valeurDecimale(found->opcode);
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
                  value=rs->valeur+rt->valeur;
                  if (value<=0xffffffff) {
                    rd->valeur=value;
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
                  value=rs->valeur&rt->valeur;
                  rd->valeur=value;
                  #ifdef DEBUG
                  afficheRegistre(rd);
                  #endif
                }
                /* XOR */
                else if (intOpcode==100110) {
                  value=rs->valeur^rt->valeur;
                  rd->valeur=value;
                  #ifdef DEBUG
                  afficheRegistre(rd);
                  #endif
                }
                /* OR */
                else if (intOpcode==100101) {
                  value=rs->valeur|rt->valeur;
                  rd->valeur=value;
                  #ifdef DEBUG
                  afficheRegistre(rd);
                  #endif
                }
                /* SLT */
                else if (intOpcode==101010) {
                  if ((value=rs->valeur<rt->valeur)) {
                    rd->valeur=1;
                  }
                  else {
                    rd->valeur=0;
                  }
                }
                /* SUB */
                else if (intOpcode==100010) {
                  value=rs->valeur-rt->valeur;
                  rd->valeur=value;
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
              value=rt->valeur>>opeHex[3];
              rd->valeur=value;
              #ifdef DEBUG
              afficheRegistre(rd);
              #endif
            }
          }
          else if (found->ordreBits==2) {
            /* ROTR/SLR */
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
            rt=trouveRegistre(registres,intVersChaine(opeHex[2]));
            rd=trouveRegistre(registres,intVersChaine(opeHex[3]));
            /* On selectionne la bonne structure mémoire (utile pour la suite) */
            /* SRL */
            if (opeHex[1]==0) {
              found=trouveOperation(instructions,"SRL");
              value=rt->valeur;
              value=value>>opeHex[4];
              tmp=(1<<(NB_BIT_REGISTRE-opeHex[4]))+-1;
              printf("ROTR : %ld|%ld\n", value,tmp);

              value=value&tmp;
              rd->valeur=value;
              #ifdef DEBUG
              afficheRegistre(rt);
              afficheRegistre(rd);
              #endif
            }
            /* ROTR */
            else if (opeHex[1]==1) {
              found=trouveOperation(instructions,"ROTR");
              value=rt->valeur;
              tmp=value;
              value=value>>opeHex[4];
              tmp=tmp<<((NB_BIT_REGISTRE)-opeHex[4]);
              value=value|tmp;
              rd->valeur=value;
              #ifdef DEBUG
              afficheRegistre(rt);
              afficheRegistre(rd);
              #endif
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
            rs=trouveRegistre(registres,intVersChaine(opeHex[0]));
            rt=trouveRegistre(registres,intVersChaine(opeHex[1]));
            hi=trouveRegistre(registres,"HI");
            lo=trouveRegistre(registres,"LO");
            /* MULT/DIV */
            if (found->styleRemplissage==1) {
              /* MULT */
              if (intOpcode==11000) {
                value=rs->valeur*rt->valeur;
                hi->valeur=((value&0xffffffff00000000)>>32);
                lo->valeur=(value&0xffffffff);
                #ifdef DEBUG
                afficheRegistre(rs);
                afficheRegistre(rt);
                afficheRegistre(hi);
                afficheRegistre(lo);
                #endif
              }
              /* SUB */
              else if (intOpcode==11010) {
                value=rs->valeur-rt->valeur;
                rd->valeur=value;
                #ifdef DEBUG
                afficheRegistre(rd);
                #endif
              }
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
            afficheBin(&bin[offset-16],16);
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
                value=rs->valeur+imm;
                if (value<=0xffffffff) {
                  rt->valeur=value;
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
}


int chargeProgramme(memoire *mem, const char* progHex) {
  FILE *prog=NULL;
  char* instruction=NULL;
  size_t len=0;
  int pc=INIT_PC;

  prog=fopen(progHex,"r");
  if(NULL==prog){
    printf("Erreur d'ouverture du fichier\n");
    exit(-1);
  }
  while (getline(&instruction,&len,prog)!=-1) {
    insertion(pc,hexToDec(instruction),mem);
    pc+=4;
    printf("Insert %ld|%d\n",hexToDec(instruction),pc);
  }
  free(instruction);
  fclose(prog);
  return (pc-4);
}

void execProgramme(memoire *mem, registre** registres, instruction** instructions, char* prog) {
  long int instruction=0;
  registre *PC=trouveRegistre(registres,"PC");
  int pcMax=0,pc=INIT_PC;
  pcMax=chargeProgramme(mem,prog);
  while(pc<=pcMax) {
    instruction=valeurMemoire(pc,mem);
    printf("A : %ld|%s\n", instruction,decToHex(instruction));
    traduitHex(decToHex(instruction),registres,instructions,mem,&pc); /* S'occupe d'incrémenter le PC */
    PC->valeur=pc;
  }
}
