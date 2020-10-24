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
    if (s[i]==',') {
      ret++;
    }
    i++;
  }
  return ret+1;
}

/* Ecrit à partir de l'offset (inclu) du tableau bin passé par adresse la représentation binaire de n (decimal) */
void decToBinary(int n, int offset, int* bin) {
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
  for (j=offset;i>=0;j++) {
    bin[j]=binTmp[i];
    i--;
  }
}

/* Ecrit à partir d'un nombre binaire en string dans le tableau bin à partir de l'offset inclu */
void rempliBinTabBin(char* cBin, int offset, int* bin) {
  int i=0,j=offset;
  while (cBin[i]!='\0') {
    bin[j]=cBin[i]-'0';
    i++;
    j++;
  }
}

char** parseRegistres(char *ligne, char* registres[], int* offset) {
  int binIncrement=6; /* Taille de l'opcode */
  char *bufferRegiste=malloc((TAILLE_MAX_REGISTE+1)*sizeof(char*));
  int nbOperande=0;
  int i=*offset,j=0,k=0,numOpe=0;
  nbOperande=nombreOperande(ligne);
  if((registres=malloc(sizeof(char*)*nbOperande))==NULL){exit(1);};
  for(j=0;j<nbOperande;j++) {
    if((registres[j]=malloc(sizeof(char*)))==NULL){exit(1);};
  }
  for(j=0;j<nbOperande;j++) {
    k=0;
    while(ligne[i]!=',' && ligne[i]!='\0') {
      if (ligne[i]>='0' && ligne[i]<='9') {
          registres[numOpe][k]=ligne[i];
        k++;
      }
      i++;
    }
    i++;
    printf("%s|%d\n", registres[numOpe],numOpe);
    numOpe++;
    *offset=i;
  }
  printf("\n");
  for(i=0;i<nombreOperande(ligne);i++) {
    printf("B : %s\n", registres[i]);
  }
  return registres;
}


void parseOperation(char *ligne, char* operation, int* offset) {
  int i=*offset,j=0;
  while(ligne[i]!='$' && ligne[i]!='\0') {
    operation[j]=ligne[i];
    j++;
    i++;
  }
  operation[j]='\0';
  printf("%s\n", operation);
  *offset=i;
  return (char *) operation;
}

void parseLigne(char *ligne) {
  int offset=0;
  char operation[TAILLE_MAX_OPERATEUR];
  /*ligne=enleveEspaces(ligne);
  ligne=enleveCommentaires(ligne);*/
  parseOperation(ligne,(char *) &operation,&offset);
  printf("Opération : %s|%d\n", operation,offset);

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
