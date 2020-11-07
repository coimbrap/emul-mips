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
  insertion(0x4,2,&mem);
  afficherMemoire(&mem);
  insertion(0x0,1,&mem);
  afficherMemoire(&mem);
  /* Insertion en queue */
  insertion(0xffec,4,&mem);
  afficherMemoire(&mem);
  insertion(0xfffc,7,&mem);
  afficherMemoire(&mem);
  /* Insertion au millieu */
  insertion(0xfff8,6,&mem);
  afficherMemoire(&mem);
  insertion(0xfff4,5,&mem);
  afficherMemoire(&mem);
  insertion(0x101c,3,&mem);
  afficherMemoire(&mem);
  /* Test doublons */
  insertion(0xfffc,17,&mem);
  afficherMemoire(&mem);
  insertion(0x0,10,&mem);
  afficherMemoire(&mem);
  insertion(0xffec,14,&mem);
  afficherMemoire(&mem);
  /* Suppression en tête */
  suppression(0x0000,&mem);
  afficherMemoire(&mem);
  /* Suppression en queue */
  suppression(0xfffc,&mem);
  afficherMemoire(&mem);
  /* Suppression millieu */
  suppression(0xffec,&mem);
  afficherMemoire(&mem);
  /* Test millieu mot */
  suppression(0xeeee,&mem);
  afficherMemoire(&mem);
  /* Vidage de la liste */
  suppression(0x0004,&mem);
  afficherMemoire(&mem);
  suppression(0xfff4,&mem);
  afficherMemoire(&mem);
  suppression(0xfff8,&mem);
  afficherMemoire(&mem);
  suppression(0x101c,&mem);
  afficherMemoire(&mem);
  /* Un de trop existant */
  suppression(0xfff8,&mem);
  afficherMemoire(&mem);
  /* Test libération */
  liberation(&mem);
  insertion(0x4,2,&mem);
  insertion(0x0,1,&mem);
  insertion(0xffec,4,&mem);
  insertion(0xfffc,7,&mem);
  insertion(0xfff8,6,&mem);
  insertion(0xfff4,5,&mem);
  insertion(0x101c,3,&mem);
  afficherMemoire(&mem);
  liberation(&mem);
  afficherMemoire(&mem);
  return 0;
}
