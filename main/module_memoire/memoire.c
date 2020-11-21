#include "memoire.h"
#include <stdio.h>
#include <stdlib.h>

/* FONCTIONS PRINCIPALE */

/* prend en entrée un pointeur vers la mémoire, une adresse et un mot à écrire */
/* écrit dans le mémoire à l'adresse donnée le mot donné en argument */
/* si il y a déjà une valeur on la remplace */
/* on garde la liste triéée par adresse */
/* vérifie que l'adresse et bien celle du premier octet d'un mot */
void insertion(int adresse, long int mot, memoire *m) {
	element *elem=NULL;
	memoire increment=*m;
  /* On vérifie que l'on soit bien en tête de mot */
  if (!(adresse&3)) {
		/* On malloc un nouvel élément et on l'initialise */
		if((elem=(element *)malloc(sizeof(element)))==NULL){exit(1);};
  	elem->adresse=adresse;
    elem->valeur=mot;
  	/* Cas d'une mémoire vide ou d'une insertion en tête */
  	if((increment==NULL) || (increment->adresse>adresse)) {
  		elem->suivant=*m;
  		*m=elem;
  	}
		/* Insertion au millieu ou on queue */
    else {
			/* On avance dans la liste tant que l'on n'a pas atteint la fin ou la place de notre adresse */
  		while (increment->suivant!=NULL && (adresse)>((increment->suivant)->adresse)) {
  			increment=increment->suivant; /* On avance d'un élément */
  		}
  		/* On rechaîne en faisant attention au doublon, en cas de doubons on écrase */
      if (increment->adresse==adresse) { /* Pour le premier élément */
        free(elem); /* On libère elem car non nécessaire */
        increment->valeur=mot; /* On remplace la valeur */
      }
      else if (increment->suivant!=NULL && (increment->suivant)->adresse==adresse) { /* Pour tout les autres */
        increment=increment->suivant; /* On avance */
				free(elem); /* On libère elem car non nécessaire */
        increment->valeur=mot;
      }
      else { /* Si c'est un nouvel élément on l'insère et on rechaîne*/
        elem->suivant=increment->suivant;
        increment->suivant=elem;
      }
  	}
  }
  else {printf("Vous essayez d'écrire au millieu d'un mot 0x%04x\n",adresse);}
}


/* prend en entrée un pointeur vers la mémoire et une adresse */
/* supprime la case mémoire associé à l'adresse */
/* vérifie que l'adresse et bien celle du premier octet d'un mot */
void suppression(int adresse, memoire *m) {
	memoire increment=NULL;
	element *precedent=NULL;
  if (!(adresse&3)) {
  	if (*m==NULL) {printf("Suppression : mémoire vide\n");}
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
          precedent->suivant=increment->suivant; /* On rechaîne en sautant l'élément à supprimé */
          free(increment); /* On libère l'élément a supprimer */
        }
				/* Sinon on supprime le premier élément et on fait pointé *m vers le second */
        else {
          *m=increment->suivant;
          free(increment);
        }
      }
  	}
  }
  else {printf("Vous essayez de supprimer au millieu d'un mot 0x%04x\n",adresse);}
}

/* prend en entrée un pointeur vers la mémoire et une adresse */
/* va chercher la valeur en mémoire correspondant à cette adresse et la retourne */
/* si elle n'est pas présente retourne 0 */
/* vérifie que l'adresse et bien celle du premier octet d'un mot */
long int valeurMemoire(int adresse, memoire *m) {
  memoire increment=NULL;
  long int binMem=0;
  /* Si l'adresse est divisible par 4 */
  if (!(adresse&3)) {
  	if (*m==NULL) {printf("Valeur : mémoire vide\n");}
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
			/* Si l'adresse de l'increment est l'adresse recherché on met le valeur dans binMem */
      if (increment->adresse==adresse) {
        binMem=increment->valeur;
      }
  	}
  }
  else {printf("Vous essayez de lire au millieu d'un mot 0x%04x\n",adresse);}
  return binMem;
}

/* prend en entrée un pointeur vers la mémoire */
/* libère toute les cases mémoire dans l'ordre de parcourt pour éviter les fuites mémoire */
/* *m pointera donc vers NULL à la fin */
void liberation(memoire *m) {
  memoire increment=NULL,precedent=NULL;
  if (*m==NULL) {printf("Libération : mémoire vide\n");}
  else {
    increment=*m;
		/* Tant que le prochain élément ne pas NULL */
    while (increment->suivant!=NULL) {
			/* On mémorise l'élément actuel, on avance d'un élément et on libère l'élément */
      precedent=increment;
      increment=increment->suivant;
      suppression(precedent->adresse,m);
    }
    /* On supprime le dernier */
    suppression(increment->adresse,m);
  }
}

/* AFFICHAGE */

/* prend en entrée un pointeur vers la mémoire une adresse de début et une de fin de lecture */
/* si la mémoire est vide affiche une message d'avertissement */
/* sinon affiche toute le mémoire entre adresseMin inclu et adresseMax exclu*/
void afficherMemoires(memoire *m, int adresseMin, int adresseMax) {
	memoire increment=*m;
	int one=0; /* passe à un si on affiche quelque chose */
  printf("\nAdresse     Décimal      Hex          Binaire\n--------------------------------------------------------------------------\n");
	/* Sinon tant que l'on est pas en fin de liste et que l'adresse est inférieure à l'adresse max */
  while(increment!=NULL && increment->adresse<adresseMax) {
		/* Si l'adresse et supérieure à l'adresse min on affiche et on passe one à 1 */
		if ((increment->adresse>=adresseMin)) {
    	afficherMemoire(increment);
			one=1;
		}
		increment=increment->suivant; /* On avance d'une case mémoire */
  }
	if (one) {printf("\n");} /* Si on à écrit quelque chose on ajoute un \n */
	else {printf("No record\n");} /* Sinon on affiche un avertissement */
  }
}

/* prend en entré un slot de la mémoire */
/* affiche l'adresse en hexa ainsi que les représentation décimal hexadécimale et binaire de la valeur en mémoire */
void afficherMemoire(memoire slot) {
  if(slot==NULL) {printf("No record\n");}
  else {
    printf("0x%04x      %-10ld   0x%08lx   ",slot->adresse,slot->valeur,slot->valeur);
    decToBin(slot->valeur);
  }
}
