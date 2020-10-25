#include "hex.h"
#include <string.h>


/* Remplit la structure de stockage à l'aide du fichier contenant les opérations */
void remplissageStructInstruction(instruction *instructions[], const char* fichier) {
  FILE *fs=fopen(fichier,"r");
  instruction *tmp=NULL;
  int i=0;
  if(fs==NULL) {
    perror("Erreur d'ouverture");
    exit(1);
  }
  fseek(fs,0,SEEK_SET);

  for(i=0;i<NB_OPERATIONS;i++) {
    instructions[i]=malloc(sizeof(instruction));
    tmp=instructions[i];
    fscanf(fs,"%[^,],%[^,],%c,%d,%d*",(char *) &tmp->nom,(char *) &tmp->opcode,&tmp->typeInstruction,&tmp->ordreBits,&tmp->styleRemplissage);
    fgetc(fs); /* Enlève \n */
  }
  fclose(fs);
}

/* Retourne un pointeur vers la structure contenant toutes les informations d'une opération */
instruction* trouveOperation(instruction* instructions[], char* nom) {
  int i=0, nonTrouvee=1;
  instruction *ret=NULL;
  while (nonTrouvee && i<NB_OPERATIONS) {
    if (strcmp(instructions[i]->nom,nom)==0) {
      ret=instructions[i];
      nonTrouvee=0;
    }
    i++;
  }
  return ret;
}

/* Affiche les informations contenu dans une structure de stockage */
void afficheInstruction(instruction *instruction) {
  printf("Nom : %s\n", instruction->nom);
  printf("opcode : %s\n", instruction->opcode);
  printf("Type d'instruction : %c\n", instruction->typeInstruction);
  printf("Ordre bits : %d\n", instruction->ordreBits);
  printf("Style de remplissage : %d\n", instruction->styleRemplissage);
  printf("\n");
}

/* Affiche toutes les structures du tableau de stockage */
void afficheStructInstruction(instruction *instructions[]) {
  int i=0;
  for(i=0;i<NB_OPERATIONS;i++) {
    printf("--%d--\n", i);
    afficheInstruction(instructions[i]);
  }
}

/* Affiche le tableau binaire de l'opération */
void afficheBin(int* bin) {
  int i=0;
  for (i=0;i<TAILLE_BIT_OPERATION;i++) {
    printf("%d",bin[i]);
  }
  printf("\n");

  for (i=TAILLE_BIT_OPERATION-1;i>=0;i--) {
    printf("%d",bin[i]);
  }
  printf("\n");
}

/* Retourne la chaine d'entrée sans espaces */
char* enleveEspaces(char *s) {
  char *ret=(char *) malloc(strlen(s)+1);
  int i,j=0;
  while(s[i]!='\n' && s[i]!='\0') {
    if(s[i]!=' ' && s[i]!='\t') {
      ret[j]=s[i];
      j++;
    }
    i++;
  }
  ret[j]='\0';
  free(s);
  return ret;
}

/* Retourne la chaine d'entrée sans commentaires */
char* enleveCommentaires(char *s) {
  char *ret=(char *) malloc(strlen(s)+1);
  int i=0;
  while(s[i]!='\n' && s[i]!='#' && s[i]!='\0') {
    ret[i]=s[i];
    i++;
  }
  ret[i]='\0';
  free(s);
  return ret;
}

/* Retourne le nombre d'opérande dans un opération passé en entrée */
int nombreOperande(char *s) {
  int ret=0,i=0;
  while(s[i]!='\0') {
    if (s[i]==',' || s[i]=='(') {
      ret++;
    }
    i++;
  }
  return ret+1;
}

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

void inverseTab(int *tab, int n) {
  int i=0,j=TAILLE_BIT_OPERATION-1,tmp=0;
  while(i<j) {
    tmp=tab[i];
    tab[i++]=tab[j];
    tab[j--]=tmp;
  }
}

/* Retourne un tableau de string contenant la valeur de tout les registres */
char** parseRegistres(char *ligne, char* registres[], int* offset) {
  int binIncrement=6; /* Taille de l'opcode */
  int nbOperande=0;
  int i=*offset,j=0,k=0,numOpe=0;
  printf("%s\n", ligne);
  nbOperande=nombreOperande(ligne);
  if((registres=malloc(sizeof(char*)*nbOperande))==NULL){exit(1);};
  for(j=0;j<nbOperande;j++) {
    if((registres[j]=malloc(sizeof(char*)))==NULL){exit(1);};
  }
  for(j=0;j<nbOperande;j++) {
    k=0;
    while(ligne[i]!='(' && ligne[i]!=',' && ligne[i]!='\0') {
      if (ligne[i]>='0' && ligne[i]<='9') {
          registres[numOpe][k]=ligne[i];
        k++;
      }
      registres[numOpe][k]='\0';
      i++;
    }
    i++;
    numOpe++;
    *offset=i;
  }
  return registres;
}


