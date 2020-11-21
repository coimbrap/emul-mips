#include "calcul.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../module_tools/tools.h"

/* prend en entrée un pointeur vers la mémoire et le fichier des instructions hex */
/* remplit la partie segment assembleur de la mémoire à partir des valeurs hex */
/* retourne la valeur de la dernière case mémoire contenant une instruction (vMax du PC) */
int chargeProgramme(memoire *mem, const char* progHex) {
  FILE *prog=NULL;
  char* instruction=NULL;
  size_t len=0;
  int pc=DEBUT_PROG; /* On met le PC à l'init */
  prog=fopen(progHex,"r"); /* On ouvre le programme */
  if(NULL==prog){
    printf("Erreur d'ouverture du fichier\n");
    exit(-1);
  }
  /* On met en mémoire les instructions une à une en incrémentant le PC */
  while (getline(&instruction,&len,prog)!=-1) {
    insertion(pc,hexToDec(instruction),mem);
    pc+=4;
  }
  /* On libère le buffer et on ferme le fichier */
  free(instruction);
  fclose(prog);
  return (pc-4);
}

/* Prend en entrée une instruction hexadécimale (demandé dans les specifications) */
/* Exécute l'instruction, met à jour les registres et la mémoire et change le PC */
void execInstruction(long int hex, registre** registres, instruction** instructions, memoire *mem) {
  instruction *found=NULL;
  registre *rs=NULL,*rt=NULL,*rd=NULL,*hi=NULL,*lo=NULL,*pc=NULL;
  int rsI=0,rtI=0,rdI=0,sa=0,imm=0,opcode=0;
  long int value=0,tmp=0;
  /* Buff est nécessaire au fonctionnement de intVersChaine */
  char *buff=NULL;
  if((buff=(char *)calloc(TAILLE_MAX_INT,sizeof(char)))==NULL){exit(1);};
  /* Si la taille de valeur hexadécimale est inférieure à la taille max */
  if (hex<=0xFFFFFFFF) {
    pc=trouveRegistre(registres,"PC"); /* On initialise le registre du PC */
    /* Cas NOP, on avance d'une instruction */
    if (hex==0) {
      pc->valeur+=4;
    }
    /* Si les 6 premiers zéro sont nul -> R */
    else if ((hex&(0xfc000000))==0) { /* Masque : 1111110...0 */
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
          /* On initialise les registres rs,rt et rd */
          rs=trouveRegistre(registres,intVersChaine(rsI,buff));
          rt=trouveRegistre(registres,intVersChaine(rtI,buff));
          rd=trouveRegistre(registres,intVersChaine(rdI,buff));
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
            /* Initialise les registres HI et LO */
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
                rs->valeur+=DEBUT_PROG; /* On se place au bon endroit */
                if (rs->valeur>=DEBUT_PROG) {
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
            /* Initialise les registres HI et LO */
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
    /* Sinon c'est une instruction I ou J */
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
          /* Initialise les registres rs et rt */
          rs=trouveRegistre(registres,intVersChaine(rsI,buff));
          rt=trouveRegistre(registres,intVersChaine(rtI,buff));
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
  free(buff); /* On libère le buffer de intVersChaine */
}

/* prend en entrée un pointeur vers : la mémoire,les registres et les instructions et le fichier des segments asm */
/* exécute le programme en faisant appel aux sous fonctions */
void execProgramme(memoire *mem, registre** registres, instruction** instructions, char* prog) {
  long int instruction=0;
  registre *pc=NULL;
  int pcMax=0;
  /* Init du registre PC */
  pc=trouveRegistre(registres,"PC");
  (pc->valeur)=DEBUT_PROG;
  /* Chargement du programme et détermination du pc max */
  pcMax=chargeProgramme(mem,prog); /* On mémorise le pcMAx */
  /* Tant qu'on à pas atteint la dernière instruction */
  while((pc->valeur)<=pcMax) {
    instruction=valeurMemoire(pc->valeur,mem); /* On récupère la valeur de l'instruction en mémoire */
    execInstruction(instruction,registres,instructions,mem); /* Exécute l'opération, s'occupe d'incrémenter le PC */
  }
}
