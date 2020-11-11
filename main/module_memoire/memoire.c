#include "memoire.h"
#include <stdio.h>
#include <stdlib.h>

int estDivisiblePar4(int n) {
  return !(n&3);
}

void afficherMemoire(memoire *m) {
	memoire increment=*m;
  long int decReg=0;
  char hex[8]; /* Buffer */
  printf("\nAdresse     Décimal      Hex          Binaire\n--------------------------------------------------------------------------\n");
  if(increment==NULL) {
    printf("No record\n");
  }
  else {
    while(increment!=NULL) {
      binaryToHex(increment->valeur,hex);
      decReg=hexToDec(hex);
      printf("0x%04x      %-10ld   0x%08lx   ",increment->adresse,decReg,decReg);
      afficheBin(increment->valeur,NB_BIT_MEMOIRE);
      increment=increment->suivant;
    }
		printf("\n");
  }
}

void insertion(int adresse, int mot[NB_BIT_MEMOIRE], memoire *m) {
	element *elem=malloc(sizeof(element));
	memoire increment=*m;
  int i=0;
  /* On vérifie que l'on soit bien en tête de mot */
  if (estDivisiblePar4(adresse)) {
  	elem->adresse=adresse;
    for (i=0;i<NB_BIT_MEMOIRE;i++) {
      elem->valeur[i]=mot[i];
    }
  	/* Cas d'une mémoire vide */
  	if(increment==NULL) {
  		elem->suivant=*m;
  		*m=elem;
  	}
    /* Cas d'un insertion en tête */
    else if (increment->adresse>adresse) {
      elem->suivant=*m;
      *m=elem;
    }
    else {
  		while (increment->suivant!=NULL && (adresse)>((increment->suivant)->adresse)) {
  			increment=increment->suivant; /* On avance d'un élément */
  		}
  		/* On rechaîne en faisant attention au doublon, en cas de doubons on écrase */
      if (increment->adresse==adresse) { /* Pour le premier élément */
        free(elem);
        for (i=0;i<NB_BIT_MEMOIRE;i++) {
          increment->valeur[i]=mot[i];
        }
      }
      else if (increment->suivant!=NULL && (increment->suivant)->adresse==adresse) { /* Pour tout les autres */
        increment=increment->suivant;
        free(elem);
        for (i=0;i<NB_BIT_MEMOIRE;i++) {
          increment->valeur[i]=mot[i];
        }
      }
      else { /* Si nouvel élément */
        elem->suivant=increment->suivant;
        increment->suivant=elem;
      }
  	}
  }
  else {
    printf("Vous essayez d'écrire au millieu d'un mot 0x%04x\n",adresse);
  }
}

void suppression(int adresse, memoire *m) {
	memoire increment=NULL;
	element *precedent=NULL;
  if (estDivisiblePar4(adresse)) {
  	if (*m==NULL) {
  		printf("Suppression : mémoire vide\n");
  	}
  	/* Il reste un unique élément dans la mémoire est c'est celui à supprimer */
  	else if(((*m)->suivant==NULL) && ((*m)->adresse)==adresse) {
  		/* On libère et on pointe sur NULL */
  		free(*m);
  		*m=NULL;
  	}
  	else {
  		increment=*m; /* On utilise un incrément pour ne pas modifier *m */
  		/* On parcourt la mémoire tant quelle n'est pas fini et que l'indice n'est pas atteint */
  		while (increment->suivant!=NULL && ((increment)->adresse)<(adresse)) {
  			precedent=increment; /* On garde en mémoire l'adresse de l'élément actuel */
  			increment=increment->suivant; /* On avance d'un élément */
  		}
      if (increment->adresse==adresse) {
        /* Si ce n'est pas le premier élément */
        if(*m!=increment) {
          precedent->suivant=increment->suivant; /*On saute un élément */
          free(increment); /* On libère l'élément a supprimer */
        }
        else {
          *m=increment->suivant;
          free(increment);
        }
      }
  	}
  }
  else {
    printf("Vous essayez de supprimer au millieu d'un mot 0x%04x\n",adresse);
  }
}

void liberation(memoire *m) {
  memoire increment=NULL,precedent=NULL;
  if (*m==NULL) {
    printf("Libération : mémoire vide\n");
  }
  else {
    increment=*m;
    while (increment->suivant!=NULL) {
      precedent=increment;
      increment=increment->suivant;
      suppression(precedent->adresse,m);
    }
    /* On supprime le dernier */
    suppression(increment->adresse,m);
  }
}