void parseOperation(char *ligne, char* operation, int* offset) {
  int i=*offset,j=0;
  while(ligne[i]!= ' ' && ligne[i]!='$' && ligne[i]!='\0') {
    operation[j]=ligne[i];
    j++;
    i++;
  }
  operation[j]='\0';
  printf("%s\n", operation);
  *offset=i;
  return (char *) operation;
}

int valeurDecimale(char* m) {
  int num=0,i=0;
  while(m[i]>='0' && m[i] <='9') {
    num=num*10+(m[i]-'0');
    i++;
  }
  return num;
}

void parseLigne(char *ligne) {
  int offset=0,offsetBin=0;
  int registreDec=0;
  instruction *found=NULL;
  int i=0,l=0;
  char *listeope="module_hex/listeOpe.txt";
  instruction *instructions[NB_OPERATIONS+1];
  remplissageStructInstruction(instructions,listeope);
  char operation[TAILLE_MAX_OPERATEUR];
  char **registres=NULL;
  int bin[TAILLE_BIT_OPERATION];
  for (l=0;l<TAILLE_BIT_OPERATION;l++) {
    bin[l]=0;
  }
  afficheBin(bin);
  /*ligne=enleveEspaces(ligne);
  ligne=enleveCommentaires(ligne);*/
  /* Pour éviter des offset complexe on retournera le tableau à la fin */
  parseOperation(ligne,(char *) &operation,&offset);
  found=trouveOperation(instructions,operation);
  if (found!=NULL) {
    afficheInstruction(found);
  }
  printf("Offset : %d|%s\n", offsetBin,found->opcode);
  if (found->typeInstruction=='R') {
    if (found->ordreBits==1) {
      if (found->styleRemplissage==1) {
        offsetBin=6; /* Les 6 premiers zéro */
        registres=parseRegistres(ligne, registres, &offset);
        /* rs/rt/rd */
        registreDec=valeurDecimale(registres[0]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[2]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[1]);
        decToBinary(registreDec,&offsetBin, bin);
        /* 5 zéro */
        offsetBin+=5;
        rempliBinTabBin(found->opcode, &offsetBin, bin);
      }
      else if (found->styleRemplissage==2) {
        ;
      }
      else if (found->styleRemplissage==3) {
        offsetBin=11; /* Les 11 premiers zéro */
        registres=parseRegistres(ligne, registres, &offset);
        /* rt/rd/sa */
        registreDec=valeurDecimale(registres[1]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[0]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[2]);
        decToBinary(registreDec,&offsetBin, bin);
        /* opcode 00000 */
      }
    }
    else if (found->ordreBits==2) {
      if (found->styleRemplissage==1) {
        offsetBin=10; /* Les 10 premiers zéro */
        offsetBin++;/* R1 ?? */
        registres=parseRegistres(ligne, registres, &offset);
        /* rt/rd/sa */
        registreDec=valeurDecimale(registres[1]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[0]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[2]);
        decToBinary(registreDec,&offsetBin, bin);
        rempliBinTabBin(found->opcode, &offsetBin, bin);
      }
      else if (found->styleRemplissage==2) {
        offsetBin=10; /* Les 10 premiers zéro */
        offsetBin++;/* R0 ?? */
        registres=parseRegistres(ligne, registres, &offset);
        /* rt/rd/sa */
        registreDec=valeurDecimale(registres[1]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[0]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[2]);
        decToBinary(registreDec,&offsetBin, bin);
        rempliBinTabBin(found->opcode, &offsetBin, bin);
      }
    }
    else if (found->ordreBits==3) {
      if (found->styleRemplissage==1) {
        offsetBin=6; /* Les 6 premiers zéro */
        registres=parseRegistres(ligne, registres, &offset);
        /* rt/rd/sa */
        for(i=0;i<nombreOperande(ligne);i++) {
          registreDec=valeurDecimale(registres[i]);
          decToBinary(registreDec,&offsetBin, bin);
        }
        rempliBinTabBin(found->opcode, &offsetBin, bin);
      }
    }
    /* Non fonctionnel */
    else if (found->ordreBits==4) {
      if (found->styleRemplissage==1) {
        offsetBin=6; /* Les 6 premiers zéro */
        registres=parseRegistres(ligne, registres, &offset);
        /* rt/rd/sa */
        for(i=0;i<nombreOperande(ligne);i++) {
          registreDec=valeurDecimale(registres[i]);
          decToBinary(registreDec,&offsetBin, bin);
          offsetBin+=5;
        }
        offsetBin-=5; /* La boucle for avance de 5 de trop */
        rempliBinTabBin(found->opcode, &offsetBin, bin);
      }
    }
    else if (found->ordreBits==5) {
      if (found->styleRemplissage==1) {
        offsetBin=11; /* Les 11 premiers zéro */
        registres=parseRegistres(ligne, registres, &offset);
        /* 0/rd/0 */
        for(i=0;i<nombreOperande(ligne);i++) {
          registreDec=valeurDecimale(registres[i]);
          decToBinary(registreDec,&offsetBin, bin);
        }
        offsetBin+=5; /* 5 zéro */
        rempliBinTabBin(found->opcode, &offsetBin, bin);
      }
    }
    else if (found->ordreBits==6) {
      if (found->styleRemplissage==1) {
        offsetBin=6; /* Les 6 premiers zéro */
        registres=parseRegistres(ligne, registres, &offset);
        /* code */
        for(i=0;i<nombreOperande(ligne);i++) {
          registreDec=valeurDecimale(registres[i]);
          decToBinary(registreDec,&offsetBin, bin);
        }
        rempliBinTabBin(found->opcode, &offsetBin, bin);
      }
    }
  }
  else if (found->typeInstruction=='I') {
    printf("On est I\n");
    if (found->ordreBits==1) {
      if (found->styleRemplissage==1) {
        rempliBinTabBin(found->opcode, &offsetBin, bin);
        registres=parseRegistres(ligne, registres, &offset);
        /* rs rt imm */
        registreDec=valeurDecimale(registres[1]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[0]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[2]);
        decToBinary(registreDec,&offsetBin, bin);
      }
      else if (found->styleRemplissage==2) {
        rempliBinTabBin(found->opcode, &offsetBin, bin);
        registres=parseRegistres(ligne, registres, &offset);
        /* rs 0 offset */
        for(i=0;i<nombreOperande(ligne);i++) {
          registreDec=valeurDecimale(registres[i]);
          decToBinary(registreDec,&offsetBin, bin);
          offsetBin+=5; /* Laisse 5 bits à zéro */
        }
      }
      else if (found->styleRemplissage==3) {
        rempliBinTabBin(found->opcode, &offsetBin, bin);
        registres=parseRegistres(ligne, registres, &offset);
        /* 0 rt offset */
        offsetBin+=5; /* Laisse 5 bits à zéro */
        for(i=0;i<nombreOperande(ligne);i++) {
          registreDec=valeurDecimale(registres[i]);
          decToBinary(registreDec,&offsetBin, bin);
        }
      }
      else if (found->styleRemplissage==4) {
        rempliBinTabBin(found->opcode, &offsetBin, bin);
        registres=parseRegistres(ligne, registres, &offset);
        /* base rt offset */
        registreDec=valeurDecimale(registres[2]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[0]);
        decToBinary(registreDec,&offsetBin, bin);
        registreDec=valeurDecimale(registres[1]);
        decToBinary(registreDec,&offsetBin, bin);
      }
    }
  }
  afficheBin(bin);
  inverseTab(bin,TAILLE_BIT_OPERATION);
  afficheBin(bin);
}

/* Lit le fichier d'instruction assembleur ligne par ligne */
/* Parse l'expression puis lance le remplissage du tableau binaire */
void parseFichier(const char *nomFichier) {
  FILE *fp = fopen(nomFichier, "r");
  size_t len=0;
  char *ligne=NULL;
  if (fp==NULL) {
    perror("Erreur lors de l'ouverture du fichier");
    exit(1);
  }
  while(getline(&ligne,&len,fp)!=-1) {
    if(ligne[0]!='\n' && ligne[0]!='\0') {
      /* On a quelque chose */
      printf("Brut :  %s\n",ligne);
      parseLigne(ligne);
    }
  }
  fclose(fp);
}
