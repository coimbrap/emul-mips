#include "table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../module_hex/hex.h"
#include "../module_registres/registres.h"

/* prend en entrée la table des symboles, le nom du symbole et la valeur du PC */
/* insère en tête le symbole dans la table des symboles */
void insertionTete(symtable *tableorigine, char* nom, int valeur) {
	symtable head=malloc(sizeof(symbol));
	head->nom=malloc((strlen(nom)+1)*sizeof(char));
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
    increment->nom=malloc((strlen(nom)+1)*sizeof(char));
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
	if (NULL==symbols)
		printf("Table vide!");
	else
		printf("Contenu de la table : ");
	while (NULL!=symbols) {
		printf("%s %i\n",symbols->nom,symbols->valeur);
		symbols=symbols->suivant;
	}
}

/* prend en entrée la table des symboles */
/* libère l'ensemble de la table des symboles pour éviter les fuites mémoire */
void libereTable(symtable *symbols) {
  symtable increment=NULL;
  if (*symbols!=NULL) {
    increment=*symbols;
    while (increment->suivant!=NULL) {
      increment=increment->suivant;
      supressionTete(symbols);
    }
    supressionTete(symbols);
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

/* prend en entrée le fichier du programme, la structure des instruction, des registres et la table des symboles */
/* on écrit par adresse */
/* remplis la table des symboles et doit pour cela vérifié si les instructions sont correcte (parse + checksum) pour que la valeur du PC soit correcte */
void fillSymbols(char *input, instruction **instructions, registre** registres, symtable *symbols) {
  int nbOpe=0,nbReg=0,nbImm=0,num=0,coeffOpe=0;
  char *p=NULL,*parse=NULL,*tampon=NULL,*s=NULL; /* tampon va être la zone de travail de strtok pour pouvoir utiliser strtok dans deux fonctions */
  int ret=1;
	FILE *fin=NULL;
	int pc=0xDDDC;
	instruction *instr=NULL;
	size_t len=0;
	int tailleTab=0;
	fin=fopen(input, "r");
  if (fin==NULL) {
    printf("Erreur lors de l'ouverture du fichier '%s'\n",input);
    exit(-1);
  }
	/* on parcourt tout le fichier ligne par ligne */
	while(getline(&s,&len,fin)!=-1) {
		ret=1;
		nbReg=nbImm=nbOpe=num=coeffOpe=0;
		parse=malloc(strlen(s)*sizeof(char));
	  int state=nettoyageInstruction(s,&parse,&tailleTab,&nbOpe);
		/* même principe que pour le parseur */
	  if (state==0) {
	    free(parse);
	    ret=0;
	  }
	  else if (state==1) {
	    ret=1;
			insertionQueue(symbols,parse,pc);
			free(parse);
	  }
	  else {
	    /* On sépare l'instruction avec l'espace comme séparateur */
	    for (p=strtok_r(parse," ",&tampon);p!=NULL;p=strtok_r(NULL, " ",&tampon)) {
	      /* Ecriture */
	      if (num==0 && nbOpe>=0) {
	        instr=trouveOperation(instructions,p);
	        if(instr==NULL) {ret=0;}
	      }
	      /* Calcul de notre checksum */
	      if (p[0]=='$') {
	        if(!traduitRegistre(registres,p)) {ret=0;};
	        nbReg+=2<<coeffOpe++; /* On ajoute la puissance de deux du numéro de l'opérande */
	      }
	      else if (isdigit(p[0]) || p[0]=='-') {
	        nbImm+=2<<coeffOpe++;
	      }
	      else if (num>0 && isalpha(p[0])) {
	        nbImm+=2<<coeffOpe++;
	      }
	      /* Si il y a une parenthèse ouvrante dans l'opérande */
	      if((p=strchr(p,'('))!=NULL) {
	        p++; /* On va au caractère d'après */
	        if (p[0]=='$') {
	          if(!traduitRegistre(registres,p)) {ret=0;};
	          nbReg+=2<<coeffOpe++;
	        }
	        else if (isdigit(p[0]) || p[0]=='-') {
	          nbImm+=2<<coeffOpe++;
	        }
	      }
	      num++;
	    }
	    /* Comparaison du checksum avec le checksum théorique */
	    if (ret!=0) {
	      if((compareChecksum(nbReg,instr->checksumReg,2)==0 || compareChecksum(nbImm,instr->checksumImm,1)==0)){ret=0;};
			}
			if (ret==1) {pc+=4;}
			free(parse);
			free(p);
	  }
	}
	free(s);
}
