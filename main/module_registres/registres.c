#include "registres.h"
#include "../module_tools/tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* MEMOIRE REGISTRES */

/* prend en entrée un tableau de pointeur vers la structure registre et le nom du fichier source de remplissage */
/* remplit le tableau à l'aide du fichier source de remplissage */
/* on part du principe que le fichier source est correctement renseigné */
void remplissageStructRegistre(registre **registres, const char* fichier) {
  FILE *freg=fopen(fichier,"r");
  registre *tmp=NULL;
  int i=0;
  if(freg==NULL) {
    printf("Erreur lors de l'ouverture du fichier");
    exit(-1);
  }
  fseek(freg,0,SEEK_SET); /* On se place au début du fichier */
  /* On parcourt le tableau */
  for(i=0;i<NB_REGISTRE;i++) {
    /* On malloc la structure de mémoire d'un registre et on la met dans le tableau */
    if((registres[i]=malloc(sizeof(registre)))==NULL){exit(1);};
    tmp=registres[i];
    /* On remplit la mémoire du registre à partir du fichier */
    fscanf(freg,"%d,%[^\n]",&tmp->numero,tmp->nom);
    tmp->valeur=INIT_VALUE; /* On initialise la valeur du registre */
    fgetc(freg); /* Enlève \n */
  }
  fclose(freg);
}

/* prend en entrée un tableau de pointeur vers la structure registre et une chaine représentant le nom du registre */
/* retourne un pointeur vers la structure contenant toutes les informations du registre en question */
registre* trouveRegistre(registre **registres, char* nom) {
  int i=0, nonTrouvee=1, special=1,find=-2;
  registre *ret=NULL;
  /* Si le registre n'est pas un registre spécial (PC,HI,LO ayant comme valeur -1) */
  if (valeurDecimale(nom)!=-1) {
    special=0;
    find=valeurDecimale(nom); /* on stocke la valeur décimal de la chaine dans find */
  }
  /* Tant que l'on n'a pas trouvé et que l'on est pas à la fin du tableau */
  while (nonTrouvee && i<NB_REGISTRE) {
    /* Si c'est un registre spécial (PC,HI,LO) on compare le nom avec la chaine */
    if (special && strcmp(registres[i]->nom,nom)==0) {
      ret=registres[i];
      nonTrouvee=0;
    }
    /* Sinon on compare le numéro du registre avec le numéro contenu dans la chaine */
    else if (!special && find==registres[i]->numero) {
      ret=registres[i];
      nonTrouvee=0;
    }
    i++;
  }
  return ret;
}

/* prend en entrée un tableau de pointeur vers la structure registre et une chaine représentant le nom du registre */
/* retourne la valeur de la case mémoire sous forme de unsigned long int */
unsigned long int valeurRegistre(registre **registres, char* nom) {
  int i=0, nonTrouvee=1, special=1,find=-2;
  registre *ret=NULL;
  traduitRegistre(registres,nom); /* traduit le registre */
  /* Si le registre n'est pas un registre spécial (PC,HI,LO ayant comme valeur -1) */
  if (valeurDecimale(nom)!=-1) {
    special=0;
    find=valeurDecimale(nom);
  }
  /* Tant que l'on n'a pas trouvé et que l'on est pas à la fin du tableau */
  while (nonTrouvee && i<NB_REGISTRE) {
    /* Si c'est un registre spécial (PC,HI,LO) on compare le nom avec la chaine */
    if (special && strcmp(registres[i]->nom,nom)==0) {
      ret=registres[i];
      nonTrouvee=0;
    }
    /* Sinon on compare le numéro du registre avec le numéro contenu dans la chaine */
    else if (!special && find==registres[i]->numero) {
      ret=registres[i];
      nonTrouvee=0;
    }
    i++;
  }
  return ret->valeur;
}

/* prend en entrée un tableau de pointeur vers la structure registre et une chaine représentant le nom du registre */
/* si il y a lieu remplace le mnémonique du registre par sa valeur entière dans nom */
void traduitRegistre(registre **registres, char* nom) {
  char *ret=NULL;
  registre *found=NULL;
  /* Si on a une registre non traduit */
  if (valeurDecimale(nom)==-1) {
    /* Allocation de la chaine pour la représentation de l'int */
    if((ret=(char *)calloc(TAILLE_MAX_INT,sizeof(char)))==NULL){exit(1);};
    found=trouveRegistre(registres,nom);
    /* Si on trouve le registre on met dans ret la valeur décimale du registre */
    if (found!=NULL) {
      ret=intVersChaine(found->numero,ret);
    }
    /* Si la valeur décimale dans ret n'est pas -1 on remplace la mnémonique par la valeur entière */
    if (valeurDecimale(ret)!=-1) {
      strcpy(nom,ret);
    }
    free(ret); /* On libère ret */
  }
}

/* prend en entrée un tableau de pointeur vers la structure registre */
/* libère chaque case du tableau pour éviter les fuites en mémoires */
void liberationRegistres(registre** registres) {
  int i=0;
  for (i=0;i<NB_REGISTRE;i++) {
    free(registres[i]);
  }
}

/* AFFICHAGE */

/* prend en entrée une structure d'un registre */
/* affiche la valeur décimale (sauf pour les registres spéciaux) ainsi que la mnémonique */
/* affiche aussi la valeur sous forme décimale signé, hexadécimale et binaire */
void afficheRegistre(registre *registre) {
  if (strcmp(registre->nom,"zero")==0) {
    printf("$%d ($%s)  %-11d   0x%08x   ",registre->numero,registre->nom,(int)registre->valeur,(int)registre->valeur);
  }
  else if (registre->numero==-1) {
    printf("$%s         %-11d   0x%08x   ",registre->nom,(int)registre->valeur,(int)registre->valeur);

  }
  else if (registre->numero<10) {
    printf("$%d ($%s)    %-11d   0x%08x   ",registre->numero,registre->nom,(int)registre->valeur,(int)registre->valeur);
  }
  else {
    printf("$%d ($%s)   %-11d   0x%08x   ",registre->numero,registre->nom,(int)registre->valeur,(int)registre->valeur);
  }
  decToBin(registre->valeur);
}

/* prend en entrée un tableau de pointeur vers la structure registre */
/* affiche toutes les structures du tableau */
void afficheRegistres(registre **registres) {
  int i=0;
  printf("\nRegistre    Décimal      Hex          Binaire\n--------------------------------------------------------------------------\n");
  for(i=0;i<NB_REGISTRE;i++) {
    afficheRegistre(registres[i]);
  }
}
