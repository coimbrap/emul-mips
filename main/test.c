#include "module_hex/hex.h"
#include "module_tools/tools.h"
#include "module_registres/registres.h"
#include "module_memoire/memoire.h"
#include <string.h>
#include <stdio.h>

int main() {
  /* Partie 1 */
  int l=0,offsetBin=0;
  char *binNum="11101001";
  char *nomFichier="tests_emul/test.txt";
  char *listeope="src/listeOpe.txt";
  char *nom="ADD";
  char *fichierHex="output/hex.txt";
  char *ligne="SLL $10,$23,24";
  instruction *instructions[NB_OPERATIONS+1];
  instruction *found=NULL;
  int bin[TAILLE_BIT_OPERATION];
  int binParse[TAILLE_BIT_OPERATION];
  int binHex[TAILLE_BIT_OPERATION]={0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0};
  int binHex2[TAILLE_BIT_OPERATION]={0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0};
  char hex[TAILLE_HEX_OPERATION]={0,0,0,0,0,0,0};
  /* Partie 2 */
  registre* registres[NB_REGISTRE];
  registre* found=NULL;
  char* nom="s0";
  int valeur[NB_BIT_REGISTRE]={1,0,1,1,1,0,1,1,0,0,1,1,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,1,1};

  /* Tests partie 1 */
  for (l=0;l<TAILLE_BIT_OPERATION;l++) {
    bin[l]=0;
  }
  printf("\n----------------------------------\nTest Remplissage Structure\n----------------------------------\n");
  remplissageStructInstruction(instructions,listeope);
  afficheStructInstruction(instructions);
  printf("\n----------------------------------\nTest recheche Structure\n----------------------------------\n");
  printf("On cherche %s\n", nom);
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }
  nom="JUL";
  printf("On cherche %s\n", nom);
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }
  nom="JAL";
  printf("On cherche %s\n", nom);
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }
  nom="SYSCALL";
  printf("On cherche %s\n", nom);
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }
  nom="SLL";
  printf("On cherche %s\n", nom);
  found=trouveOperation(instructions,nom);
  if (found!=NULL) {
    afficheInstruction(found);
  }
  printf("\n----------------------------------\nTest binTabBin\n----------------------------------\n");
  offsetBin=8;
  rempliBinTabBin(binNum,&offsetBin,bin);
  afficheBin(bin,TAILLE_BIT_OPERATION);
  printf("\n----------------------------------\nTest Dec2Bin\n----------------------------------\n");
  offsetBin=3; /*Offset de 3*/
  decToBinary(6,&offsetBin,bin);
  afficheBin(bin,TAILLE_BIT_OPERATION);
  printf("\n----------------------------------\nTest parseur ligne\n----------------------------------\n");
  ligne="SLL $10,$23,24";
  printf("%s\n", ligne);
  parseLigne(ligne,binParse,instructions);
  afficheBin(binParse,TAILLE_BIT_OPERATION);
  printf("\n------------------------------\nTest binToHex + Fichier\n------------------------------\n");
  binaryToHex(binHex,hex);
  afficheBin(binHex,TAILLE_BIT_OPERATION);
  afficheHex(hex);
  ecrireHex(hex,fichierHex);
  binaryToHex(binHex2,hex);
  afficheBin(binHex2,TAILLE_BIT_OPERATION);
  afficheHex(hex);
  ecrireHex(hex,fichierHex);
  printf("\n----------------------------------\nTest du Parseur\n----------------------------------\n");
  parseFichier(nomFichier,"output/test.txt");
  /* Tests partie 2 */
  remplissageStructRegiste(registres,"src/listeReg.txt");
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
