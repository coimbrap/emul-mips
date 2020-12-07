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

/* type=1 : pc,hex,char type=0 : char */
void afficherSegmentPc(prog *segments, int pc, int type) {
  prog increment=*segments;
  int c=1;
  if (increment==NULL) {
    printf("Segments vide\n");
  }
  else {
    while(increment!=NULL && c) {
      if(increment->pc==pc) {
        if(type) {printf("%08d 0x%08lx   %s\n",increment->pc,increment->hex,increment->asem);}
        else if(type==0) {printf("%s\n",increment->asem);}
        c=0;
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
void parseFichier(char *input, char* output, int mode, instruction **instructions, registre** registres, symtable *symbols, memoire *mem, prog *segments) {
  /* Fichiers d'entrée, de sortie du programme et d'affichage */
  FILE *fin=NULL,*fout=fopen(output, "w");
  size_t len=0;
  char *ligne=NULL,*ligneOut=NULL,*line=NULL; /* Ligne brute & uniformisée & buffer de lecture */
  unsigned long int instructionHex=0; /* Valeur hexadécimale de l'instruction */
  int lignes=1,pcMax=0,tmpMode=mode,retParse=0,saisie=1,inW=1;
  char c='0';
  unsigned long int instruction=0;
  registre *pc=NULL,*sp=NULL;
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
    fillSymbols(input,instructions,registres,symbols);
    while(getline(&ligne,&len,fin)!=-1) { /* Tant qu'on est pas à la fin du fichier */
      if(ligne[0]!='\n' && ligne[0]!='\0' && ligne[0]!='#') { /* Si on n'à pas une ligne vide */
        /* On a quelque chose */
        if (tmpMode) {
          retParse=parseLigne(ligne,pc->valeur,&ligneOut,&instructionHex,symbols,instructions,registres);
          if (retParse==1) {
            insererSegment(segments,pc->valeur,instructionHex,ligneOut);
            insertion(mem,pc->valeur,instructionHex);
            fprintf(fout,"%08lx\n",instructionHex);
            (pc->valeur)+=4;
          }
          else if (retParse==10) {
            printf("On a un label %ld \n", pc->valeur);
            free(ligneOut);
          }
          else {
            printf("Erreur ligne %d, on passe à la suivante (instruction ou argument non reconnue) %s\n",lignes,ligne);
            free(ligneOut);
          }
        }
        /* Mode pas à pas */
        else if (!tmpMode) {
          retParse=parseLigne(ligne,pc->valeur,&ligneOut,&instructionHex,symbols,instructions,registres);
          if (retParse==1) {
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
                }
              }
              else {clean_stdin();}
              if (c=='p') {
                free(ligneOut);
                inW=0;
              }
            } while(inW);
          }
          else if (retParse==10) {
            printf("On a un label %ld \n", pc->valeur);
          }
          else {
            printf("Erreur ligne %d, on passe à la suivante (instruction ou argument non reconnue) %s\n\n",lignes,ligne);
            free(ligneOut);
          }
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
    printf("Saisissez un instruction ou EXIT : \n\n");
    int exec=0;
    while(saisie) {
      if(getline(&line,&len,stdin)>0) {
        exec=0;
        if(line[0]!='\0' && line[0]!='\n') { /* Si la ligne uniformisé n'est pas vide */
          /* On a quelque chose */
          retParse=parseLigne(line,pc->valeur,&ligneOut,&instructionHex,symbols,instructions,registres);
          if (retParse==1) {
            insererSegment(segments,pc->valeur,instructionHex,ligneOut);
            insertion(mem,pc->valeur,instructionHex);
            fprintf(fout,"%08lx\n",instructionHex);
            afficherSegmentPc(segments,pc->valeur,1);
            printf("registres: [r], memoire: [m], programme: [p], executer [enter]\n");
            do {
              c=getchar();
              inW=1;
              if (c=='\n' && !exec) {
                instruction=valeurMemoire(mem,pc->valeur); /* On récupère la valeur de l'instruction en mémoire */
                printf("--> Exécution de l'instruction : ");
                printf("\nregistres: [r], memoire: [m], programme: [p], saisie [i]\n");
                afficherSegmentPc(segments,pc->valeur,0);
                execInstruction(instruction,registres,instructions,mem); /* Exécute l'opération, s'occupe d'incrémenter le PC */
                exec=1;
              }
              else {clean_stdin();}
              if (c=='p') {
                afficherSegments(segments,pc->valeur);
                printf("\nregistres: [r], memoire: [m], programme: [p], saisie [i], executer [enter]\n");
              }
              if (c=='m') {
                afficherMemoires(mem,DEBUT_MEMOIRE,DEBUT_PROG);
                printf("\nregistres: [r], memoire: [m], programme: [p], saisie [i], executer [enter]\n");
              }
              if (c=='r') {
                afficheRegistres(registres);
                printf("\nregistres: [r], memoire: [m], programme: [p], saisie [i], executer [enter]\n");
              }
              if (c=='i' && exec) {
                printf("Saisissez un instruction ou EXIT : \n\n");
                inW=0;
              }
            } while(inW);
            (pc->valeur)+=4;
          }
          else {
            free(ligneOut);
            printf("Saisissez un instruction ou EXIT : \n\n");
          }
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
  clean_stdin();
  while((pc->valeur)<pcMax) {
    if (mode==1) {
      instruction=valeurMemoire(mem,pc->valeur); /* On récupère la valeur de l'instruction en mémoire */
      printf("Exécution de l'instruction :\n");
      afficherSegmentPc(segments,pc->valeur,1);
      execInstruction(instruction,registres,instructions,mem); /* Exécute l'opération, s'occupe d'incrémenter le PC */
    }
    else if (mode==0) {
      afficherSegmentPc(segments,pc->valeur,1);
      printf("registres: [r], memoire: [m], programme: [p], continuer [enter]\n");
      do {
        c=getchar();
        inW=1;
        if (c=='\n') {
          instruction=valeurMemoire(mem,pc->valeur); /* On récupère la valeur de l'instruction en mémoire */
          printf("--> Exécution de l'instruction : ");
          afficherSegmentPc(segments,pc->valeur,0);
          execInstruction(instruction,registres,instructions,mem); /* Exécute l'opération, s'occupe d'incrémenter le PC */
          inW=0;
        }
        else {clean_stdin();}
        if (c=='p') {
          afficherSegments(segments,pc->valeur);
          printf("\nregistres: [r], memoire: [m], programme: [p], continuer [enter]\n");
        }
        if (c=='m') {
          afficherMemoires(mem,DEBUT_MEMOIRE,DEBUT_PROG);
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
  afficherMemoires(mem,DEBUT_MEMOIRE,DEBUT_PROG);
  libereSegments(segments);
  printf("Fin\n");
}
