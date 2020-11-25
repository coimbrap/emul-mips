#include "console.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Lit le fichier d'instruction assembleur ligne par ligne */
/* Parse l'expression et appele la fonction de traduction hexadécimale */
/* Mode défini si on est en mode auto (1) ou pàp (0) */
/* Prend les fichiers input et output, la memoire des registres et des instructions en entrée */

void insererSegment(prog *segments, int programCounter, unsigned long int tradHex, char* instruction) {
  prog increment=*segments;
  segment *elem=NULL;
  if ((elem=malloc(sizeof(segment)))==NULL) {exit(-1);};
  elem->pc=programCounter;
  elem->hex=tradHex;
  elem->asem=instruction;
  elem->suivant=NULL;
  if ((*segments)==NULL) {
    *segments=elem;
    (*segments)->suivant=NULL;
  }
  else {
		while(increment->suivant!=NULL) {
			increment=increment->suivant;
		}
		increment->suivant=elem;
  }
}

void afficherSegments(prog *segments, int pc) {
  prog increment=*segments;
  if (increment==NULL) {
    printf("Segments vide\n");
  }
  else {
    if (pc!=-1) {printf("    ");};
    printf("PC       Hex          Instruction\n------------------------------------------\n");
    while(increment!=NULL) {
      if (pc!=-1) {
        if (increment->pc!=pc) {
          printf("    ");
        }
        else {
          printf("--> ");
        }
      }
      printf("%08d 0x%08lx   %s\n",increment->pc,increment->hex,increment->asem);
      increment=increment->suivant;
    }
  }
}

void afficherSegmentPc(prog *segments, int pc) {
  prog increment=*segments;
  if (increment==NULL) {
    printf("Segments vide\n");
  }
  else {
    while(increment!=NULL) {
      if(increment->pc==pc) {
        printf("%08d 0x%08lx   %s\n",increment->pc,increment->hex,increment->asem);
      }
      increment=increment->suivant;
    }
    printf("\n");
  }
}

void libereSegments(prog *segments) {
  prog increment=NULL;
  while (*segments!=NULL) {
    increment=(*segments)->suivant;
    free((*segments)->asem);
    free(*segments);
    *segments=increment;
  }
}

