#include "hex.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* MEMOIRE INSTRUCTIONS */

/* Prend en entrée la structure de stockage vide et le fichier de stockage */
/* Remplit la structure de stockage à l'aide du fichier contenant les opérations */
void remplissageStructInstruction(instruction **instructions, const char* fichier) {
  FILE *fs=fopen(fichier,"r");
  instruction *tmp=NULL;
  int i=0;
  if(fs==NULL) {
    printf("Erreur lors de l'ouverture du fichier");
    exit(-1);
  }
  fseek(fs,0,SEEK_SET);
  for(i=0;i<NB_OPERATIONS;i++) {
    if((instructions[i]=malloc(sizeof(instruction)))==NULL){exit(1);};
    tmp=instructions[i];
    /* La ligne dans le fichier est de la forme : ADD,0x20,R,1,1,3 */
    fscanf(fs,"%[^,],%x,%c,%d,%d,%d,%d,%d*",tmp->nom,&tmp->opcode,&tmp->typeInstruction,&tmp->ordreBits,&tmp->styleRemplissage,&tmp->nbOperande,&tmp->checksumReg,&tmp->checksumImm);
    fgetc(fs); /* Enlève \n */
  }
  fclose(fs);
}

/* Affiche les informations contenu dans une structure de stockage */
void afficheInstruction(instruction *instruction) {
  printf("Nom : %s\n", instruction->nom);
  printf("opcode : %06d\n", instruction->opcode);
  printf("Type d'instruction : %c\n", instruction->typeInstruction);
  printf("Ordre bits : %d\n", instruction->ordreBits);
  printf("Style de remplissage : %d\n", instruction->styleRemplissage);
  printf("Checksum Registe : %d\n", instruction->checksumReg);
  printf("Checksum Immédiat : %d\n", instruction->checksumImm);
  printf("\n");
}

/* Affiche toutes les structures du tableau de stockage */
void afficheStructInstruction(instruction **instructions) {
  int i=0;
  for(i=0;i<NB_OPERATIONS;i++) {
    printf("--%d--\n", i);
    afficheInstruction(instructions[i]);
  }
}

/* Prend en entrée la structure de stockage et la libère */
void liberationInstruction(instruction** instructions) {
  int i=0;
  for (i=0;i<NB_OPERATIONS;i++) {
    free(instructions[i]);
  }
}

/* Prend en entrée la structure de stockage et le nom d'une instruction */
/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
instruction* trouveOperation(instruction** instructions, char* nom) {
  int i=0, nonTrouvee=1;
  instruction *ret=NULL;
  /* On parcourt tout le tableau instructions tant qu'on est pas à la fin ou que l'on à pas trouvé */
  while (nonTrouvee && i<NB_OPERATIONS) {
    if (strcmp(instructions[i]->nom,nom)==0) {
      ret=instructions[i];
      nonTrouvee=0;
    }
    i++;
  }
  return ret;
}

/* Prend en entrée la structure de stockage un opcode et un type d'opération */
/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
instruction* trouveOpcode(instruction** instructions, int opcode, char type) {
  int i=0, nonTrouvee=1;
  instruction *ret=NULL;
  /* On parcourt tout le tableau instructions tant qu'on est pas à la fin ou que l'on à pas trouvé */
  while (nonTrouvee && i<NB_OPERATIONS) {
    if ((instructions[i]->typeInstruction==type || instructions[i]->typeInstruction=='J') && (instructions[i]->opcode==opcode)) {
      ret=instructions[i];
      nonTrouvee=0;
    }
    i++;
  }
  return ret;
}

/* LABEL */

