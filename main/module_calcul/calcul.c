#include "calcul.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execTypeR(instruction *found, int opcode, int rsI, int rtI, int rdI, int sa, registre *pc, registre **registres, instruction **instructions, memoire *mem) {
  registre *rs=NULL,*rt=NULL,*rd=NULL,*hi=NULL,*lo=NULL;
  rs=registres[rsI];
  rt=registres[rtI];
  rd=registres[rdI];
  if (found->ordreBits==1) {
    if (found->styleRemplissage==1) { /* ADD/AND/XOR/OR/SLT/SUB */
      if (rs!=NULL && rt!=NULL && rd!=NULL && rdI!=0) {
        /* ADD */
        switch(opcode) {
          case 0x22: /* SUB */
            rd->valeur=(rs->valeur-rt->valeur); break;
          case 0x24: /* AND */
            rd->valeur=(rs->valeur&rt->valeur); break;
          case 0x26: /* XOR */
            rd->valeur=(rs->valeur^rt->valeur); break;
          case 0x25: /* OR */
            rd->valeur=(rs->valeur|rt->valeur); break;
          case 0x2A: /* SLT */
            if (rs->valeur<rt->valeur) {rd->valeur=1;}
            else {rd->valeur=0;};
            break;
          case 0x20: /* ADD */
            if ((rs->valeur+rt->valeur)<=0xffffffff) {rd->valeur=(rs->valeur+rt->valeur);}
            else {printf("Exception : Overflow\nNo changes\n");};
            break;
        }
      }
      pc->valeur+=4;
    }
    else if (found->styleRemplissage==3) { /* SLL | Pas par opcode */
      if (rt!=NULL && rd!=NULL && rdI!=0) {rd->valeur=(rt->valeur<<sa);}
      pc->valeur+=4;
    }
  }
  else if (found->ordreBits==2) { /* ROTR/SLR | Pas par opcode */
    if (rt!=NULL && rd!=NULL && rdI!=0) {
      switch(found->styleRemplissage) {
        case 1: /* ROTR */
          rd->valeur=((rt->valeur>>sa)|((rt->valeur)<<((NB_BIT_REGISTRE)-sa))); break;
        case 2: /* SRL */
          rd->valeur=((rt->valeur)>>sa); break;
      }
    }
    pc->valeur+=4;
  }
  else if (found->ordreBits==3) { /* MULT/DIV */
    hi=registres[33]; /* HI */
    lo=registres[34]; /* LO */
    if (rs!=NULL && rt!=NULL && hi!=NULL && lo!=NULL) {
      switch(opcode) {
        case 0x18: /* MULT */
          hi->valeur=((((rs->valeur*rt->valeur)&0xffffffff00000000)>>32)&MASQUE_MAX);
          lo->valeur=(((rs->valeur*rt->valeur)&0xffffffff)&MASQUE_MAX);
          break;
        case 0x1A: /* DIV */
          hi->valeur=(rs->valeur%rt->valeur);
          lo->valeur=(rs->valeur/rt->valeur);
          break;
      }
    }
    pc->valeur+=4;
  }
  else if (found->ordreBits==4) { /* JR */
    if (rs!=NULL && pc!=NULL) {
      switch(opcode) {
        case 0x08: /* JR */
          if ((rs->valeur+DEBUT_PROG)>=DEBUT_PROG) {pc->valeur=(rs->valeur+DEBUT_PROG);}
          else {printf("Erreur - JR : Valeur du PC trop faible\n");}
          break;
      }
    }
    pc->valeur+=4;
  }
  else if (found->ordreBits==5) { /* MFHI/MFLO */
    hi=registres[33]; /* HI */
    lo=registres[34]; /* LO */
    if (rd!=NULL && hi!=NULL && lo!=NULL && rdI!=0) {
      switch(opcode) {
        case 0x10: /* MFHI */
          rd->valeur=hi->valeur; break;
        case 0x12: /* MFLO */
          rd->valeur=lo->valeur; break;
      }
    }
    pc->valeur+=4;
  }
  else if (found->ordreBits==6) {pc->valeur+=4;}; /* Pas utilisé dans notre cas */
  if (rd!=NULL) {rd->valeur&=MASQUE_MAX;};
}

