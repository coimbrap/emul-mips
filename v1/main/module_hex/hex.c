#include "hex.h"

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

void parseR(char *ope, int offset) {
  int bin[TAILLE_BIT_OPERATION];
  int binIncrement=0;
  char bufferRegiste[TAILLE_MAX_REGISTE+1];
  int nbOperande=0;
  int i=offset,j=0,k=0,l=0;
  nbOperande=nombreOperande(ope);
  for (l=0;l<TAILLE_BIT_OPERATION;l++) {
    bin[l]=0;
  }
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
  int i=0,j=0,nbOperande;
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
    return -1;
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
