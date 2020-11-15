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
    fscanf(fs,"%[^,],%[^,],%c,%d,%d,%d*",tmp->nom,tmp->opcode,&tmp->typeInstruction,&tmp->ordreBits,&tmp->styleRemplissage,&tmp->nbOperande);
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

/* Les deux nombres binaire doivent être de la même taille !! */
int binStrIntCmp(char* binC, int* binD) {
  int ret=1,i=0;
  while (ret && binC[i]!='\0') {
    if ((binC[i]-'0')!=binD[i]) {
      ret=0;
    }
    i++;
  }
  return ret;
}

/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
instruction* trouveOpcode(instruction* instructions[], int* bin, char type) {
  int i=0, nonTrouvee=1;
  instruction *ret=NULL;
  /* On parcourt tout le tableau instructions tant qu'on est pas à la fin ou que l'on à pas trouvé */
  while (nonTrouvee && i<NB_OPERATIONS) {
    if (instructions[i]->typeInstruction==type && binStrIntCmp(instructions[i]->opcode,bin)) {
      ret=instructions[i];
      nonTrouvee=0;
    }
    i++;
  }
  return ret;
}

/* UNIFORMISATION DE L'INSTRUCTION */

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

/* PARTIE BINAIRE */

/* Ecrit à partir de l'offset (inclu) du tableau bin passé par adresse la représentation binaire de n (decimal) */
void decToBinary(int n, int* offset, int* bin) {
  /* Tableau temporaire */
  int binTmp[TAILLE_BIT_OPERATION];
  int i=0,j=0;
  while (n>0) {
    binTmp[i]=n%2;
    n=n/2;
    i++;
  }
  i--;
  /* Renverse le tableau binTmp dans bin */
  /*+(TAILLE_REG-(i+1)) permet de rajouter des zéros pour atteindre 5 bits */
  for (j=*offset+(TAILLE_REG-(i+1));i>=0;j++) {
    bin[j]=binTmp[i];
    i--;
  }
  *offset=j;
}

/* Ecrit à partir de l'offset (inclu) du tableau bin la valeur binaire d'une valeur immédiate ou d'un offset (sur 16 bits) */
/* Supporte les nombres négatif */
void decToBinaryImm(int n, int* offset, int* bin) {
  /* Tableau temporaire */
  int binTmp[TAILLE_IMM];
  int binTmp2[TAILLE_IMM];
  int i=0,j=0,neg=0,k=0;
  for(k=0;k<TAILLE_IMM;k++) {
    binTmp[k]=0;
    binTmp2[k]=0;
  }
  if (n<0) {
    n=-n;
    neg=1;
  }
  while (n>0) {
    binTmp[i]=n%2;
    n=n/2;
    i++;
  }
  i--;
  if (neg) {
    complementADeux(binTmp,binTmp2,TAILLE_IMM);
    i=TAILLE_IMM-1; /* Dernière cas du tableau binTmp2 */
    /* On renverse binTmp2 dans bin */
    for (j=*offset;i>=0;j++) {
      bin[j]=binTmp2[i];
      i--;
    }
  }
  else {
    i=TAILLE_IMM-1; /* Dernière cas du tableau binTmp */
    /* On renverse binTmp dans bin */
    for (j=*offset;i>=0;j++) {
      bin[j]=binTmp[i];
      i--;
    }
  }
  *offset=j;
}

/* Ecrit à partir d'un nombre binaire en string dans le tableau bin à partir de l'offset inclu */
void rempliBinTabBin(char* cBin, int* offset, int* bin) {
  int i=0,j=*offset;
  while (cBin[i]!='\0') {
    bin[j]=cBin[i]-'0';
    i++;
    j++;
  }
  *offset=j;
}

/* PARTIE HEXADECIMAL */