void execTypeI(instruction *found, int opcode, int rsI, int rtI, int imm, registre *pc, registre **registres, instruction **instructions, memoire *mem) {
  registre *rs=NULL,*rt=NULL;
  if (found->typeInstruction=='I') {
    rs=registres[rsI];
    rt=registres[rtI];
    if (found->ordreBits==1) {
      if (found->styleRemplissage==1) { /* ADDI/BEQ/BNE */
        if (rs!=NULL && rt!=NULL) {
          switch(opcode) {
            case 0x8: /* ADDI */
              if (rtI!=0 && ((rs->valeur+imm)<=0xFFFFFFFF)) {rt->valeur=(rs->valeur+imm);}
              else {printf("Exception : Overflow\nNo changes\n");};
              break;
            case 0x4: /* BEQ */
              if (rs->valeur==rt->valeur) {pc->valeur+=(imm<<2);}; break;
            case 0x5: /* BNE */
              if (rs->valeur!=rt->valeur) {pc->valeur+=(imm<<2);}; break;
          }
        }
        pc->valeur+=4;
      }
      if (found->styleRemplissage==2) { /* BGTZ/BLEZ */
        if (rs!=NULL) {
          switch(opcode) {
            case 0x7: /* BGTZ */
              if (rs->valeur>0) {pc->valeur+=(imm<<2);}; break;
            case 0x6: /* BLEZ */
              if (rs->valeur<=0) {pc->valeur+=(imm<<2);}; break;
          }
        }
        pc->valeur+=4;
      }
      if (found->styleRemplissage==3) { /* LUI */
        if (rt!=NULL) {
          switch(opcode) {
            case 0xf: /* LUI */
              if (rtI!=0) {rt->valeur=(imm<<16);}; break;
          }
        }
        pc->valeur+=4;
      }
      if (found->styleRemplissage==4) { /* LW/SW */
        if (rs!=NULL && rt!=NULL) {
          switch(opcode) {
            case 0x23: /* LW */
              if (rtI!=0) {rt->valeur=valeurMemoire(mem,(rs->valeur+imm));}; break;
            case 0x2b: /* SW */
              insertion(mem,(rs->valeur+imm),rt->valeur); break;
          }
        }
        pc->valeur+=4;
      }
    }
  }
}

void execTypeJ(instruction *found, int opcode, int imm, registre *pc, registre **registres) {
  registre *ra=NULL;
  if (found->ordreBits==1) { /* JAL/J */
    if (found->styleRemplissage==1) { /* JAL */
      ra=registres[31];
      ra->valeur=pc->valeur+4;
      pc->valeur+=(imm<<2);
    }
    else if (found->styleRemplissage==2) { /* J */
      pc->valeur+=(imm<<2);
    }
  }
  pc->valeur=imm;
}

/* Prend en entrée une instruction hexadécimale (demandé dans les specifications) */
/* Exécute l'instruction, met à jour les registres et la mémoire et change le PC */
void execInstruction(unsigned long int hex, registre **registres, instruction **instructions, memoire *mem) {
  instruction *found=NULL;
  registre *pc=NULL;
  int rsI=0,rtI=0,rdI=0,sa=0,imm=0,opcode=0;
  /* Si la taille de valeur hexadécimale est inférieure à la taille max on traite l'instruction */
  if (hex<=0xFFFFFFFF) {
    pc=registres[32]; /* On charge le registre du PC */
    if (hex==0) {pc->valeur+=4;} /* Cas NOP, on avance d'une instruction */
    /* Si les 6 premiers zéro sont nul -> Type R */
    else if ((hex&(0xfc000000))==0) { /* Masque : 11111100...0 */
      opcode=hex&0x3f;
      if ((found=trouveOpcode(instructions,opcode,'R'))!=NULL) {
        rsI=((hex>>21) & 0x1F); /* Valeur décimale des 21 à 26 bits */
        rtI=((hex>>16) & 0x1F);
        rdI=((hex>>11) & 0x1F);
        sa=((hex>>6) & 0x1F);
        execTypeR(found,opcode,rsI,rtI,rdI,sa,pc,registres,instructions,mem);
      }
    }
    /* Sinon c'est une instruction I ou J */
    else {
      opcode=((hex>>26)&0x3f);
      if ((found=trouveOpcode(instructions,opcode,'I'))!=NULL) { /* Instruction de type I */
        if (found->typeInstruction=='I') {
          rsI=((hex>>21) & 0x1F);
          rtI=((hex>>16) & 0x1F);
          imm=(hex & 0xFFFF);
          /* Identification ROTR/SLR car même opcode */
          if (found->ordreBits==2) {
            if ((hex>>21)&0x1) {found=trouveOperation(instructions,"ROTR");}
            else {found=trouveOperation(instructions,"SRL");}
          }
          imm=complementADeux(imm,16); /* On prend le complément à 2 d'un 16 bits car signé */
          execTypeI(found,opcode,rsI,rtI,imm,pc,registres,instructions,mem);
        }
        else if (found->typeInstruction=='J') {
          imm=(hex&0x3FFFFFF);
          execTypeJ(found,opcode,imm,pc,registres);
        }
      }
    }
  }
  else {printf("Format de l'instruction incorrect 0x%lx\n", hex);}
}