/* prend en entrée le fichier du programme, la structure des instruction, des registres et la table des symboles */
/* on écrit par adresse */
/* remplis la table des symboles et doit pour cela vérifié si les instructions sont correcte (parse + checksum) pour que la valeur du PC soit correcte */
void fillSymbols(char *input, instruction **instructions, registre** registres, symtable *symbols) {
  int nbOpe=0,nbReg=0,nbImm=0,num=0,coeffOpe=0;
  char *p=NULL,*parse=NULL,*tampon=NULL,*s=NULL; /* tampon va être la zone de travail de strtok pour pouvoir utiliser strtok dans deux fonctions */
  int ret=1;
	FILE *fin=NULL;
	int pc=0xDDDC;
	instruction *instr=NULL;
	size_t len=0;
	int tailleTab=0;
	fin=fopen(input, "r");
  if (fin==NULL) {
    printf("Erreur lors de l'ouverture du fichier '%s'\n",input);
    exit(-1);
  }
	/* on parcourt tout le fichier ligne par ligne */
	while(getline(&s,&len,fin)!=-1) {
		ret=1;
		nbReg=nbImm=nbOpe=num=coeffOpe=0;
		parse=malloc(strlen(s)*sizeof(char));
	  int state=nettoyageInstruction(s,&parse,&tailleTab,&nbOpe);
		/* même principe que pour le parseur */
	  if (state==0) {
	    free(parse);
	    ret=0;
	  }
	  else if (state==1) {
	    ret=1;
			insertionQueue(symbols,parse,pc);
			free(parse);
	  }
	  else {
	    /* On sépare l'instruction avec l'espace comme séparateur */
	    for (p=strtok_r(parse," ",&tampon);p!=NULL;p=strtok_r(NULL, " ",&tampon)) {
	      /* Ecriture */
	      if (num==0 && nbOpe>=0) {
	        instr=trouveOperation(instructions,p);
	        if(instr==NULL) {ret=0;}
	      }
	      /* Calcul de notre checksum */
	      if (p[0]=='$') {
	        if(!traduitRegistre(registres,p)) {ret=0;};
	        nbReg+=2<<coeffOpe++; /* On ajoute la puissance de deux du numéro de l'opérande */
	      }
	      else if (isdigit(p[0]) || p[0]=='-') {
	        nbImm+=2<<coeffOpe++;
	      }
	      else if (num>0 && isalpha(p[0])) {
	        nbImm+=2<<coeffOpe++;
	      }
	      /* Si il y a une parenthèse ouvrante dans l'opérande */
	      if((p=strchr(p,'('))!=NULL) {
	        p++; /* On va au caractère d'après */
	        if (p[0]=='$') {
	          if(!traduitRegistre(registres,p)) {ret=0;};
	          nbReg+=2<<coeffOpe++;
	        }
	        else if (isdigit(p[0]) || p[0]=='-') {
	          nbImm+=2<<coeffOpe++;
	        }
	      }
	      num++;
	    }
	    /* Comparaison du checksum avec le checksum théorique */
	    if (ret!=0) {
	      if((nbReg!=instr->checksumReg) ||( nbImm!=instr->checksumImm)){ret=0;};
			}
			if (ret==1) {pc+=4;}
			free(parse);
			free(p);
	  }
	}
	free(s);
}

/* PARSAGE */

/* prend en entrée le checksum calculé et le checksum théorique pour les registres et les immédiats */
/* les compare est dit ou sont les différences et donc ou sont les erreurs */
int compareChecksum(char* ligne, int checksumCalcR,int checksumTheorR, int checksumCalcI,int checksumTheorI, int type) {
  int val[]={2,4,8}; /* checksumCalc des 3 puissances de deux utilisé dans le calcul de la somme de controle */
  int ret=1,i=0;
  if ((checksumCalcR!=checksumTheorR) || (checksumCalcI!=checksumTheorI)) {
    for(i=0;i<3;i++) {
      if((checksumTheorR&val[i])!=0 && (checksumCalcR&val[i])!=(checksumTheorR&val[i])) {
        if(ret) {printf("\n%s",ligne);};
        ret=0;
        printf(" Erreur sur le %dème paramètre, un argument de type registre est attendu\n",i+1); /* REG */
      }
      if((checksumTheorI&val[i])!=0 && (checksumCalcI&val[i])!=(checksumTheorI&val[i])) {
        if(ret) {printf("%s ",ligne);};
        ret=0;
        printf(" Erreur sur le %dème paramètre, un argument de type immédiat est attendu\n",i+1); /* IMM */
      }
    }
  }
  return ret;
}

