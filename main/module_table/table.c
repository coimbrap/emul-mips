#include "table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


void insertionTete(symtable *tableorigine, char* nom, int valeur) {
	symtable head=malloc(sizeof(symbol));
  head->nom=nom;
  head->suivant=*tableorigine;
	head->valeur=valeur;
	*tableorigine=head;
}

void insertionQueue(symtable *tableorigine, char* nom, int valeur) {
	symtable increment=*tableorigine;
	if(increment==NULL) {
		insertionTete(tableorigine,nom,valeur);
	}
	else {
		while(increment->suivant!=NULL) {
			increment=increment->suivant;
		}
		increment->suivant=malloc(sizeof(symbol));
		increment=increment->suivant;
    increment->nom=nom;
		increment->valeur=valeur;
		increment->suivant=NULL;
	}
}

void supressionTete(symtable *tableorigine) {
	if(*tableorigine!=NULL) {
		symtable tablewithouthead=(*tableorigine)->suivant;
    free((*tableorigine)->nom);
		free(*tableorigine);
		*tableorigine=tablewithouthead;
	}
}

void tableAffiche(symtable ptr){
	if (NULL==ptr)
		printf("Table vide!");
	else
		printf("Contenu de la table : ");
	while (NULL!=ptr) {
		printf("%s %i\n",ptr->nom,ptr->valeur);
		ptr=ptr->suivant;
	}
}

void libereTable(symtable *symbols) {
  symtable increment=NULL;
  if (*symbols==NULL) {printf("Libération : Table des symboles vide\n");}
  else {
    increment=*symbols;
    while (increment->suivant!=NULL) {
      increment=increment->suivant;
      supressionTete(symbols);
    }
    supressionTete(symbols);
  }
}

int foundSymbol(symtable *symbols, char* symbol) {
	symtable increment=NULL;
	int nonTrouvee=1,ret=-1;
	increment=*symbols;
	if (*symbols==NULL) {printf("Recherche : Table des symboles vide\n");}
	else if (increment->suivant==NULL && (strcmp(increment->nom,symbol)==0)) {
		printf("Trouvé dans la table des symboles %s\n",increment->nom);
		ret=increment->valeur;
		nonTrouvee=0;
	}
	else {
		while (increment->suivant!=NULL && nonTrouvee) {
			if (strcmp(increment->nom,symbol)==0) {
				printf("Trouvé dans la table des symboles %s\n",increment->nom);
				ret=increment->valeur;
				nonTrouvee=0;
			}
			increment=increment->suivant;
		}
	}
	return ret;
}
