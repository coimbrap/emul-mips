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
void traduitHex(long int hex, registre** registres, instruction** instructions, memoire *mem) {
  instruction *found=NULL;
  registre *rs=NULL;
  registre *rt=NULL;
  registre *rd=NULL;
  registre *hi=NULL;
  registre *lo=NULL;
  registre *pc=NULL;
  int rsI=0,rtI=0,rdI=0,sa=0,imm=0,opcode=0;
  long int value=0;
  long int tmp=0;
  if (hex<=0xFFFFFFFF) {
    pc=trouveRegistre(registres,"PC");
    /* NOP */
    if (hex==0) {
      pc->valeur+=4;
    }
    /* Si les 6 premiers zéro sont nul */
    else if ((hex&(0xfc000000))==0) { /* Masque : 1111110...0 */
      printf("R\n");

      opcode=hex&0x3f;
      if ((found=trouveOpcode(instructions,opcode,'R'))!=NULL) {
        printf("On vérifie R : %d|%d|%d|%d\n", found->ordreBits,found->styleRemplissage,found->opcode,opcode);

        /* Instruction de type R */
        /* On trouve les valeurs de registres, c'est commun à tous */
        rsI=((hex>>21) & 0x1F); /* Valeur décimale des 21 à 26 bits */
        rtI=((hex>>16) & 0x1F);
        rdI=((hex>>11) & 0x1F);
        sa=((hex>>6) & 0x1F);
        if (found->typeInstruction=='R') {
          value=0;
          rs=trouveRegistre(registres,intVersChaine(rsI));
          rt=trouveRegistre(registres,intVersChaine(rtI));
          rd=trouveRegistre(registres,intVersChaine(rdI));
          if (found->ordreBits==1) {
            /* ADD/AND/XOR/OR/SLT/SUB */
            if (found->styleRemplissage==1) {
              if (rs!=NULL && rt!=NULL && rd!=NULL) {
                /* ADD */
                if (opcode==0x20) {
                  value=rs->valeur+rt->valeur;
                  if (value<=0xffffffff) {
                    printf("ADD %d|%d %d|%d %d|%d \n",rdI,rd->valeur,rtI,rt->valeur,rsI,rs->valeur);
                    rd->valeur=value;
                  }
                  else {
                    printf("Exception : Overflow\nNo changes\n");
                  }
                }
                /* AND */
                else if (opcode==0x24) {
                  rd->valeur=(rs->valeur&rt->valeur);
                }
                /* XOR */
                else if (opcode==0x26) {
                  rd->valeur=(rs->valeur^rt->valeur);
                }
                /* OR */
                else if (opcode==0x25) {
                  rd->valeur=(rs->valeur|rt->valeur);
                }
                /* SLT */
                else if (opcode==0x2A) {
                  if ((value=(int)rs->valeur<(int)rt->valeur)) {rd->valeur=1;}
                  else {rd->valeur=0;}
                }
                /* SUB */
                else if (opcode==0x22) {
                  rd->valeur=(rs->valeur-rt->valeur);
                }
                pc->valeur+=4;
              }
            }
            /*NOP déjà fait*/
            /* SLL */
            else if (found->styleRemplissage==3) {
              if (rt!=NULL && rd!=NULL) {
                rd->valeur=(rt->valeur>>sa);
              }
              pc->valeur+=4;
            }
          }
          else if (found->ordreBits==2) {
            /* ROTR/SLR */
            if (rt!=NULL && rd!=NULL) {
              /* ROTR */
              if ((hex>>21)&0x1) {
                found=trouveOperation(instructions,"ROTR");
                value=rt->valeur;
                tmp=value;
                value=value>>sa;
                tmp=tmp<<((NB_BIT_REGISTRE)-sa);
                rd->valeur=(value|tmp);
              }
              /* SRL */
              else {
                found=trouveOperation(instructions,"SRL");
                rd->valeur=((rt->valeur)>>sa);
              }
            }
            pc->valeur+=4;
          }
          else if (found->ordreBits==3) {
            hi=trouveRegistre(registres,"HI");
            lo=trouveRegistre(registres,"LO");
            if (rs!=NULL && rt!=NULL && hi!=NULL && lo!=NULL) {
              /* MULT */
              if (opcode==0x18) {
                value=(rs->valeur*rt->valeur);
              }
              /* DIV */
              else if (opcode==0x1A) {
                value=(rs->valeur/rt->valeur);
              }
              hi->valeur=(((value&0xffffffff00000000)>>32)&MASQUE_MAX);
              lo->valeur=((value&0xffffffff)&MASQUE_MAX);
            }
            pc->valeur+=4;
          }
          else if (found->ordreBits==4) {
            if (rs!=NULL && pc!=NULL) {
              if (opcode==0x08) {
                rs->valeur+=INIT_PC; /* On se place au bon endroit */
                if (rs->valeur>=INIT_PC) {
                  pc->valeur=rs->valeur;
                }
                else {
                  printf("Erreur - JR : Valeur du PC trop faible\n");
                }
                pc->valeur+=4;
              }
            }
          }
          else if (found->ordreBits==5) {
            hi=trouveRegistre(registres,"HI");
            lo=trouveRegistre(registres,"LO");
            if (rd!=NULL && hi!=NULL && lo!=NULL) {
              /* MFHI */
              if (opcode==0x10) {
                rd->valeur=hi->valeur;
              }
              /* MFLO */
              else if (opcode==0x12) {
                rd->valeur=lo->valeur;
              }
            }
            pc->valeur+=4;
          }
          else if (found->ordreBits==6) {
            pc->valeur+=4;
            /* Pas utilisé dans notre cas */
          }
        }
        if (rd!=NULL) {rd->valeur&=MASQUE_MAX;}
      }
    }
    else {
      opcode=((hex>>26)&0x3f);
      if ((found=trouveOpcode(instructions,opcode,'I'))!=NULL) {
        /* Instruction de type I */
        /* On trouve les valeurs de registres, c'est commun à tous */
        rsI=((hex>>21) & 0x1F);
        rtI=((hex>>16) & 0x1F);
        imm=(hex & 0xFFFF);
        /* On prend le complément à 2 d'un 16 bits car signé */
        imm=complementInt(imm,16); /* Valeur immédiate € [-32768,32767] */
        if (found->typeInstruction=='I') {
          value=0;
          rs=trouveRegistre(registres,intVersChaine(rsI));
          rt=trouveRegistre(registres,intVersChaine(rtI));
          if (found->ordreBits==1) {
            if (found->styleRemplissage==1) {
              if (rs!=NULL && rt!=NULL) {
                /* ADDI */
                if (opcode==0x8) {
                  value=rs->valeur+imm;
                  if (value<=0xFFFFFFFF) {
                    rt->valeur=value;
                  }
                  else {
                    printf("Exception : Overflow\nNo changes\n");
                  }
                  pc->valeur+=4;
                }
                /* BEQ */
                if (opcode==0x4) {
                  if (rs->valeur==rt->valeur) {
                    pc->valeur+=(imm<<2);
                  }
                  pc->valeur+=4;
                }
                /* BNE */
                if (opcode==0x5) {
                  if (rs->valeur!=rt->valeur) {
                    pc->valeur+=(imm<<2);
                  }
                  pc->valeur+=4;
                }
              }
            }
            if (found->styleRemplissage==2) {
              if (rs!=NULL) {
                /* BGTZ */
                if (opcode==0x7) {
                  if (rs->valeur>0) {
                    pc->valeur+=(imm<<2);
                  }
                  pc->valeur+=4;
                }
                /* BLEZ */
                if (opcode==0x6) {
                  if (rs->valeur<=0) {
                    pc->valeur+=(imm<<2);
                  }
                  pc->valeur+=4;
                }
              }
            }
            if (found->styleRemplissage==3) {
              if (rt!=NULL) {
                /* LUI */
                if (opcode==0xf) {
                  rt->valeur=(imm<<16);
                }
              }
              pc->valeur+=4;
            }
            if (found->styleRemplissage==4) {
              if (rs!=NULL && rt!=NULL) {
                /* LW */
                if (opcode==0x23) {
                  rt->valeur=valeurMemoire((rs->valeur+imm),mem);
                }
                /* SW */
                if (opcode==0x2b) {
                  insertion((rs->valeur+imm),rt->valeur,mem);
                }
              }
              pc->valeur+=4;
            }
          }
        }
      }
    }
  }
  else {
    printf("Format de l'instruction incorrect 0x%lx\n", hex);
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
  }
  free(instruction);
  fclose(prog);
  return (pc-4);
}

void execProgramme(memoire *mem, registre** registres, instruction** instructions, char* prog) {
  long int instruction=0;
  registre *pc=NULL;
  int pcMax=0;
  pc=trouveRegistre(registres,"PC");
  (pc->valeur)=INIT_PC;

  pcMax=chargeProgramme(mem,prog);
  while((pc->valeur)<=pcMax) {
    instruction=valeurMemoire(pc->valeur,mem);
    traduitHex(instruction,registres,instructions,mem); /* S'occupe d'incrémenter le PC */
  }
}
