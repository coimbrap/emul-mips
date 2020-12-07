#ifndef _TABLE_H_
#define _TABLE_H_

typedef struct symbol {
  char *nom;
  int valeur;
  struct symbol* suivant;
} symbol;

typedef symbol* symtable;

/* prend en entrée la table des symboles, le nom du symbole et la valeur du PC */
/* insère en tête le symbole dans la table des symboles */
void insertionTete(symtable *tableorigine, char* nom, int valeur);

/* prend en entrée la table des symboles, le nom du symbole et la valeur du PC */
/* insère en queue le symbole dans la table des symboles */
void insertionQueue(symtable *tableorigine, char* nom, int valeur);

/* prend en entrée la table des symboles */
/* efface le premier élément de la liste */
void supressionTete(symtable *tableorigine);

/* prend en entrée la table des symboles */
/* affiche l'ensemble de la table des symboles dans l'ordre */
void tableAffiche(symtable symbols);

/* prend en entrée la table des symboles */
/* libère l'ensemble de la table des symboles pour éviter les fuites mémoire */
void libereTable(symtable *symbols);

/* prend en entrée la table des symboles ainsi qu'un symbole à identifier */
/* retourne le PC associé à ce symbole si non trouvé -1 */
int foundSymbol(symtable *symbols, char* symbol);

#endif