/* Mode: 0: pàp, 1: auto et 2: intéractif */
void parseFichier(char *input, char* output, int mode, instruction **instructions, registre** registres, memoire *mem, prog *segments) {
  /* Fichiers d'entrée, de sortie du programme et d'affichage */
  FILE *fin=NULL,*fout=fopen(output, "w");
  /* Fichiers pour remplir les mémoires (opérandes et registres) */
  size_t len=0;
  char *ligne=NULL,*ligneOut=NULL; /* Ligne brute & uniformisée & buffer de lecture */
  unsigned long int instructionHex=0; /* Valeur hexadécimale de l'instruction */
  int lignes=1;
  char c='0';
  int inW=1;
  unsigned long int instruction=0;
  registre *pc=NULL,*sp=NULL;
  int pcMax=0;
  int tmpMode=mode;
  pc=registres[32]; /* PC */
  pc->valeur=DEBUT_PROG; /* Initialisation du PC */
  /* Initialisation du fichier (suppression du contenu ou création) */
  if (fout==NULL) {
    printf("Erreur lors de l'ouverture du fichier '%s'\n",output);
    exit(-1);
  }
  /* Si lecture de fichier */
  if(tmpMode==1 || tmpMode==0) {
    fin=fopen(input, "r");
    if (fin==NULL) {
      printf("Erreur lors de l'ouverture du fichier '%s'\n",input);
      exit(-1);
    }
    printf("Lecture du fichier : %s\n\n", input);
    while(getline(&ligne,&len,fin)!=-1) { /* Tant qu'on est pas à la fin du fichier */
      if(ligne[0]!='\n' && ligne[0]!='\0') { /* Si on n'à pas une ligne vide */
        /* On uniforise la ligne */
        if((ligneOut=(char *)calloc(strlen(ligne),sizeof(char)))==NULL){exit(1);};
        uniformisationInstruction(ligne,ligneOut);
        if(ligneOut[0]!='\0') { /* Si la ligne uniformisé n'est pas vide */
          /* On a quelque chose */
          if (tmpMode) {
            if (parseLigne(ligneOut,&instructionHex,instructions,registres)) {
              insererSegment(segments,pc->valeur,instructionHex,ligneOut);
              insertion(mem,pc->valeur,instructionHex);
              fprintf(fout,"%08lx\n",instructionHex);
              (pc->valeur)+=4;
            }
            else {
              free(ligneOut);
              printf("Erreur ligne %d, on passe à la suivante (opération non reconnue)\n",lignes);
            }
          }
          /* Mode pas à pas */
          else if (!tmpMode) {
            if (parseLigne(ligneOut,&instructionHex,instructions,registres)) {
              printf("Instruction assembleur ligne %d : \n%s\n\n",lignes,ligneOut);
              printf("Expression hexadécimale : \n0x%08lx\n\n", instructionHex);
              printf("passer l'instruction: [p], instruction suivante: [enter], saut de la lecture: [s]\n");
              do {
                c=getchar();
                inW=1;
                if (c=='s' || c=='\n') {
                  insererSegment(segments,pc->valeur,instructionHex,ligneOut);
                  insertion(mem,pc->valeur,instructionHex);
                  fprintf(fout,"%08lx\n",instructionHex);
                  (pc->valeur)+=4;
                  inW=0;
                  if (c=='s') {
                    tmpMode=1;
                /*    clean_stdin(); */
                  }
                }
                else {clean_stdin();}
                if (c=='p') {
                  free(ligneOut);
                  inW=0;
                }
              } while(inW);
            }
            else {
              free(ligneOut);
              printf("Erreur ligne %d, on passe à la suivante (opération non reconnue)\n\n",lignes);
            }
          }
          else {free(ligneOut);}
        }
        else {free(ligneOut);}
      }
      lignes++;
    }
    fclose(fin);
    free(ligne);
  }
  /* Sinon mode intéractif */
  else if(mode==2) {
    /* On écrit l'instruction assembleur au clavier jusqu'a EXIT */
    int saisie=1;
    char *line=NULL;
    while(saisie) {
        if(getline(&line,&len,stdin)>0) {
        if((ligneOut=(char *)calloc(strlen(line),sizeof(char)))==NULL){exit(1);};
        uniformisationInstruction(line,ligneOut);
        if(ligneOut[0]!='\0') { /* Si la ligne uniformisé n'est pas vide */
          /* On a quelque chose */
          if (parseLigne(ligneOut,&instructionHex,instructions,registres)) {
            insererSegment(segments,pc->valeur,instructionHex,ligneOut);
            insertion(mem,pc->valeur,instructionHex);
            fprintf(fout,"%08lx\n",instructionHex);
            (pc->valeur)+=4;
          }
          else {free(ligneOut);}
        }
        else {free(ligneOut);}
        if(strcmp(line,"EXIT\n")==0) {
          saisie=0;
        }
      }
    }
    free(line);
    /* Une fois la structure remplit on passe à l'éxécution */
  }
  fclose(fout);
  printf("\nFormes hexadécimale écrites dans '%s'\n", output);
  printf("\n---- Assembleur ----\n\n");
  afficherSegments(segments,-1);
  printf("\n----- Exécution du programme -----\n\n");
  /* Init du registre SP */
  sp=registres[29];
  (sp->valeur)=DEBUT_PILE;
  pcMax=pc->valeur; /* On mémorise le pcMAx */
  pc->valeur=DEBUT_PROG;
  /* Tant qu'on à pas atteint la dernière instruction */
  while((pc->valeur)<pcMax) {
    if (mode==1) {
      instruction=valeurMemoire(mem,pc->valeur); /* On récupère la valeur de l'instruction en mémoire */
      printf("Exécution de l'instruction :\n");
      afficherSegmentPc(segments,pc->valeur);
      execInstruction(instruction,registres,instructions,mem); /* Exécute l'opération, s'occupe d'incrémenter le PC */
    }
    else if (mode==0 || mode==2) {
      afficherSegmentPc(segments,pc->valeur);
      printf("registres: [r], memoire: [m], programme: [p], continuer [enter]\n");
      do {
        c=getchar();
        inW=1;
        if (c=='\n') {
          instruction=valeurMemoire(mem,pc->valeur); /* On récupère la valeur de l'instruction en mémoire */
          printf("Exécution de l'instruction :\n");
          afficherSegmentPc(segments,pc->valeur);
          execInstruction(instruction,registres,instructions,mem); /* Exécute l'opération, s'occupe d'incrémenter le PC */
          inW=0;
        }
        else {clean_stdin();}
        if (c=='p') {
          afficherSegments(segments,pc->valeur);
          printf("\nregistres: [r], memoire: [m], programme: [p], continuer [enter]\n");
        }
        if (c=='m') {
          afficherMemoires(mem,DEBUT_PILE,DEBUT_PROG);
          printf("\nregistres: [r], memoire: [m], programme: [p], continuer [enter]\n");
        }
        if (c=='r') {
          afficheRegistres(registres);
          printf("\nregistres: [r], memoire: [m], programme: [p], continuer [enter]\n");
        }
      } while(inW);
    }
  }
  printf("\n------ Registres ------\n");
  afficheRegistres(registres);
  printf("\n------- Pile -------\n");
  afficherMemoires(mem,DEBUT_PILE,DEBUT_PROG);
  printf("\n------- Prog -------\n");
  afficherMemoires(mem,DEBUT_PROG,0xFFFF);
}
