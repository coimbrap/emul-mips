#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAILLE_MAX_OPERATEUR 5
#define TAILLE_MAX_REGISTE 2
#define TAILLE_BIT_OPERATION 32

void afficheBin(int* bin);
char* enleveEspaces(char *s);
char* enleveCommentaires(char *s);
int nombreOperande(char *s);
int valeurDecimale(char *s);
void decToBinary(int n, int offset, int* bin);
void parseR(char *ope, int offset);
void parseOperation(char *ope);
void parseFichier(char *nomFichier);