/* prend une ligne du fichier (src) un pointeur vers la chaine que l'on va parser */
/* ainsi que la taille du tableau opérandes et le nombre d'opérande que nous allons écrire par adresse */
/* met au propre de l'expression, chaque partie est séparée de la précédente par un espace */
/* retourne 0 -> echec | 1 -> label | 2 -> instruction */
int nettoyageInstruction(char *src, char **parse, int *tailleTab,int *nbOpe) {
  int i=0,commence=0,debutOpe=0,incremOut=0;
  while(src[i]!='\0' && src[i]!='\n' && src[i]!='#') {
    if(isalpha(src[i]) && !commence) {commence=1;}; /* Commence=1 : Opération trouvée */
    if (src[i]==',' || src[i]==' ') {
      debutOpe=1;
    }
    else if ((isalnum(src[i]) || src[i]=='$' || src[i]==':' || src[i]=='(' || src[i]==')' || src[i]=='-') && commence>0) { /* on filtre */
      if (src[i]=='(') {*tailleTab=1;}; /* Si on à un offset imm(reg) on aura un paramètre de plus dans le tableau */
      if (debutOpe && commence==2) { /* Commence=2 : Opération écrite */
        (*parse)[incremOut++]=' '; /* On ajoute un ' ' */
        (*nbOpe)++;
      }
      debutOpe=0;
      commence=2;
      (*parse)[incremOut++]=src[i]; /* On copie le caractère */
    }
    i++; /* On avance dans la ligne source */
  }
  if (incremOut==0) {
    return 0; /* On a rien copié -> Ligne incorecte */
  }
  (*parse)[incremOut]='\0';
  if (strchr((*parse),':')!=NULL) {
    (*parse)[incremOut-1]='\0'; /* On enlève le : */
    return 1; /* On a un label */
  }
  (*tailleTab)+=(*nbOpe); /* Mise à jour de la taille du tableau des opérandes */
  return 2; /* Tout s'est bien passé */
}

