#include "memoire.h"
#include <stdio.h>
#include <stdlib.h>

int estDivisiblePar4(int n) {
  return !(n&3);
}

void afficherMemoires(memoire *m) {
	memoire increment=*m;
  int decReg=0;
  char hex[8]; /* Buffer */
  printf("\nAdresse     Décimal      Hex          Binaire\n--------------------------------------------------------------------------\n");
  if(increment==NULL) {
    printf("No record\n");
  }
  else {
    while(increment!=NULL) {
      afficherMemoire(increment);
      increment=increment->suivant;
    }
		printf("\n");
  }
}

void afficherMemoire(memoire slot) {
  int decReg=0;
  char hex[8]; /* Buffer */
  if(slot==NULL) {
    printf("No record\n");
  }
  else {
    printf("0x%04x      %-10ld   0x%08lx   ",slot->adresse,slot->valeur,slot->valeur);
    afficheBin(decToBin(slot->valeur,NB_BIT_MEMOIRE),NB_BIT_MEMOIRE);
  }
}

long int valeurMemoire(int adresse, memoire *m) {
  memoire increment=NULL;
  long int binMem=0;
  if (estDivisiblePar4(adresse)) {
  	if (*m==NULL) {
  		printf("Valeur : mémoire vide\n");
  	}
  	/* Il reste un unique élément dans la mémoire est c'est celui à supprimer */
  	else if(((*m)->suivant==NULL) && ((*m)->adresse)==adresse) {
  		binMem=(*m)->valeur;
  	}
  	else {
  		increment=*m; /* On utilise un incrément pour ne pas modifier *m */
  		/* On parcourt la mémoire tant quelle n'est pas fini et que l'indice n'est pas atteint */
  		while (increment->suivant!=NULL && ((increment)->adresse)<(adresse)) {
  			increment=increment->suivant; /* On avance d'un élément */
  		}
      if (increment->adresse==adresse) {
        binMem=increment->valeur;
      }
  	}
  }
  else {
    printf("Vous essayez de lire au millieu d'un mot 0x%04x\n",adresse);
  }
  return binMem;
}

void insertion(int adresse, long int mot, memoire *m) {
	element *elem=malloc(sizeof(element));
	memoire increment=*m;
  int i=0;
  /* On vérifie que l'on soit bien en tête de mot */
  if (estDivisiblePar4(adresse)) {
  	elem->adresse=adresse;
    elem->valeur=mot;
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
        increment->valeur=mot;
      }
      else if (increment->suivant!=NULL && (increment->suivant)->adresse==adresse) { /* Pour tout les autres */
        increment=increment->suivant;
        free(elem);
        increment->valeur=mot;
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
