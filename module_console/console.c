#include "console.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* prend en entrée la liste chaîné des segments, le program counteur la version hexa et le string de l'instruction uniforisé */
/* ajoute cette instruction à la liste chaîné */
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

/* prend en entrée la liste chaîné des segments et le program counteur */
/* affiche l'ensemble des segments assembleur contenu dans la liste chainée */
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

/* prend en entrée la liste chaîné des segments, le program counteur et le type */
/* type=1 : pc,hex,char type=0 : char */
/* affiche l'ensemble des segments assembleur contenu dans la liste chainée */
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

/* prend en entrée la liste chaîné des segments */
/* libère l'ensemble de la liste chainée pour éviter les fuites mémoire */
void libereSegments(prog *segments) {
  prog increment=NULL;
  while (*segments!=NULL) {
    increment=(*segments)->suivant;
    free((*segments)->asem);
    free(*segments);
    *segments=increment;
  }
}

/* prend en entrée le fichier d'entrée, le fichier de sortie (fichier d'entrée pas forcément utilisé) */
/* ainsi que les structures de stockage des instructions, des registres, la mémoire, les segments pour l'afficage, la table des symboles */
/* ET le Mode: 0: pàp, 1: auto et 2: intéractif */
/* parse ligne après ligne le fichier et s'occupe de tout l'affichage et des saisie utilisateur */
void parseFichier(char *input, char* output, int mode, instruction **instructions, registre** registres, memoire *mem, prog *segments, symtable *symbols) {
  /* Fichiers d'entrée, de sortie du programme et d'affichage */
  FILE *fin=NULL,*fout=fopen(output, "w");
  size_t len=0;
  char *ligne=NULL,*ligneOut=NULL,*line=NULL; /* Ligne brute & uniformisée & buffer de lecture */
  unsigned long int instructionHex=0; /* Valeur hexadécimale de l'instruction */
  int lignes=1,pcMax=0,tmpMode=mode,retParse=0,saisie=1,inW=1;
  char c='0';
  unsigned long int instruction=0;
  registre *pc=NULL;
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
        /* Si mode automatique */
        if (tmpMode) {
          retParse=hexLigne(ligne,pc->valeur,&ligneOut,&instructionHex,symbols,instructions,registres);
          /* Ligne correcte on stocke les infos dans la structure d'affichage */
          if (retParse==1) {
            insererSegment(segments,pc->valeur,instructionHex,ligneOut);
            insertion(mem,pc->valeur,instructionHex);
            fprintf(fout,"%08lx\n",instructionHex);
            (pc->valeur)+=4;
          }
          else {free(ligneOut);} /* On a un label ou une erreur */
        }
        /* Si mode pas à pas */
        else if (!tmpMode) {
          retParse=hexLigne(ligne,pc->valeur,&ligneOut,&instructionHex,symbols,instructions,registres);
          if (retParse==1) {
            printf("Instruction assembleur ligne %d : \n%s\n\n",lignes,ligneOut);
            printf("Expression hexadécimale : \n0x%08lx\n\n", instructionHex);
            printf("passer l'instruction: [p], instruction suivante: [enter], saut de la lecture: [s]\n");
            do { /* Saisie de l'utilisateur en boucle */
              c=getchar();
              inW=1;
              if (c=='s' || c=='\n') { /* On stocke dans la structure d'affichage */
                insererSegment(segments,pc->valeur,instructionHex,ligneOut);
                insertion(mem,pc->valeur,instructionHex);
                fprintf(fout,"%08lx\n",instructionHex);
                (pc->valeur)+=4;
                inW=0;
                if (c=='s') {
                  tmpMode=1;
                }
              }
              else {clean_stdin();} /* On clean stdin pour le tour suivant */
              if (c=='p') {
                free(ligneOut);
                inW=0;
              }

            } while(inW);
          }
          else {free(ligneOut);} /* Label ou erreur */
        }
        else {free(ligneOut);}
      }
      lignes++;
    }
    fclose(fin);
    free(ligne);
  }
  /* Sinon mode intéractif exécution et lecture en simultané */
  else if(mode==2) {
    /* On écrit l'instruction assembleur au clavier jusqu'a EXIT */
    printf("Saisissez un instruction ou EXIT : \n");
    int exec=0;
    while(saisie) {
      if(getline(&line,&len,stdin)>0) {
        exec=0;
        if(line[0]!='\0' && line[0]!='\n') { /* Si la ligne uniformisé n'est pas vide */
          /* On a quelque chose */
          retParse=hexLigne(line,pc->valeur,&ligneOut,&instructionHex,symbols,instructions,registres);
          if(strcmp(line,"EXIT\n")==0) {
            inW=0;
            saisie=0;
            free(ligneOut);
          }
          else if (retParse==1) {
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
                printf("Saisissez un instruction ou EXIT : \n");
                inW=0;
              }
            } while(inW);
            (pc->valeur)+=4;
          }
          else {
            free(ligneOut);
            ligneOut=NULL;
            printf("Saisissez un instruction ou EXIT : \n\n");
          }
        }
        else {
          if(ligneOut!=NULL) {free(ligneOut);};
        }
      }
    }
    free(line);
    /* Une fois la structure remplit on passe à l'éxécution */
  }
  fclose(fout);
  /* On exécute le programme si mode automatique ou pas à pas */
  if (mode!=2) {
    printf("\nFormes hexadécimale écrites dans '%s'\n", output);
    printf("\n---- Assembleur ----\n\n");
    afficherSegments(segments,-1);
    printf("\n----- Exécution du programme -----\n\n");
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
  }
  printf("\n------ Registres ------\n");
  afficheRegistres(registres);
  printf("\n------- Mémoire -------\n");
  afficherMemoires(mem,DEBUT_MEMOIRE,FIN_MEM); /* On affiche toute la mémoire */
  libereSegments(segments);
}
