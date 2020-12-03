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
    fscanf(fs,"%[^,],%x,%c,%d,%d,%d*",tmp->nom,&tmp->opcode,&tmp->typeInstruction,&tmp->ordreBits,&tmp->styleRemplissage,&tmp->nbOperande);
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
    if (instructions[i]->typeInstruction==type && (instructions[i]->opcode==opcode)) {
      ret=instructions[i];
      nonTrouvee=0;
    }
    i++;
  }
  return ret;
}

/* PARSSAGE */

/* Prend en entrée un chaine s (l'instruction) */
/* Retourne le nombre d'opérande dans un opération passée en entrée */
int nombreOperande(char *s) {
  int ret=0,i=0,commence=0,space=0;
  while(s[i]!='\0') {
    /* La ligne commence quand on rencontre une lette */
    if(isalpha(s[i]) && !commence) {commence=1;}
     /* Il y a au moins une opérande quand on recontre un espace après le début */
    if(isspace(s[i]) && commence && !space) {space=1;}
    /* Si on rencontre un caractère de fin d'opérande on incrémente */
    if (commence && space && (s[i]==',' || s[i]=='(' || s[i]=='\n')) {ret++;}
    i++;
  }
  /* On incrémente de 1 si on a rencontré au moins une opérande */
  if (ret!=0 || space) {ret++;}
  return ret;
}

/* Prend en entrée un pointeur vers une chaine non uniforisé et une pointeur pour stocké la chaine uniformisé */
/* S'occupe d'uniformiser la chaine */
void uniformisationInstruction(char *s, char *out) {
  int i=0,incremOut=0,writeSpace=-1,commence=0,ope=0;
  while(s[i]!='\0' && s[i]!='\n' && s[i]!='#') {
    /* La ligne commence lorsque l'on rencontre le premier caractère */
    if(isalpha(s[i]) && !commence) {
      commence=1;
    }
    /* Pour garder UN espace entre l'operation et les opérandes */
    /* Si on est sur un espace, qu'on a pas encore écrit un espace et que l'instruction à commencé on demande l'écriture d'un espace */
    if(isspace(s[i]) && writeSpace==-1 && commence) {
      out[incremOut++]=s[i];
      writeSpace=1;
    }
    /* Sinon si on à pas un espace et que l'instruction à commencé on recopie dans la chaine uniformisé */
    else if (s[i]!=' ' && commence) {
      out[incremOut++]=s[i];
      ope=incremOut;
    }
    i++;
  }
  /*On marque la fin de la chaîne */
  out[ope]='\0';
}

/* Retourne 1 si le numéro est valide, 0 sinon */
/* Vérifie que num appartienne bien à l'intervale [min,max] */
int check(int num, int min, int max) {
  int ret=1;
  if (num<min || num>max) {
    printf("ERREUR : %d n'est pas une valeur valide\n-> Vous devez choisir une valeur incluse dans [%d,%d]\n",num,min,max);
    ret=0;
  }
  return ret;
}

/* Retourne un tableau d'int contenant la valeur de toutes les operandes */
/* Si l'opérande est passé en hexadécimal elle est traduite en décimal */
/* Offset représente l'avancement dans le tableau de char ligne */
int* parseOperandes(char *ligne, int* offset, registre** registres) {
  int nbOperande=0;
  int* operandes=0;
  int i=*offset,j=0,k=0,numOpe=0,hexa=0;
  char tmp[16]=""; /* Tableau temporaire */
  nbOperande=nombreOperande(ligne); /* On compte les opérandes */
  /* Allocation du tableau de stockage des operandes que nous retournerons */
  if((operandes=calloc(nbOperande,sizeof(int)))==NULL){exit(1);};
  for(j=0;j<nbOperande;j++) {
    k=0;
    /* La fin d'une opérande est marqué par les caractères '(',',' ou '\0' */
    while(ligne[i]!='(' && ligne[i]!=',' && ligne[i]!='\0') {
      /* On prend que les nombres, les moins et les lettres */
      if ((ligne[i]>='0' && ligne[i]<='9') || ligne[i]=='-' || isalpha(ligne[i])) {
          tmp[k]=ligne[i];
          /* Si on rencontre un x c'est que la valeur est hexadécimale */
          if (ligne[i]=='x') {hexa=1;}
        k++;
      }
      i++;
    }
    tmp[k]='\0';
    /* Si on à une valeur immédiate en hexadécimal on la traduit en décimal */
    if (hexa) {operandes[numOpe]=hexToDec(tmp);}
    else {
      traduitRegistre(registres,tmp);
      operandes[numOpe]=valeurDecimale(tmp);
    }
    i++; /* On passe au caractère suivant */
    numOpe++; /* On avance d'un opérateur */
    *offset=i;
  }
  return operandes; /* On retourne le tableau des opérandes */
}

/* Stocke dans le tableau de char operation l'opération assembleur de l'instruction de la ligne (ADD...) */
/* Offset représente l'avancement dans le tableau de char ligne */
void parseOperation(char *ligne, char* operation, int* offset) {
  int i=*offset,j=0;
  /* Cas général : espace ou $ || NOP : \n ou \0 */
  while(ligne[i]!= ' ' && ligne[i]!='$' && ligne[i]!='\0' && ligne[i]!='\n') {
    operation[j++]=ligne[i++];
  }
  operation[j]='\0'; /* On met un \0 pour marquer la fin de la ligne et cacher la suite */
  *offset=i;
}

