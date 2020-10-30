#include "module_registres/registres.h"
#include <stdio.h>

int main() {
  registre* registres[NB_REGISTRE];
  registre* found=NULL;
  char* nom="s0";
  int valeur[NB_BIT_REGISTRE]={1,0,1,1,1,0,1,1,0,0,1,1,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,1,1};
  remplissageStructRegiste(registres,"module_registres/listeReg.txt");
  afficheRegistres(registres);
  printf("\n------------------------------------\nTest recheche et écriture structure\n------------------------------------\n");
  printf("On cherche %s\n", nom);
  found=trouveRegistre(registres,nom);
  if (found!=NULL) {
    afficheRegistre(found);
  }
  nom="HI";
  printf("\nOn cherche %s\n", nom);
  found=trouveRegistre(registres,nom);
  if (found!=NULL) {
    afficheRegistre(found);
  }
  nom="12";
  printf("\nOn cherche %s\n", nom);
  found=trouveRegistre(registres,nom);
  if (found!=NULL) {
    afficheRegistre(found);
    changeRegistre(found,valeur);
    afficheRegistre(found);
  }
  nom="-1";
  printf("\nOn cherche %s\n", nom);
  found=trouveRegistre(registres,nom);
  if (found!=NULL) {
    afficheRegistre(found);
  }
  return 0;
}
