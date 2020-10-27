#include "hex.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Module tools */
#include "../module_tools/tools.h"

/* MEMOIRE INSTRUCTIONS */

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
    fscanf(fs,"%[^,],%[^,],%c,%d,%d*",tmp->nom,tmp->opcode,&tmp->typeInstruction,&tmp->ordreBits,&tmp->styleRemplissage);
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

/* UNIFORMISATION DE L'INSTRUCTION */

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
void binaryToHex(int* bin, char* hex){
  int i=0, j=0, k=0, l=0;
  int temp1=0;
  char temp2=0;
  int pui=3;
  for(i=0;i<TAILLE_BIT_OPERATION;i+=8){
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
}

/* Ecrit la valeur hexadécimale dans un fichier */
void ecrireHex(char* hex, char *fichier){
  int i=0;
  FILE *fout=NULL;
  fout=fopen(fichier,"r+");

  if(NULL==fout){
  printf("Erreur d'ouverture du fichier\n");
  exit(1);
  }
  fseek(fout, 0, SEEK_END);
  for(i=0;i<TAILLE_HEX_OPERATION-1;i++){
    fprintf(fout,"%c", hex[i]);
  }
  fprintf(fout,"%c\n", hex[TAILLE_HEX_OPERATION-1]);
  fclose(fout);
}

/* PARSSAGE DE L'INSTRUCTION */

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

/* Retourne un tableau de string contenant la valeur de tout les operandes */
char** parseOperandes(char *ligne, char* operandes[], int* offset) {
  int nbOperande=0;
  int i=*offset,j=0,k=0,numOpe=0;
  nbOperande=nombreOperande(ligne);
  if((operandes=malloc(sizeof(char*)*nbOperande))==NULL){exit(1);};
  for(j=0;j<nbOperande;j++) {
    if((operandes[j]=malloc(sizeof(char*)))==NULL){exit(1);};
  }
  for(j=0;j<nbOperande;j++) {
    k=0;
    while(ligne[i]!='(' && ligne[i]!=',' && ligne[i]!='\0') {
      if (ligne[i]>='0' && ligne[i]<='9') {
          operandes[numOpe][k]=ligne[i];
        k++;
      }
      operandes[numOpe][k]='\0';
      i++;
    }
    i++;
    numOpe++;
    *offset=i;
  }
  return operandes;
}

/* Stocke dans operation l'opération assembleur de l'instruction (ADD...) */
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

void parseLigne(char *ligne, int* bin) {
  int offset=0,offsetBin=0;
  int registreDec=0;
  instruction *found=NULL;
  int i=0,l=0;
  char *listeope="module_hex/listeOpe.txt";
  instruction *instructions[NB_OPERATIONS+1];
  char operation[TAILLE_MAX_OPERATEUR];
  char **operandes=NULL;
/*  int bin[TAILLE_BIT_OPERATION];*/
  for (l=0;l<TAILLE_BIT_OPERATION;l++) {
    bin[l]=0;
  }
  #ifdef DEBUG
  printf("\n");
  afficheBin(bin);
  #endif
  /* On remplit la structure de stockage à partir du fichier */
  remplissageStructInstruction(instructions,listeope);
  /* Pour éviter des offset complexe on renversera le tableau à la fin */
  parseOperation(ligne,operation,&offset);
  found=trouveOperation(instructions,operation);
  if (found!=NULL) {
    #ifdef DEBUG
    afficheInstruction(found);
    #endif
    if (found->typeInstruction=='R') {
      if (found->ordreBits==1) {
        if (found->styleRemplissage==1) {
          offsetBin=6; /* Les 6 premiers zéro */
          operandes=parseOperandes(ligne,operandes,&offset);
          /* rs/rt/rd */
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[2]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[1]);
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
          operandes=parseOperandes(ligne,operandes,&offset);
          /* rt/rd/sa */
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
        if (found->styleRemplissage==1) {
          offsetBin=10; /* Les 10 premiers zéro */
          offsetBin++;/* R1 ?? */
          operandes=parseOperandes(ligne,operandes,&offset);
          /* rt/rd/sa */
          registreDec=valeurDecimale(operandes[1]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[2]);
          decToBinary(registreDec,&offsetBin, bin);
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
        else if (found->styleRemplissage==2) {
          offsetBin=10; /* Les 10 premiers zéro */
          offsetBin++;/* R0 ?? */
          operandes=parseOperandes(ligne,operandes,&offset);
          /* rt/rd/sa */
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
        if (found->styleRemplissage==1) {
          offsetBin=6; /* Les 6 premiers zéro */
          operandes=parseOperandes(ligne,operandes,&offset);
          /* rt/rd/sa */
          for(i=0;i<nombreOperande(ligne);i++) {
            registreDec=valeurDecimale(operandes[i]);
            decToBinary(registreDec,&offsetBin, bin);
          }
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
      }
      /* Non fonctionnel */
      else if (found->ordreBits==4) {
        if (found->styleRemplissage==1) {
          offsetBin=6; /* Les 6 premiers zéro */
          operandes=parseOperandes(ligne,operandes,&offset);
          /* rt/rd/sa */
          for(i=0;i<nombreOperande(ligne);i++) {
            registreDec=valeurDecimale(operandes[i]);
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
          operandes=parseOperandes(ligne,operandes,&offset);
          /* 0/rd/0 */
          for(i=0;i<nombreOperande(ligne);i++) {
            registreDec=valeurDecimale(operandes[i]);
            decToBinary(registreDec,&offsetBin, bin);
          }
          offsetBin+=5; /* 5 zéro */
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
      }
      else if (found->ordreBits==6) {
        if (found->styleRemplissage==1) {
          offsetBin=6; /* Les 6 premiers zéro */
          operandes=parseOperandes(ligne,operandes,&offset);
          /* code */
          for(i=0;i<nombreOperande(ligne);i++) {
            registreDec=valeurDecimale(operandes[i]);
            decToBinary(registreDec,&offsetBin, bin);
          }
          rempliBinTabBin(found->opcode, &offsetBin, bin);
        }
      }
    }
    else if (found->typeInstruction=='I') {
      if (found->ordreBits==1) {
        if (found->styleRemplissage==1) {
          rempliBinTabBin(found->opcode, &offsetBin, bin);
          operandes=parseOperandes(ligne,operandes,&offset);
          /* rs rt imm */
          registreDec=valeurDecimale(operandes[1]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[2]);
          decToBinary(registreDec,&offsetBin, bin);
        }
        else if (found->styleRemplissage==2) {
          rempliBinTabBin(found->opcode, &offsetBin, bin);
          operandes=parseOperandes(ligne,operandes,&offset);
          /* rs 0 offset */
          for(i=0;i<nombreOperande(ligne);i++) {
            registreDec=valeurDecimale(operandes[i]);
            decToBinary(registreDec,&offsetBin, bin);
            offsetBin+=5; /* Laisse 5 bits à zéro */
          }
        }
        else if (found->styleRemplissage==3) {
          rempliBinTabBin(found->opcode, &offsetBin, bin);
          operandes=parseOperandes(ligne,operandes,&offset);
          /* 0 rt offset */
          offsetBin+=5; /* Laisse 5 bits à zéro */
          for(i=0;i<nombreOperande(ligne);i++) {
            registreDec=valeurDecimale(operandes[i]);
            decToBinary(registreDec,&offsetBin, bin);
          }
        }
        else if (found->styleRemplissage==4) {
          rempliBinTabBin(found->opcode, &offsetBin, bin);
          operandes=parseOperandes(ligne,operandes,&offset);
          /* base rt offset */
          registreDec=valeurDecimale(operandes[2]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[0]);
          decToBinary(registreDec,&offsetBin, bin);
          registreDec=valeurDecimale(operandes[1]);
          decToBinary(registreDec,&offsetBin, bin);
        }
      }
    }
    #ifdef DEBUG
    printf("----Little Endian----\n");
    afficheBin(bin);
    inverseTab(bin,TAILLE_BIT_OPERATION);
    printf("----Big Endian----\n");
    afficheBin(bin);
    #endif
  }
}

/* Lit le fichier d'instruction assembleur ligne par ligne */
/* Parse l'expression puis lance le remplissage du tableau binaire */
void parseFichier(const char *nomFichier) {
  FILE *fp = fopen(nomFichier, "r");
  size_t len=0;
  char *ligne=NULL;
  char *fichierHex="output/hex.txt";
  int bin[TAILLE_BIT_OPERATION];
  char hex[TAILLE_HEX_OPERATION];
  if (fp==NULL) {
    perror("Erreur lors de l'ouverture du fichier");
    exit(1);
  }
  while(getline(&ligne,&len,fp)!=-1) {
    if(ligne[0]!='\n' && ligne[0]!='\0') {
      /* On a quelque chose */
      #ifdef VERBEUX
      printf("\n----Instruction----\n%s",ligne);
      #endif
      parseLigne(ligne,bin);
      #ifdef VERBEUX
      printf("------Binaire------\n");
      afficheBin(bin);
      #endif
      binaryToHex(bin,hex);
      #ifdef VERBEUX
      printf("----Hexadécimal----\n");
      afficheHex(hex);
      #endif
      ecrireHex(hex,fichierHex);
    }
  }
  fclose(fp);
}