/* prend en entrée la ligne d'entrée, la table des symboles et la mémoire des instructions */
/* ainsi que des variables par adresses que la fonction va modifier lors du calcul des checksums */
/* pratique, de la mise en forme de l'expression et de l'identification de l'instruction */
/* retourne : 1 c'est bon on a calculé || 0 opération invalide */
int calculChecksum(instruction **instr, char *parse, char *out, int **incremOpe, registre **registres, int* nbOpe, int* nbImm, int* nbReg, instruction** instructions, symtable *symbols) {
  char *tampon=NULL;
  char *p=NULL;
  int num=0,coeffOpe=0,numOpe=0,tmp=0;
  char *parseTmp=strdup(parse); /* on garde une copie de parse pour l'affichage */
  for (p=strtok_r(parse," ",&tampon);p!=NULL;p=strtok_r(NULL, " ",&tampon)) {
    /* Ecriture */
    if (num==0 && (*nbOpe)>=0) {
      strcat(out,p);
      if((*nbOpe)!=0) {strcat(out," ");}; /* Si on a une instruction sans argument on ne met pas d'espace */
      /* Test d'existance de l'opération return NULL si inexistante */
      (*instr)=trouveOperation(instructions,p);
      if((*instr)==NULL) {
        free(parseTmp);
        printf("%s\n Erreur, l'opération est invalide\n",parseTmp);
        return 0; /* On arrête le calcul, l'opération n'est pas existante */
      }
    }
    /* On reconstitue l'instruction */
    else if (num!=(*nbOpe)) {
      strcat(out,p);
      strcat(out,",");
    }
    else {
      strcat(out,p);
    }
    /* Calcul de notre checksum */
    if (p[0]=='$') {
      if(!traduitRegistre(registres,p)) {
        printf("%s\n Erreur, %s n'est pas un registre valide\n",parseTmp,p);
        (*instr)=NULL;
      }
      (*incremOpe)[numOpe++]=valeurDecimale(p);
      (*nbReg)+=2<<coeffOpe++; /* On ajoute la puissance de deux du numéro de l'opérande */
    }
    else if (isdigit(p[0]) || p[0]=='-') {
      (*incremOpe)[numOpe++]=valeurDecimale(p);
      (*nbImm)+=2<<coeffOpe++; /* Pareil */
    }
    else if (num>0 && isalpha(p[0])) {
      /* On a une label dans ce cas on cherche la correspondance dans la table des symboles */
      if ((tmp=foundSymbol(symbols,p))!=-1) {
        (*incremOpe)[numOpe++]=tmp;
        (*nbImm)+=2<<coeffOpe++; /* On incrémente le checksum des immédiats */
      }
      else {printf("Label non trouvée\n");};
    }
    /* Si il y a une parenthèse ouvrante dans l'opérande on refait la même chose */
    if((p=strchr(p,'('))!=NULL) {
      p++; /* On va au caractère d'après */
      /* Même principe on incrémente en puissance de deux */
      if (p[0]=='$') {
        if(!traduitRegistre(registres,p)) {
          printf("%s\nErreur, %s n'est pas un registre valide\n",parseTmp,p);
          (*instr)=NULL;
        }
        (*incremOpe)[numOpe++]=valeurDecimale(p);
        (*nbReg)+=2<<coeffOpe++;
      }
      else if (isdigit(p[0]) || p[0]=='-') {
        (*incremOpe)[numOpe++]=valeurDecimale(p);
        (*nbImm)+=2<<coeffOpe++;
      }
    }
    num++;
  }
  free(parseTmp);
  return 1; /* On a calculé le checksum */
}

/* prend en entrée la mémoire des instruction, les registres, la mémoire, la table des symboles */
/* la ligne non édité, la ligne de sortie, un pointeur vers le tableau des opérandes et un pointeur vers la taille de ce tableau */
/* s'occupe de parser l'expression c'est à dire d'identifier toutes les erreur possible et de remplir le tableau des opérandes */
/* return 2: Expression valide || 1: Label || 0: Erreur */
int parsageInstruction(instruction **instr, instruction **instructions,registre** registres, symtable *symbols, char *s, char *out, int** operandes, int* tailleTab) {
  int nbOpe=0,nbReg=0,nbImm=0;
  char *parse=NULL; /* tampon va être la zone de travail de strtok pour pouvoir utiliser strtok dans deux fonctions */
  int ret=2; /* 2 -> expression valide */
  parse=malloc(strlen(s)+1);
  int state=nettoyageInstruction(s,&parse,tailleTab,&nbOpe);
  if (state==0) {
    ret=0; /* Retourne 0 -> On affiche pas d'erreur car vide */
  }
  else if (state==1) {
    ret=1; /* label */
    strcat(out,parse);
  }
  else if (*tailleTab==0) { /* Pour les opérations sans opérandes */
    (*instr)=trouveOperation(instructions,parse);
    strcat(out,parse);
    if ((*instr)==NULL) {
      printf("\n%s\n Erreur l'opération n'est pas reconnue\n", out);
      ret=0;
    }
  }
  else {
    if(((*operandes)=(int *)calloc((*tailleTab),sizeof(int)))==NULL){exit(1);};
    /* Les checksums seront dans nbImm et nbReg */
    calculChecksum(instr,parse,out,operandes,registres,&nbOpe,&nbImm,&nbReg,instructions,symbols);
    /* Comparaison du checksum avec le checksum théorique */
    if ((*instr)!=NULL) {
      /* Si on a pas le bon nombre d'opérandes on retourne un erreur */
      if ((*tailleTab)!=(*instr)->nbOperande) {
        printf("\n%s Erreur on attend %d opérandes, on en a %d\n",s,(*instr)->nbOperande,(*tailleTab));
        (*instr)=NULL;
        ret=0;
      }
      else {
        /* Si les checksums ne sont pas égaux la fonction affiche les erreurs et on quitte la fonction sur une erreur */
        if(compareChecksum(s,nbReg,(*instr)->checksumReg,nbImm,(*instr)->checksumImm,1)==0) {
          (*instr)=NULL;
          ret=0;
        }
      }
    }
  }
  free(parse);
  return ret;
}

