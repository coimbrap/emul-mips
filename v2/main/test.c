#include "module_registres/registres.h"
#include "module_memoire/memoire.h"

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
  printf("\n------------------------------------\nTest traduction registre\n------------------------------------\n");
  nom="HI";
  nom=traduitRegiste(registres, nom);
  printf("Traduction %s\n", nom);
  memoire mem=NULL;
  printf("\n------------------------------------\nTest de la liste chaînée\n------------------------------------\n");
  /* Insertion en tête */
  afficherMemoire(&mem);
  insertion(0xffee,10,&mem);
  afficherMemoire(&mem);
  insertion(0x3,112,&mem);
 afficherMemoire(&mem);
  insertion(0x0,11,&mem);
  afficherMemoire(&mem);
  /* Insertion en queue */
  insertion(0xfffe,10,&mem);
  afficherMemoire(&mem);
  insertion(0xffff,10,&mem);
  afficherMemoire(&mem);
  /* Insertion au millieu */
  insertion(0xeeee,10,&mem);
  afficherMemoire(&mem);
  /* Test doublons */
  insertion(0xffff,9,&mem);
  afficherMemoire(&mem);
  insertion(0x0,1120,&mem);
  afficherMemoire(&mem);
  insertion(0xeeee,11320,&mem);
  afficherMemoire(&mem);
  /* Suppression en tête */
  suppression(0x0000,&mem);
  afficherMemoire(&mem);
  /* Suppression en queue */
  suppression(0xffff,&mem);
  afficherMemoire(&mem);
  /* Suppression millieu */
  suppression(0xeeee,&mem);
  afficherMemoire(&mem);
  /* Test inexistant */
  suppression(0xeeee,&mem);
  afficherMemoire(&mem);
  /* Vidage de la liste */
  suppression(0x0003,&mem);
  afficherMemoire(&mem);
  suppression(0xfffe,&mem);
  afficherMemoire(&mem);
  suppression(0xffee,&mem);
  afficherMemoire(&mem);
  /* Un de trop */
  suppression(0xffee,&mem);
  return 0;
}
