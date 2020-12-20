#include "calcul.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* prend en entrée tout les paramètres d'une instruction de type R (rs,rt,rd et sa) */
/* l'opcode, la structure de l'instruction, le pc et l'ensemble des registres */
/* exécute l'instruction, met à jour les registres et la mémoire et change le PC */
void execTypeR(int opcode, instruction *instr, int rsI, int rtI, int rdI, int sa, registre *pc, registre **registres) {
  registre *rs=NULL,*rt=NULL,*rd=NULL,*hi=NULL,*lo=NULL;
  rs=registres[rsI];
  rt=registres[rtI];
  rd=registres[rdI];
  if (instr->ordreBits==1) {
    if (instr->styleRemplissage==1) { /* ADD/AND/XOR/OR/SLT/SUB */
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
            if (((int)rs->valeur)<((int)rt->valeur)) {rd->valeur=1;}
            else {rd->valeur=0;};
            break;
          case 0x20: /* ADD */
            if ((((int)(rs->valeur+rt->valeur))<=0xFFFFFFFF)) {rd->valeur=(rs->valeur+rt->valeur);}
            else {printf("Erreur - ADD : Overflow\n\n");};
            break;
        }
      }
      pc->valeur+=4;
    }
    else if (instr->styleRemplissage==3) { /* SLL | Pas par opcode */
      if (rt!=NULL && rd!=NULL && rdI!=0) {rd->valeur=(rt->valeur<<sa);}
      pc->valeur+=4;
    }
  }
  else if (instr->ordreBits==2) { /* ROTR/SLR | Pas par opcode */
    if (rt!=NULL && rd!=NULL && rdI!=0) {
      switch(instr->styleRemplissage) {
        case 1: /* ROTR */
          rd->valeur=((rt->valeur>>sa)|((rt->valeur)<<((NB_BIT_REGISTRE)-sa))); break;
        case 2: /* SRL */
          rd->valeur=((rt->valeur)>>sa); break;
      }
    }
    pc->valeur+=4;
  }
  else if (instr->ordreBits==3) { /* MULT/DIV */
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
  else if (instr->ordreBits==4) { /* JR */
    if (rs!=NULL && pc!=NULL) {
      switch(opcode) {
        case 0x08: /* JR */
          if ((rs->valeur+DEBUT_PROG)>=DEBUT_PROG) {pc->valeur=(rs->valeur+DEBUT_PROG);}
          else {printf("Erreur - JR : Valeur du PC trop faible\n\n");}
          break;
      }
    }
    pc->valeur+=4;
  }
  else if (instr->ordreBits==5) { /* MFHI/MFLO */
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
  else if (instr->ordreBits==6) {pc->valeur+=4;}; /* Pas utilisé dans notre cas */
  if (rd!=NULL) {rd->valeur&=MASQUE_MAX;};
}

/* prend en entrée tout les paramètres d'une instruction de type I (rs,rt et imm) */
/* l'opcode, la structure de l'instruction, le pc et l'ensemble des registres */
/* exécute l'instruction, met à jour les registres et la mémoire et change le PC */
void execTypeI(int opcode, instruction *instr, int rsI, int rtI, int imm, registre *pc, registre **registres, memoire *mem) {
  registre *rs=NULL,*rt=NULL;
  if (instr->typeInstruction=='I') {
    rs=registres[rsI];
    rt=registres[rtI];
    if (instr->ordreBits==1) {
      if (instr->styleRemplissage==1) { /* ADDI/BEQ/BNE */
        if (rs!=NULL && rt!=NULL) {
          switch(opcode) {
            case 0x8: /* ADDI */
              if (rtI!=0 && (((int)(rs->valeur+imm)<=0xFFFFFFFF))) {rt->valeur=(rs->valeur+imm);}
              else {printf("Erreur - ADDI : Overflow\n\n");};
              break;
            case 0x4: /* BEQ */
              if (rs->valeur==rt->valeur) {pc->valeur+=(imm<<2);}; break;
            case 0x5: /* BNE */
              if (rs->valeur!=rt->valeur) {pc->valeur+=(imm<<2);}; break;
          }
        }
        pc->valeur+=4;
      }
      if (instr->styleRemplissage==2) { /* BGTZ/BLEZ */
        if (rs!=NULL) {
          switch(opcode) {
            case 0x7: /* BGTZ */
              if (((int)rs->valeur)>0) {pc->valeur+=(imm<<2);}; break;
            case 0x6: /* BLEZ */
              if (((int)rs->valeur)<=0) {pc->valeur+=(imm<<2);}; break;
          }
        }
        pc->valeur+=4;
      }
      if (instr->styleRemplissage==3) { /* LUI */
        if (rt!=NULL) {
          switch(opcode) {
            case 0xf: /* LUI */
              if (rtI!=0) {rt->valeur=(imm<<16);}; break;
          }
        }
        pc->valeur+=4;
      }
      if (instr->styleRemplissage==4) { /* LW/SW */
        if (rs!=NULL && rt!=NULL) {
          switch(opcode) {
            case 0x23: /* LW */
              if (rtI!=0) {rt->valeur=valeurMemoire(mem,4*(rs->valeur+imm));}; break;
            case 0x2b: /* SW */
              insertion(mem,4*(rs->valeur+imm),rt->valeur); break;
          }
        }
        pc->valeur+=4;
      }
    }
  }
}

/* prend en entrée tout le paramètre d'une instruction de type J (index) */
/* l'opcode, la structure de l'instruction, le pc et l'ensemble des registres */
/* exécute l'instruction, met à jour le registre ra et change le PC */
void execTypeJ(int opcode, instruction *instr, int index, registre *pc, registre **registres) {
  registre *ra=NULL;
  if (instr->ordreBits==1) { /* JAL/J */
    if (instr->styleRemplissage==1) { /* JAL */
      ra=registres[31];
      ra->valeur=pc->valeur+4;
      pc->valeur=index;
    }
    else if (instr->styleRemplissage==2) { /* J */
      pc->valeur=index;
    }
  }
}

/* Prend en entrée une instruction hexadécimale (demandé dans les spécifications) */
/* Appelle la fonction associé au type d'instruction qui s'occupe du reste */
void execInstruction(unsigned long int hex, registre **registres, instruction **instructions, memoire *mem) {
  instruction *instr=NULL;
  registre *pc=NULL;
  int rsI=0,rtI=0,rdI=0,sa=0,imm=0,opcode=0;
  /* Si la taille de valeur hexadécimale est inférieure à la taille max on traite l'instruction */
  if (hex<=0xFFFFFFFF) {
    pc=registres[32]; /* On charge le registre du PC */
    if (hex==0) {pc->valeur+=4;} /* Cas NOP, on avance d'une instruction */
    /* Si les 6 premiers zéro sont nul -> Type R */
    else if ((hex&(0xfc000000))==0) { /* Masque : 11111100...0 */
      opcode=hex&0x3f;
      if ((instr=trouveOpcode(instructions,opcode,'R'))!=NULL) {
        rsI=((hex>>21) & 0x1F); /* Valeur décimale des 21 à 26 bits */
        rtI=((hex>>16) & 0x1F);
        rdI=((hex>>11) & 0x1F);
        sa=((hex>>6) & 0x1F);
        execTypeR(opcode,instr,rsI,rtI,rdI,sa,pc,registres);
      }
    }
    /* Sinon c'est une instruction I ou J */
    else {
      opcode=((hex>>26)&0x3f);
      if ((instr=trouveOpcode(instructions,opcode,'I'))!=NULL) { /* Instruction de type I */
        if (instr->typeInstruction=='I') {
          rsI=((hex>>21) & 0x1F);
          rtI=((hex>>16) & 0x1F);
          imm=(hex & 0xFFFF);
          /* Identification ROTR/SLR car même opcode */
          if (instr->ordreBits==2) {
            if ((hex>>21)&0x1) {instr=trouveOperation(instructions,"ROTR");}
            else {instr=trouveOperation(instructions,"SRL");}
          }
          imm=complementADeux(imm,16); /* On prend le complément à 2 d'un 16 bits car signé */
          execTypeI(opcode,instr,rsI,rtI,imm,pc,registres,mem);
        }
        else if (instr->typeInstruction=='J') {
          imm=(hex&0x3FFFFFF);
          execTypeJ(opcode,instr,imm,pc,registres);
        }
      }
    }
  }
  else {printf("Format de l'instruction incorrect 0x%lx\n", hex);}
}
