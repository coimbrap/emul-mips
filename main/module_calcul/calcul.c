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
void traduitHex(long int hex, registre** registres, instruction** instructions, memoire *mem, int* pc) {
  instruction *found=NULL;
  int imm=0;
  registre *rs=NULL;
  registre *rt=NULL;
  registre *rd=NULL;
  registre *hi=NULL;
  registre *lo=NULL;
  int rsI=0,rtI=0,rdI=0,sa=0;
  int opcode=0;
  long int value=0;
  long int tmp=0;
  if (hex<=0xFFFFFFFF) {
    /* NOP */
    if (hex==0) {
      #ifdef DEBUG
      printf("NOP\n");
      #endif
      *pc+=4;
    }
    /* Si les 6 premiers zéro sont nul */
    else if ((hex&(0xfc000000))==0) { /* Masque : 1111110...0 */
      printf("R\n");
      opcode=hex&0x3f;
      if ((found=trouveOpcode(instructions,opcode,'R'))!=NULL) {
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
                  if ((value=rs->valeur<rt->valeur)) {rd->valeur=1;}
                  else {rd->valeur=0;}
                }
                /* SUB */
                else if (opcode==0x22) {
                  rd->valeur=(rs->valeur-rt->valeur);
                }
              }
            }
            /*NOP déjà fait*/
            /* SLL */
            else if (found->styleRemplissage==3) {
              if (rt!=NULL && rd!=NULL) {
                rd->valeur=(rt->valeur>>sa);
              }
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
                value=rt->valeur;
                value=value>>sa;
                tmp=(1<<(NB_BIT_REGISTRE-sa))+-1;
                rd->valeur=(value&tmp);
              }
              printf("ROTR 0x%lx\n",rd->valeur);

            }
          }
          else if (found->ordreBits==3) {
            hi=trouveRegistre(registres,"HI");
            lo=trouveRegistre(registres,"LO");
            if (rs!=NULL && rt!=NULL && hi!=NULL && lo!=NULL) {
              /* MULT */
              if (opcode==0x18) {
                value=(rs->valeur*rt->valeur);
                hi->valeur=(((value&0xffffffff00000000)>>32)&MASQUE_MAX);
                lo->valeur=((value&0xffffffff)&MASQUE_MAX);
              }
              /* DIV */
              else if (opcode==0x1A) {
                value=rs->valeur/rt->valeur;
                rd->valeur=value;
              }
            }
          }
          else if (found->ordreBits==4) {

          }
          else if (found->ordreBits==5) {

          }
          else if (found->ordreBits==6) {

          }
          *pc+=4;
        }
        if (rd!=NULL) {rd->valeur&=MASQUE_MAX;}

        #ifdef DEBUG
        printf("Traduction %s\n", found->nom);
        #endif
      }
    }
    else {
      opcode=((hex>>26)&0x3f);

      if ((found=trouveOpcode(instructions,opcode,'I'))!=NULL) {
        printf("Type I %x\n",opcode);

        /* Instruction de type I */
        /* On trouve les valeurs de registres, c'est commun à tous */
        rsI=((hex>>21) & 0x1F);
        rtI=((hex>>16) & 0x1F);
        imm=(hex & 0xFFFF);
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
                  *pc+=4;
                }
              }
            }
          }
        }
        /* Opération trouvé */
        #ifdef DEBUG
        printf("Traduction %s\n", found->nom);
        #endif
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
  registre *PC=trouveRegistre(registres,"PC");
  int pcMax=0,pc=INIT_PC;
  pcMax=chargeProgramme(mem,prog);
  while(pc<=pcMax) {
    instruction=valeurMemoire(pc,mem);
    printf("0x%lx\n", instruction);
    traduitHex(instruction,registres,instructions,mem,&pc); /* S'occupe d'incrémenter le PC */
    PC->valeur=pc;
  }
}