/* Traduit une ligne passé en argument (*ligne) en une valeur hexadécimale stockée dans *instructionHex (passé par adresse) */
/* Retourne 0 si l'operation n'existe pas, est invalide ou que les valeurs sont out of range 1 sinon */
int parseLigne(char *ligne, unsigned long int *instructionHex, instruction **instructions, registre **registres) {
  int offset=0,ret=0;
  unsigned long int hex=0;
  instruction *found=NULL;
  int rs=0,rt=0,imm=0,rd=0,sa=0;
  char operation[TAILLE_MAX_OPERATEUR]="";
  int *operandes=NULL;

  /* On récupère l'opération */
  parseOperation(ligne,operation,&offset);
  /* On cherche l'opération dans la structure mémoire (pointeur vers structure) */
  found=trouveOperation(instructions,operation);
  /* Si l'opération à été trouvé dans la structure mémoire et qu'on à le bon nombre d'opérande on continu */
  if (found!=NULL && nombreOperande(ligne)==found->nbOperande) {
    ret=1; /* On a une expression valide */
    /* On parse les operandes */
    operandes=parseOperandes(ligne,&offset,registres);
    /* Instruction de type R */
    if (found->typeInstruction=='R') {
      /* On remplit les valeurs de opcode,rs,rt,rd et sa */
      if (found->ordreBits==1) {
        /* ADD/AND/XOR/OR/SLT/SUB */
        if (found->styleRemplissage==1) {
          rs=operandes[1];
          rt=operandes[2];
          rd=operandes[0];
          sa=0;
        }
        /* NOP */
        else if (found->styleRemplissage==2) {
          ;
        }
        /* SLL */
        else if (found->styleRemplissage==3) {
          rs=0;
          rt=operandes[1];
          rd=operandes[0];
          sa=operandes[2];
        }
      }
      else if (found->ordreBits==2) {
        /* ROTR */
        if (found->styleRemplissage==1) {rs=1;}
        /* SRL */
        else if (found->styleRemplissage==2) {rs=0;}
        rt=operandes[1];
        rd=operandes[0];
        sa=operandes[2];
      }
      else if (found->ordreBits==3) {
        /* MULT */
        if (found->styleRemplissage==1) {
          rs=operandes[0];
          rt=operandes[1];
          rd=0;
          sa=0;
        }
      }
      else if (found->ordreBits==4) {
        /* JR -> Implémentation de la release 1 de l'architecture */
        if (found->styleRemplissage==1) {
          rs=operandes[0];
          rt=0;
          rd=0;
          sa=0;
        }
      }
      else if (found->ordreBits==5) {
        /* MFHI/MFLO */
        if (found->styleRemplissage==1) {
          rs=0;
          rt=0;
          rd=operandes[0];
          sa=0;
        }
      }
      else if (found->ordreBits==6) {
        /* SYSCALL */
        if (found->styleRemplissage==1) {
          /*  Que faut il mettre ? */
          rs=0;
          rt=0;
          rd=0;
          sa=0;
        }
      }
      /* Vérification des valeurs passées */
      /* Valeur sa € [0,31] */
      if (!(check(rs,0,31) && check(rt,0,31) && check(rd,0,31) && check(sa,0,31))) {
        free(operandes); /* On libère le tableau opérandes */
        return 0; /* On saute la ligne */
      }
      /* On obtient la valeur hexadécimale avec des décalages binaires et des masques */
      hex=0;
      hex|=(rs&0x1f)<<21;
      hex|=(rt&0x1f)<<16;
      hex|=(rd&0x1f)<<11;
      hex|=(sa&0x1f)<<6;
      hex|=(found->opcode);
    }
    /* Instruction de type I */
    else if (found->typeInstruction=='I') {
      /* On remplit les valeurs de opcode,rs,rt et imm */
      hex=((found->opcode)<<26); /* Opcode */
      if (found->ordreBits==1) {
        /* ADDI/BEQ/BNE */
        if (found->styleRemplissage==1) {
          /* ADDI change, l'instruction présente rt rs imm */
          if ((found->opcode)==0x08) {
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
        else if (found->styleRemplissage==2) {
          rs=operandes[0];
          rt=0;
          imm=operandes[1];
        }
        /* LUI */
        else if (found->styleRemplissage==3) {
          rs=0;
          rt=operandes[0];
          imm=operandes[1];
        }
        /* LW/SW */
        else if (found->styleRemplissage==4) {
          rs=operandes[2];
          rt=operandes[0];
          imm=operandes[1];
        }
        /* Vérification des valeurs passées */
        /* Valeur immédiate € [-32768,32767] */
        if (!(check(imm,-32768,32767) && check(rs,0,31) && check(rt,0,31))) {
          free(operandes); /* On libère le tableau opérandes */
          return 0; /* On saute la ligne */
        }
        /* On obtient la valeur hexadécimale avec des décalages binaires et des masques */
        hex|=(rs&0x1f)<<21;
        hex|=(rt&0x1f)<<16;
        hex|=(imm&0xffff);
        hex&=0xffffffff; /* sécurite, normalement inutile */
      }
    }
  }
  /* On met la valeur hexadécimale dans l'argument passé par adresse */
  *instructionHex=hex;
  free(operandes);
  return ret; /* 1 si valide 0 sinon (Pour les fausses opération et mauvais nombre d'opérandes et mauvaise valeurs)*/
}
