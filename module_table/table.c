#include "table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../module_hex/hex.h"


/* prend en entrée la table des symboles, le nom du symbole et la valeur du PC */
/* insère en tête le symbole dans la table des symboles */
void insertionTete(symtable *tableorigine, char* nom, int valeur) {
	symtable head=malloc(sizeof(symbol));
	head->nom=malloc(strlen(nom)+1);
	strcpy(head->nom,nom);
  head->suivant=*tableorigine;
	head->valeur=valeur;
	*tableorigine=head;
}

/* prend en entrée la table des symboles, le nom du symbole et la valeur du PC */
/* insère en queue le symbole dans la table des symboles */
void insertionQueue(symtable *tableorigine, char* nom, int valeur) {
	/* Si la liste chaînée est vide on insère en tête */
	if((*tableorigine)==NULL) {
		insertionTete(tableorigine,nom,valeur);
	}
	else {
		symtable increment=*tableorigine;
		while(increment->suivant!=NULL) {
			increment=increment->suivant;
		}
		/* On malloc le nouvel élément */
		increment->suivant=malloc(sizeof(symbol));
		increment=increment->suivant;
		/* On malloc une place pour le nom et on le strcpy */
    increment->nom=malloc(strlen(nom)+1);
		strcpy(increment->nom,nom);
		increment->valeur=valeur;
		increment->suivant=NULL;
	}
}

/* prend en entrée la table des symboles */
/* efface le premier élément de la liste */
void supressionTete(symtable *tableorigine) {
	if(*tableorigine!=NULL) {
		symtable tablewithouthead=(*tableorigine)->suivant;
    free((*tableorigine)->nom);
		free(*tableorigine);
		*tableorigine=tablewithouthead;
	}
}

/* prend en entrée la table des symboles */
/* affiche l'ensemble de la table des symboles dans l'ordre */
void tableAffiche(symtable symbols) {
	if (NULL!=symbols) {
		printf("Contenu de la table : ");
		while (NULL!=symbols) {
			printf("%s %i\n",symbols->nom,symbols->valeur);
			symbols=symbols->suivant;
		}
	}
}

/* prend en entrée la table des symboles */
/* libère l'ensemble de la table des symboles pour éviter les fuites mémoire */
void libereTable(symtable *symbols) {
  if (*symbols!=NULL) {
    while ((*symbols)->suivant!=NULL) {
      supressionTete(symbols);
    }
  }
}

/* prend en entrée la table des symboles ainsi qu'un symbole à identifier */
/* retourne le PC associé à ce symbole si non trouvé -1 */
int foundSymbol(symtable *symbols, char* symbol) {
	symtable increment=NULL;
	int nonTrouvee=1,ret=-1;
	increment=*symbols;
	if (*symbols!=NULL) {
		if (increment->suivant==NULL && (strcmp(increment->nom,symbol)==0)) {
			ret=increment->valeur;
			nonTrouvee=0;
		}
		else {
			while (increment->suivant!=NULL && nonTrouvee) {
				if (strcmp(increment->nom,symbol)==0) {
					ret=increment->valeur;
					nonTrouvee=0;
				}
				increment=increment->suivant;
			}
		}
	}
	return ret;
}
