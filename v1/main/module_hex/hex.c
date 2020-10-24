#include "hex.h"
#include <string.h>

void remplissageStructInstruction(instruction *instructions[], const char* fichier) {
  FILE *fs=fopen(fichier,"r");
  instruction *tmp=NULL;
  int i=0;
  if(fs==NULL) {
    perror("Erreur d'ouverture");
  }
  fseek(fs,0,SEEK_SET);

  for(i=0;i<NB_OPERATIONS;i++) {
    instructions[i]=malloc(sizeof(instruction));
    tmp=instructions[i];
    fscanf(fs,"%[^,],%d,%c,%d,%d*",(char *) &tmp->nom,&tmp->opcode,&tmp->typeInstruction,&tmp->ordreBits,&tmp->styleRemplissage);
    fgetc(fs); /* Enlève \n */
  }
  fclose(fs);
}

void afficheStructInstruction(instruction *instructions[]) {
  int i=0;
  instruction tmp;
  for(i=0;i<NB_OPERATIONS;i++) {
    tmp=*instructions[i];
    printf("Case %i\n", i);
    printf("Nom : %s\n", tmp.nom);
    printf("opcode : %d\n", tmp.opcode);
    printf("Type d'instruction : %c\n", tmp.typeInstruction);
    printf("Ordre bits : %d\n", tmp.ordreBits);
    printf("Style de remplissage : %d\n", tmp.styleRemplissage);
    printf("\n");
  }
}

void afficheBin(int* bin) {
  int i=0;
  for (i=0;i<TAILLE_BIT_OPERATION;i++) {
    printf("%d",bin[i]);
  }
  printf("\n");
}

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

int valeurDecimale(char *s) {
  int num=0,i=0;
  while (s[i]>='0' && s[i]<='9'){
    num=num*10+(s[i]-'0');
    i++;
  }
  return num;
}

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

void rempliBinTabBin(char* cBin, int offset, int* bin) {
  int i=0,j=offset;
  while (cBin[i]!='\0') {
    bin[j]=cBin[i]-'0';
    i++;
    j++;
  }
}

void parseR(char *ope, int offset) {
  int binIncrement=0;
  char bufferRegiste[TAILLE_MAX_REGISTE+1];
  int nbOperande=0;
  int i=offset,j=0,k=0;
  nbOperande=nombreOperande(ope);
  binIncrement+=6; /* Champ opcode */
  printf("On doit trouver %d opérandes\n", nbOperande);
  for(j=0;j<nbOperande;j++) {
    k=0;
    while(ope[i]!=',' && ope[i]!='\0') {
      if (ope[i]>='0' && ope[i]<='9') {
        bufferRegiste[k]=ope[i];
        k++;
      }
      i++;
    }
    i++;
    bufferRegiste[k]='\0';
    printf("%s\n", bufferRegiste);
  }
}

void parseOperation(char *ope) {
  int i=0,j=0;
  /* Taille max 5 */
  char operation[TAILLE_MAX_OPERATEUR];
  while(ope[i]!='$' && ope[i]!='\0') {
    operation[j]=ope[i];
    j++;
    i++;
  }
  operation[j]='\0';
  printf("%s\n", operation);
  parseR(ope,i);
}

void parseFichier(char *nomFichier) {
  FILE *fp = fopen(nomFichier, "r");
  size_t len=0;
  char *line=NULL;
  if (fp==NULL) {
    perror("Erreur lors de l'ouverture du fichier");
  }
  while(getline(&line,&len,fp)!=-1) {
    line=enleveEspaces(line);
    line=enleveCommentaires(line);
    if(line[0]!='\n' && line[0]!='\0') {
      /* On a quelque chose */
      printf("Final : %s\n",line);
      parseOperation(line);
    }
  }
  fclose(fp);
}