/* Transforme un tableau représentant une valeur binaire sur 32 bits en un tableau hexadécimal sur 8bits */
void binaryToHex(int* bin, char* hex, int size) {
  int i=0, j=0, k=0, l=0;
  int temp1=0;
  char temp2=0;
  int pui=3;
  for(i=0;i<size;i+=8){
    for(j=i;j<(i+8);j+=4){
      k=j;
      pui=3;
      while(k<(j+4)){
        temp1=temp1+bin[k]*puissance(2,pui);
        pui--;
        k++;
      }
      if(temp1==10){
        temp2='A';
      }
      else if(temp1==11){
        temp2='B';
      }
      else if(temp1==12){
        temp2='C';
      }
      else if(temp1==13){
        temp2='D';
      }
      else if(temp1==14){
        temp2='E';
      }
      else if(temp1==15){
        temp2='F';
      }
      else{
        temp2=temp1+'0';  /* ASCII : Int -> Char */
      }
      temp1=0;
      hex[l]=temp2;
      l++;
    }
  }
  hex[l]='\0';
}

/* Ecrit la valeur hexadécimale dans un fichier */
void ecrireHex(char* hex, char *fichier){
  int i=0;
  FILE *fout=NULL;
  /* On ouvre le fichier (déjà initialisé) en mode complétion */
  fout=fopen(fichier,"r+");
  if(NULL==fout){
    printf("Erreur d'ouverture du fichier\n");
    exit(-1);
  }
  fseek(fout, 0, SEEK_END);
  for(i=0;i<TAILLE_HEX_OPERATION-1;i++){
    fprintf(fout,"%c", hex[i]);
  }
  fprintf(fout,"%c\n", hex[TAILLE_HEX_OPERATION-1]);
  fclose(fout);
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

/* Permet de traduire toutes les opérandes d'un tableau de strings d'operandes */
void traduitOperandes(registre* registres[], char* operandes[], int nbOperande) {
  int i=0;
  for(i=0;i<nbOperande;i++) {
    /* Si le premier caractère de l'opérande est une lette alors on à affaire à une mnémonique (hex commence par Ox donc pas de risque) */
    if(operandes[i]!=NULL && isalpha(operandes[i][0])) {
      operandes[i]=traduitRegistre(registres,operandes[i]);
    }
  }
}

/* Retourne un tableau de string contenant la valeur de tout les operandes */
/* Offset représente l'avancement dans le tableau de char ligne */
char** parseOperandes(char *ligne, char* operandes[], int* offset) {
  int nbOperande=0;
  int i=*offset,j=0,k=0,numOpe=0,hexa=0;
  nbOperande=nombreOperande(ligne);
  if((operandes=malloc(sizeof(char*)*nbOperande))==NULL){exit(1);};
  for(j=0;j<nbOperande;j++) {
    if((operandes[j]=malloc(sizeof(char*)))==NULL){exit(1);};
  }
  for(j=0;j<nbOperande;j++) {
    k=0;
    while(ligne[i]!='(' && ligne[i]!=',' && ligne[i]!='\0') {
      if ((ligne[i]>='0' && ligne[i]<='9') || ligne[i]=='-' || isalpha(ligne[i])) {
          operandes[numOpe][k]=ligne[i];
          if (ligne[i]=='x') {
            hexa=1;
          }
        k++;
      }
      operandes[numOpe][k]='\0';
      i++;
    }
    /* Si on à une valeur immédiate en hexadécimal on la traduit en décimal dans le tableau opérande */
    /* Ce n'est pas le mieux car on remet la valeur en hexadécimal par la suite mais c'est le plus simple à implémenter pour le moment */
    if (hexa) {
      operandesHextoDec(operandes[numOpe]);
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
    operation[j]=ligne[i];
    j++;
    i++;
  }
  operation[j]='\0';
  #ifdef DEBUG
  printf("\n%s\n", operation);
  #endif
  *offset=i;
}

/* Traduit une ligne passé en argument (*ligne) en un tableau de représentation binaire (int* bin) */
/* Retourne 0 si l'operation n'existe pas 1 sinon */
/* Prend la memoire des registres et des instructions en entrée */
int parseLigne(char *ligne, int* bin, instruction* instructions[], registre* registres[]) {
  int offset=0,offsetBin=0,ret=0;
  int registreDec=0;
  instruction *found=NULL;
  int i=0,l=0;
  char operation[TAILLE_MAX_OPERATEUR];
  char **operandes=NULL;
  /* On écrit des zéros dans le tableau de la représentation binaire */
  for (l=0;l<TAILLE_BIT_OPERATION;l++) {
    bin[l]=0;
  }
  #ifdef DEBUG
  printf("\n");
  afficheBin(bin,TAILLE_BIT_OPERATION);
  #endif
  /* On récupère l'opération */
  parseOperation(ligne,operation,&offset);
  /* On cherche l'opération dans la structure mémoire (pointeur vers structure) */
  found=trouveOperation(instructions,operation);

  /* Si l'opération à été trouvé dans la structure mémoire on continu */
  if (found!=NULL && nombreOperande(ligne)==found->nbOperande) {
    ret=1; /* On a une expression valide */
    /* Si ce n'est pas NOP on parse les operandes */
    if (!(found->typeInstruction=='R' && found->ordreBits==1 && found->styleRemplissage==2)) {
      operandes=parseOperandes(ligne,operandes,&offset);
      traduitOperandes(registres,operandes,nombreOperande(ligne));
    }
    #ifdef DEBUG
    afficheInstruction(found);
    #endif
    /* Instruction de type R */
    if (found->typeInstruction=='R') {
      if (found->ordreBits==1) {
        /* ADD/AND/XOR/OR/SLT/SUB */
        if (found->styleRemplissage==1) {
          offsetBin=6; /* Les 6 premiers zéro */
          registreDec=valeurDecimale(operandes[1]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[2]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          offsetBin+=5;
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
        /* NOP */
        else if (found->styleRemplissage==2) {
          ;
        }
        /* SLL */
        else if (found->styleRemplissage==3) {
          offsetBin=11; /* Les 11 premiers zéro */
          registreDec=valeurDecimale(operandes[1]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[2]);
          decToBinary(registreDec,&offsetBin, bin);
          /* opcode 00000 */
        }
      }
      else if (found->ordreBits==2) {
        /* ROTR */
        if (found->styleRemplissage==1) {
          offsetBin=10; /* Les 10 premiers zéro */
          bin[offsetBin]=1;
          offsetBin++;
          registreDec=valeurDecimale(operandes[1]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[2]);
          decToBinary(registreDec,&offsetBin, bin);
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
        /* SRL */
        else if (found->styleRemplissage==2) {
          offsetBin=10; /* Les 10 premiers zéro */
          offsetBin++;
          registreDec=valeurDecimale(operandes[1]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[2]);
          decToBinary(registreDec,&offsetBin, bin);
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
      }
      else if (found->ordreBits==3) {
        /* MULT */
        if (found->styleRemplissage==1) {
          offsetBin=6; /* Les 6 premiers zéro */
          /* rt/rd/0 */
          for(i=0;i<nombreOperande(ligne);i++) {
            registreDec=valeurDecimale(operandes[i]);
            decToBinary(registreDec,&offsetBin, bin);
          }
          offsetBin+=10;
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
      }
      else if (found->ordreBits==4) {
        /* JR -> Implémentation de la release 1 de l'architecture */
        if (found->styleRemplissage==1) {
          offsetBin=6; /* Les 6 premiers zéro */
          registreDec=valeurDecimale(operandes[i]);
          decToBinary(registreDec,&offsetBin, bin);
          offsetBin+=15; /* La boucle for avance de 5 de trop */
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
      }
      else if (found->ordreBits==5) {
        /* MFHI/MFLO */
        if (found->styleRemplissage==1) {
          offsetBin=16; /* Les 16 premiers zéro */
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          offsetBin+=5; /* 5 zéro */
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
      }
      else if (found->ordreBits==6) {
        /* SYSCALL */
        if (found->styleRemplissage==1) {
          offsetBin=6; /* Les 6 premiers zéro */
          /* code */
          /*  Que faut il mettre ? */
          offsetBin+=20;
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
      }
    }
    /* Instruction de type I */
    else if (found->typeInstruction=='I') {
      if (found->ordreBits==1) {
        /* ADDI/BEQ/BNE */
        if (found->styleRemplissage==1) {
          rempliBinTabBin(found->opcode, &offsetBin, bin);
          /* ADDI change au niveau de l'instruction présente rt rs imm */
          if (valeurDecimale(found->opcode)==1000) {
            registreDec=valeurDecimale(operandes[1]);
            decToBinary(registreDec,&offsetBin, bin);
            registreDec=valeurDecimale(operandes[0]);
            decToBinary(registreDec,&offsetBin, bin);
          }
          /* rs rt imm */
          else {
            registreDec=valeurDecimale(operandes[0]);
            decToBinary(registreDec,&offsetBin, bin);
            registreDec=valeurDecimale(operandes[1]);
            decToBinary(registreDec,&offsetBin, bin);
          }
          registreDec=valeurDecimale(operandes[2]);
          decToBinaryImm(registreDec,&offsetBin, bin);
        }
        /* BGTZ/BLEZ */
        else if (found->styleRemplissage==2) {
          rempliBinTabBin(found->opcode, &offsetBin, bin);
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          offsetBin+=5; /* Laisse 5 bits à zéro */
          registreDec=valeurDecimale(operandes[1]);
          decToBinaryImm(registreDec,&offsetBin, bin);
        }
        /* LUI */
        else if (found->styleRemplissage==3) {
          rempliBinTabBin(found->opcode, &offsetBin, bin);
          offsetBin+=5; /* Laisse 5 bits à zéro */
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[1]);
          decToBinaryImm(registreDec,&offsetBin, bin);
        }
        /* LW/SW */
        else if (found->styleRemplissage==4) {
          rempliBinTabBin(found->opcode, &offsetBin, bin);
          registreDec=valeurDecimale(operandes[2]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[1]);
          decToBinaryImm(registreDec,&offsetBin, bin);
        }
      }
    }
  }
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
  char *ligne=NULL;
  /* Représentation binaire de l'instruction */
  int bin[TAILLE_BIT_OPERATION];
  /* Représentation hexadécimale de l'instruction */
  char hex[TAILLE_HEX_OPERATION];
  char *ligneOut=NULL;
  int programCounter=0,lignes=1;
  /* Fichiers pour remplir les mémoires (opérandes et registres) */
  char *listeope="src/listeOpe.txt";
  char *listereg="src/listeReg.txt";
  char c;
  int inW=1;
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
  fclose(fout); /* On referme le fichier */
  while(getline(&ligne,&len,fin)!=-1) { /* Tant qu'on est pas à la fin du fichier */
    if(ligne[0]!='\n' && ligne[0]!='\0') { /* Si on n'à pas une ligne vide */
      /* On uniforise la ligne */
      ligneOut=(char *)malloc(strlen(ligne)*sizeof(char));
      uniformisationInstruction(ligne,ligneOut);
      if(ligneOut[0]!='\0') { /* Si la ligne uniformisé n'est pas vide */
         /* On a quelque chose */
         if (mode) {
           if (parseLigne(ligneOut,bin,instructions,registres)) {
             /* On parse la ligne */
            binaryToHex(bin,hex,TAILLE_BIT_OPERATION); /* On transforme en hexadécimal */
            ecrireHex(hex,output); /* On écrit la valeur hexadécimale */
            fprintf(tmp,"%08d 0x%s   %s\n",programCounter,hex,ligneOut);
            programCounter+=4;
          }
           else if (mode==0) {printf("Erreur ligne %d, on passe à la suivante (opération non reconnue)\n",lignes);}
         }
         if (!mode) {
           if (parseLigne(ligneOut,bin,instructions,registres)) {
            printf("%s\n", ligneOut);
            binaryToHex(bin,hex,TAILLE_BIT_OPERATION); /* On transforme en hexadécimal */
            printf("Instruction assembleur ligne %d : \n%s\n\n",lignes,ligneOut);
            printf("Expression hexadécimale : \n%s\n\n", hex);
            printf("passer l'instruction: [p], instruction suivante: [enter], saut de la lecture: [s]\n");
            do {
              c=getchar();
              inW=1;
              if (c=='s' || c=='\n') {
                ecrireHex(hex,output);
                fprintf(tmp,"%08d 0x%s   %s\n",programCounter,hex,ligneOut);
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
  /* Affichage final */

  char buf[1000];
  tmp=fopen(".tmp","r");
  if (tmp) {
    printf("\n---- Assembleur ----\n\nPC       Hex          Instruction\n------------------------------------------\n");
    while (fgets(buf,1000,tmp)!=NULL) {
      printf("%s",buf);
    }
  }
  else {
    printf("Pas d'affichage\n");
  }
  fclose(tmp);
  remove(".tmp");
  fclose(fin); /* On ferme le fichier d'entrée */
}
