#include <stdio.h>
#include <stdlib.h>

typedef struct segment {
  int pc;
  unsigned long int hex;
  char* asm;
  struct segment* suivant;
} segment;

typedef segment* prog;

void insererSegment(prog *segments, int programCounter, unsigned long int tradHex, char* instruction) {
  prog increment=*segments;
  segment *elem=NULL;
  if ((elem=malloc(sizeof(segment)))==NULL) {exit(-1);};
  elem->pc=programCounter;
  elem->hex=tradHex;
  elem->asm=instruction;
  elem->suivant=NULL;
  if (increment==NULL) {
    elem->suivant=increment;
    increment=elem;
  }
  else {
		while(increment->suivant!=NULL) {
			increment=increment->suivant;
		}
		increment->suivant=elem;
	}
}

void afficherSegments(prog *segments) {
  prog increment=*segments;
  if (increment==NULL) {
    printf("Segments vide\n");
  }
  else {
    while(increment->suivant!=NULL) {
      printf("%08d 0x%08lx   %s\n", elem->pc,elem->hex,elem->asm);
    }
    printf("\n");
  }
}



void emul(char *input, char* output, int mode, instruction **instructions, registre** registres, memoire *m) {
  prog segments=NULL;
  /* Fichiers d'entrée, de sortie du programme et d'affichage */
  FILE *fin=fopen(input, "r"),*fout=fopen(output, "w"),*tmp=fopen(".tmp","w");
  /* Fichiers pour remplir les mémoires (opérandes et registres) */
  size_t len=0;
  char *ligne=NULL,*ligneOut=NULL,*buf=NULL; /* Ligne brute & uniformisée & buffer de lecture */
  unsigned long int instructionHex=0; /* Valeur hexadécimale de l'instruction */
  int programCounter=0,lignes=1;
  char c='0';
  int inW=1;
  programCounter=DEBUT_PROG; /* Initialisation du PC local */
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
      if((ligneOut=(char *)calloc(strlen(ligne),sizeof(char)))==NULL){exit(1);};
      uniformisationInstruction(ligne,ligneOut);
      if(ligneOut[0]!='\0') { /* Si la ligne uniformisé n'est pas vide */
        /* On a quelque chose */
        if (parseLigne(ligneOut,&instructionHex,instructions,registres)) {
          insererSegment(&segments,programCounter,instructionHex,ligneOut);
          afficherSegments(&segments);
        }
      }
    }
  }
}

void parseFichier(char *input, char* output, int mode, instruction **instructions, registre** registres, memoire *m) {
  /* Fichiers d'entrée, de sortie du programme et d'affichage */
  FILE *fin=fopen(input, "r"),*fout=fopen(output, "w"),*tmp=fopen(".tmp","w");
  /* Fichiers pour remplir les mémoires (opérandes et registres) */
  size_t len=0;
  char *ligne=NULL,*ligneOut=NULL,*buf=NULL; /* Ligne brute & uniformisée & buffer de lecture */
  unsigned long int instructionHex=0; /* Valeur hexadécimale de l'instruction */
  int programCounter=0,lignes=1;
  char c='0';
  int inW=1;
  programCounter=DEBUT_PROG; /* Initialisation du PC local */
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
      if((ligneOut=(char *)calloc(strlen(ligne),sizeof(char)))==NULL){exit(1);};
      uniformisationInstruction(ligne,ligneOut);
      if(ligneOut[0]!='\0') { /* Si la ligne uniformisé n'est pas vide */
        /* On a quelque chose */
        /* Mode automatique */
        if (mode) {
          if (parseLigne(ligneOut,&instructionHex,instructions,registres)) {
            /* On parse la ligne */
            fprintf(fout,"%08lx\n",instructionHex);
            fprintf(tmp,"%08d 0x%08lx   %s\n",programCounter,instructionHex,ligneOut);
            programCounter+=4;
          }
          else if (mode==0) {printf("Erreur ligne %d, on passe à la suivante (opération non reconnue)\n",lignes);}
         }
         /* Mode pas à pas */
         else if (!mode) {
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
       else if {
         free(ligneOut);
       }
    }
    else if (mode==0) {printf("Erreur ligne %d, on passe à la suivante (ligne vide)\n\n",lignes);}
    if (!mode) {lignes++;}
  }
  free(ligne); /* On libère le buffer de lecture */
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
  free(buf); /* On libère le buffer de lecture */
  fclose(fin); /* On ferme le fichier d'entrée */
  /* On ferme et on efface le fichier temporaire d'affichage */
  fclose(tmp);
  remove(".tmp");
}
/*
Si ça vient d'un fichier :
Chargement des segments assembleur -> Uniformisation (label et forme)
Affichage des segments chargés en début d'éxécution

Ensuite pour tout les cas :
Instruction par instruction (soit dans un fichier soit dans une stdin) -> Uniformisation,
Affichage
Si Pas à Pas et skip on écrit pas sinon on écrit l'instruction dans le fichier





*/