/* prend en entrée un nombre ainsi qu'une borne min et une borne max */
/* vérifie que num appartienne bien à l'intervale [min,max] */
/* retourne 1 si le numéro est valide, 0 sinon */
int check(char *ligne, int num, int min, int max) {
  int ret=1;
  if (num<min || num>max) {
    printf("\n%s\n",ligne);
    printf(" Erreur %d n'est pas une valeur valide\n  -> Vous devez choisir une valeur incluse dans [%d,%d]\n",num,min,max);
    ret=0;
  }
  return ret;
}

/* on prend en entrée l'ensemble des mémoires (reg, symtable, instr...), la ligne pure, un pointeur vers la ligne de sortie */
/* Traduit une ligne passé en argument (*ligne) en un pointeur vers la valeur hexadécimale que l'on calculé (*instructionHex) */
/* Retourne 0 si l'operation n'existe pas, est invalide ou que les valeurs sont out of range 1 si elle est valide 2 si c'est une label */
int hexLigne(char *ligne, int pc, char **ligneParse, unsigned long int *instructionHex, symtable *symbols, instruction **instructions, registre **registres) {
  int ret=0;
  unsigned long int hex=0;
  instruction *instr=NULL;
  int rs=0,rt=0,imm=0,rd=0,sa=0;
  char *ligneOut=*ligneParse;
  int *operandes=NULL;
  int tailleTab=0;
  if (ligne!=NULL) {
    if((ligneOut=(char *)calloc(strlen(ligne),sizeof(char)))==NULL){exit(1);};
    if (parsageInstruction(&instr,instructions,registres,symbols,ligne,ligneOut,&operandes,&tailleTab)) { /* Si on retourne 1 c'est un label */
      insertionQueue(symbols,ligneOut,pc);
      ret=2;
    }
    else if (instr!=NULL) {
      ret=1; /* On a une expression valide */
      /* Instruction de type R */
      if (instr->typeInstruction=='R') {
        /* On remplit les valeurs de opcode,rs,rt,rd et sa */
        if (instr->ordreBits==1) {
          /* ADD/AND/XOR/OR/SLT/SUB */
          if (instr->styleRemplissage==1) {
            rs=operandes[1];
            rt=operandes[2];
            rd=operandes[0];
            sa=0;
          }
          /* NOP */
          else if (instr->styleRemplissage==2) {
            ;
          }
          /* SLL */
          else if (instr->styleRemplissage==3) {
            rs=0;
            rt=operandes[1];
            rd=operandes[0];
            sa=operandes[2];
          }
        }
        else if (instr->ordreBits==2) {
          /* ROTR */
          if (instr->styleRemplissage==1) {rs=1;}
          /* SRL */
          else if (instr->styleRemplissage==2) {rs=0;}
          rt=operandes[1];
          rd=operandes[0];
          sa=operandes[2];
        }
        else if (instr->ordreBits==3) {
          /* MULT */
          if (instr->styleRemplissage==1) {
            rs=operandes[0];
            rt=operandes[1];
            rd=sa=0;
          }
        }
        else if (instr->ordreBits==4) {
          /* JR -> Implémentation de la release 1 de l'architecture */
          if (instr->styleRemplissage==1) {
            rs=operandes[0];
            rt=rd=sa=0;
          }
        }
        else if (instr->ordreBits==5) {
          /* MFHI/MFLO */
          if (instr->styleRemplissage==1) {
            rd=operandes[0];
            rs=rt=sa=0;
          }
        }
        else if (instr->ordreBits==6) {
          /* SYSCALL */
          if (instr->styleRemplissage==1) {
            /*  Non implémenté */
            rs=rt=rd=sa=0;
          }
        }
        /* Vérification des valeurs passées */
        /* Valeur sa € [0,31] */
        if (!(check(ligneOut,rs,0,31) && check(ligneOut,rt,0,31) && check(ligneOut,rd,0,31) && check(ligneOut,sa,0,31))) {
          free(operandes); /* On libère le tableau opérandes */
          *ligneParse=ligneOut;
          return 0; /* On saute la ligne */
        }
        /* On obtient la valeur hexadécimale avec des décalages binaires et des masques */
        hex=0;
        hex|=(rs&0x1f)<<21;
        hex|=(rt&0x1f)<<16;
        hex|=(rd&0x1f)<<11;
        hex|=(sa&0x1f)<<6;
        hex|=(instr->opcode);
      }
      /* Instruction de type I */
      else if (instr->typeInstruction=='I') {
        /* On remplit les valeurs de opcode,rs,rt et imm */
        hex=((instr->opcode)<<26); /* Opcode */
        if (instr->ordreBits==1) {
          /* ADDI/BEQ/BNE */
          if (instr->styleRemplissage==1) {
            /* ADDI change, l'instruction présente rt rs imm */
            if ((instr->opcode)==0x08) {
              rs=operandes[1];
              rt=operandes[0];
            }
            /* rs rt imm */
            else {
              rs=operandes[0];
              rt=operandes[1];
            }
            imm=operandes[2];
          }
          /* BGTZ/BLEZ */
          else if (instr->styleRemplissage==2) {
            rs=operandes[0];
            rt=0;
            imm=operandes[1];
          }
          /* LUI */
          else if (instr->styleRemplissage==3) {
            rs=0;
            rt=operandes[0];
            imm=operandes[1];
          }
          /* LW/SW */
          else if (instr->styleRemplissage==4) {
            rs=operandes[2];
            rt=operandes[0];
            imm=operandes[1];
          }
          /* Vérification des valeurs passées */
          /* Valeur immédiate € [-32768,32767] */
          if (!(check(ligneOut,imm,-32768,32767) && check(ligneOut,rs,0,31) && check(ligneOut,rt,0,31))) {
            free(operandes); /* On libère le tableau opérandes */
            *ligneParse=ligneOut;
            return 0; /* On saute la ligne */
          }
          /* On obtient la valeur hexadécimale avec des décalages binaires et des masques */
          hex|=(rs&0x1f)<<21;
          hex|=(rt&0x1f)<<16;
          hex|=(imm&0xffff);
          hex&=0xffffffff; /* sécurité, normalement inutile */
        }
      }
      else if (instr->typeInstruction=='J') {
        hex=((instr->opcode)<<26); /* Opcode */
        if (instr->ordreBits==1) {
          /* ADDI/BEQ/BNE */
          if (instr->styleRemplissage==1) {
            /* ADDI change, l'instruction présente rt rs imm */
            if ((instr->opcode)==0x03) {
              imm=operandes[0];
            }
          }
        }
        hex|=(imm);
        hex&=0xffffffff; /* sécurité, normalement inutile */
      }
    }
    else {
      free(ligneOut);
    }
  }
  /* On met la valeur hexadécimale dans l'argument passé par adresse */
  *ligneParse=ligneOut;
  *instructionHex=hex;
  free(operandes);
  return ret; /* 1 si valide 0 sinon (Pour les fausses opération et mauvais nombre d'opérandes et mauvaise valeurs)*/
}
