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
		printf("Table vide!") ;
	else
		printf("Contenu de la table : ") ;
	while (NULL!=ptr) 	{
		printf("%s %i\n",ptr->nom,ptr->valeur);
		ptr=ptr->suivant ;
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
