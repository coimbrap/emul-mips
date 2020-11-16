#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hex.h"

/* Module tools */
#include "../module_tools/tools.h"

/* MEMOIRE INSTRUCTIONS */

/* Remplit la structure de stockage à l'aide du fichier contenant les opérations */
void remplissageStructInstruction(instruction *instructions[], const char* fichier) {
  FILE *fs=fopen(fichier,"r");
  instruction *tmp=NULL;
  int i=0;
  if(fs==NULL) {
    printf("Erreur lors de l'ouverture du fichier");
    exit(-1);
  }
  fseek(fs,0,SEEK_SET);
  for(i=0;i<NB_OPERATIONS;i++) {
    instructions[i]=malloc(sizeof(instruction));
    tmp=instructions[i];
    /* La ligne dans le fichier est de la forme : ADD,100000,R,1,1 */
    fscanf(fs,"%[^,],%x,%c,%d,%d,%d*",tmp->nom,&tmp->opcode,&tmp->typeInstruction,&tmp->ordreBits,&tmp->styleRemplissage,&tmp->nbOperande);
    fgetc(fs); /* Enlève \n */
  }
  fclose(fs);
}

/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
instruction* trouveOperation(instruction* instructions[], char* nom) {
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

/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
instruction* trouveOpcode(instruction* instructions[], int opcode, char type) {
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

/* PARSAGE DE L'INSTRUCTION */

/* Retourne le nombre d'opérande dans un opération passée en entrée */
int nombreOperande(char *s) {
  int ret=0,i=0,commence=0,space=0;
  while(s[i]!='\0') {
    if(isalpha(s[i]) && !commence) {commence=1;}
    if(isspace(s[i]) && commence && !space) {space=1;}
    if (commence && space && (s[i]==',' || s[i]=='(' || s[i]=='\n')) {ret++;}
    i++;
  }
  if (ret!=0 || space) {ret++;}
  return ret;
}

/* Retourne la chaine d'entrée uniformisé */
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

/* Retourne un tableau de string contenant la valeur de tout les operandes */
/* Offset représente l'avancement dans le tableau de char ligne */
int* parseOperandes(char *ligne, int* offset, registre** registres) {
  int nbOperande=0;
  int* operandes=0;
  int i=*offset,j=0,k=0,numOpe=0,hexa=0;
  nbOperande=nombreOperande(ligne);
  char* tmp=malloc(sizeof(char)*10);
  if((operandes=calloc(nbOperande,sizeof(int)))==NULL){exit(1);};
  for(j=0;j<nbOperande;j++) {
    k=0;
    while(ligne[i]!='(' && ligne[i]!=',' && ligne[i]!='\0') {
      if ((ligne[i]>='0' && ligne[i]<='9') || ligne[i]=='-' || isalpha(ligne[i])) {
          tmp[k]=ligne[i];
          if (ligne[i]=='x') {
            hexa=1;
          }
        k++;
      }
      i++;
    }
    tmp[k]='\0';
    /* Si on à une valeur immédiate en hexadécimal on la traduit en décimal dans le tableau opérande */
    /* Ce n'est pas le mieux car on remet la valeur en hexadécimal par la suite mais c'est le plus simple à implémenter pour le moment */
    if (hexa) {
      operandes[numOpe]=hexToDec(tmp);
    }
    else {
      tmp=traduitRegistre(registres,tmp);
      operandes[numOpe]=valeurDecimale(tmp);
    }
    i++;
    numOpe++;
    *offset=i;
  }
  return operandes;
}

/* Stocke dans le tableau de char operation l'opération assembleur de l'instruction de la ligne (ADD...) */
/* Offset représente l'avancement dans le tableau de char ligne */
void parseOperation(char *ligne, char* operation, int* offset) {
  int i=*offset,j=0;
  /* Cas général : espace ou $ || NOP : \n ou \0 */
  while(ligne[i]!= ' ' && ligne[i]!='$' && ligne[i]!='\0' && ligne[i]!='\n') {
    operation[j++]=ligne[i++];
  }
  operation[j]='\0';
  *offset=i;
}

/* Traduit une ligne passé en argument (*ligne) en un tableau de représentation binaire (int* bin) */
/* Retourne 0 si l'operation n'existe pas 1 sinon */
/* Prend la memoire des registres et des instructions en entrée */
int parseLigne(char *ligne, long int* instructionHex, instruction* instructions[], registre* registres[]) {
  int offset=0,ret=0;
  long int hex=0;
  instruction *found=NULL;
  int rs=0,rt=0,imm=0,rd=0,sa=0;
  char operation[TAILLE_MAX_OPERATEUR];
  int *operandes=NULL;
  /* On récupère l'opération */
  parseOperation(ligne,operation,&offset);
  /* On cherche l'opération dans la structure mémoire (pointeur vers structure) */
  found=trouveOperation(instructions,operation);
  /* Si l'opération à été trouvé dans la structure mémoire on continu */
  if (found!=NULL && nombreOperande(ligne)==found->nbOperande) {
    ret=1; /* On a une expression valide */
    /* Si ce n'est pas NOP on parse les operandes */
    if (!(found->typeInstruction=='R' && found->ordreBits==1 && found->styleRemplissage==2)) {
      operandes=parseOperandes(ligne,&offset,registres);
    }
    /* Instruction de type R */
    if (found->typeInstruction=='R') {
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
      hex=0;
      hex|=rs<<21;
      hex|=rt<<16;
      hex|=rd<<11;
      hex|=sa<<6;
      hex|=(found->opcode);
    }
    /* Instruction de type I */
    else if (found->typeInstruction=='I') {
      hex=((found->opcode)<<26); /* Opcode */
      if (found->ordreBits==1) {
        /* ADDI/BEQ/BNE */
        if (found->styleRemplissage==1) {
          /* ADDI change au niveau de l'instruction présente rt rs imm */
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
        imm&=0xffff;/* On efface un éventuel overflow */
        hex|=rs<<21;
        hex|=rt<<16;
        hex|=imm;
        hex&=0xffffffff; /* sécurite, normalement inutile */
      }
    }
  }
  *instructionHex=hex;
  return ret;
}

/* Lit le fichier d'instruction assembleur ligne par ligne */
/* Parse l'expression puis lance le remplissage du tableau binaire */
void parseFichier(char *input, char* output, int mode) {
  /* Fichiers d'entrée et de sortie du programme */
  FILE *fin=fopen(input, "r");
  FILE *fout=fopen(output, "w");
  FILE *tmp=fopen(".tmp","w"); /* Stockage de l'affichage */
  size_t len=0;
  char* buf=NULL;
  char *ligne=NULL;
  char *ligneOut=NULL;
  int programCounter=0,lignes=1;
  /* Fichiers pour remplir les mémoires (opérandes et registres) */
  char *listeope="src/listeOpe.txt";
  char *listereg="src/listeReg.txt";
  char c;
  int inW=1;
  long int instructionHex=0;
  /* Tableaux de mémoire des opérandes et des registres remplit à l'aide des fichiers de stockage */
  instruction *instructions[NB_OPERATIONS+1];
  registre* registres[NB_REGISTRE];
  /* On remplit les structures de stockage à partir des fichiers */
  remplissageStructInstruction(instructions,listeope);
  remplissageStructRegistre(registres,listereg);
  programCounter=INIT_PC;
  if (fin==NULL) {
    printf("Erreur lors de l'ouverture du fichier '%s'\n",input);
    exit(-1);
  }
  /* Initialisation du fichier (suppression du contenu ou création) */
  if (fout==NULL) {
    printf("Erreur lors de l'ouverture du fichier '%s'\n",output);
    exit(-1);
  }
  if (tmp==NULL) {
    printf("Erreur lors de l'ouverture du fichier '.tmp'\n");
    exit(-1);
  }

  while(getline(&ligne,&len,fin)!=-1) { /* Tant qu'on est pas à la fin du fichier */
    if(ligne[0]!='\n' && ligne[0]!='\0') { /* Si on n'à pas une ligne vide */
      /* On uniforise la ligne */
      ligneOut=(char *)malloc(strlen(ligne)*sizeof(char));
      uniformisationInstruction(ligne,ligneOut);
      if(ligneOut[0]!='\0') { /* Si la ligne uniformisé n'est pas vide */
         /* On a quelque chose */
         if (mode) {
           if (parseLigne(ligneOut,&instructionHex,instructions,registres)) {
             /* On parse la ligne */
            fprintf(fout,"%08lx\n",instructionHex);
            fprintf(tmp,"%08d 0x%08lx   %s\n",programCounter,instructionHex,ligneOut);
            programCounter+=4;
          }
           else if (mode==0) {printf("Erreur ligne %d, on passe à la suivante (opération non reconnue)\n",lignes);}
         }
         if (!mode) {
           if (parseLigne(ligneOut,&instructionHex,instructions,registres)) {
            printf("%s\n", ligneOut);
            printf("Instruction assembleur ligne %d : \n%s\n\n",lignes,ligneOut);
            printf("Expression hexadécimale : \n0x%08lx\n\n", instructionHex);
            printf("passer l'instruction: [p], instruction suivante: [enter], saut de la lecture: [s]\n");
            do {
              c=getchar();
              inW=1;
              if (c=='s' || c=='\n') {
                fprintf(fout,"%08lx\n",instructionHex);
                fprintf(tmp,"%08d 0x%08lx   %s\n",programCounter,instructionHex,ligneOut);
                programCounter+=4;
                inW=0;
                if (c=='s') {mode=1;}
              }
              else {clean_stdin();}
              if (c=='p') {inW=0;}
            } while(inW);
           }
           else {printf("Erreur ligne %d, on passe à la suivante (opération non reconnue)\n\n",lignes);}
         }
       }
       else if (mode==0) {printf("Erreur ligne %d, on passe à la suivante (ligne vide)\n\n",lignes);}
      free(ligneOut); /* On libère la ligne uniformisé */
    }
    else if (mode==0) {printf("Erreur ligne %d, on passe à la suivante (ligne vide)\n\n",lignes);}
    if (!mode) {lignes++;}
  }
  fprintf(tmp, "------------------ Fin -------------------\n");
  fclose(tmp);
  fclose(fout);
  /* Affichage final */
  tmp=fopen(".tmp","r");
  if (tmp) {
    printf("\n---- Assembleur ----\n\nPC       Hex          Instruction\n------------------------------------------\n");
    while (getline(&buf,&len,tmp)!=-1) {
      printf("%s",buf);
    }
  }
  else {
    printf("Pas d'affichage\n");
  }
  free(buf);
  fclose(tmp);
  remove(".tmp");
  fclose(fin); /* On ferme le fichier d'entrée */
}
