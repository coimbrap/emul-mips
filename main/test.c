#include "module_hex/hex.h"
#include "module_tools/tools.h"
#include "module_registres/registres.h"
#include <string.h>
#include <stdio.h>

int main() {
  /* Partie 1 */
  int l=0,offsetBin=0;
  char *binNum="11101001";
  char *listeope="src/listeOpe.txt";
  char *nom="ADD";
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
  registre* foundReg=NULL;
  char* nomReg="s0";
  char *listereg="src/listeReg.txt";

  /* Tests partie 1 */
  for (l=0;l<TAILLE_BIT_OPERATION;l++) {
    bin[l]=0;
  }
  printf("\n----------------------------------\nTest Remplissage Structure\n----------------------------------\n");
  remplissageStructInstruction(instructions,listeope);
  afficheStructInstruction(instructions);
  remplissageStructRegistre(registres,listereg);
  afficheRegistres(registres);
  printf("\n----------------------------------\nTest recherche Structure\n----------------------------------\n");
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
  parseLigne(ligne,binParse,instructions,registres);
  afficheBin(binParse,TAILLE_BIT_OPERATION);
  printf("\n------------------------------\nTest binToHex\n------------------------------\n");
  binaryToHex(binHex,hex);
  afficheBin(binHex,TAILLE_BIT_OPERATION);
  afficheHex(hex);
  binaryToHex(binHex2,hex);
  afficheBin(binHex2,TAILLE_BIT_OPERATION);
  afficheHex(hex);
  /* Tests partie 2 */
  printf("\n------------------------------------\nTest recherche et écriture structure\n------------------------------------\n");
  printf("On cherche %s\n", nom);
  foundReg=trouveRegistre(registres,nom);
  if (foundReg!=NULL) {
    afficheRegistre(foundReg);
  }
  nom="HI";
  printf("\nOn cherche %s\n", nom);
  foundReg=trouveRegistre(registres,nom);
  if (foundReg!=NULL) {
    afficheRegistre(foundReg);
  }
  nom="12";
  printf("\nOn cherche %s\n", nom);
  foundReg=trouveRegistre(registres,nom);
  if (foundReg!=NULL) {
    afficheRegistre(foundReg);
  }
  nom="-1";
  printf("\nOn cherche %s\n", nom);
  foundReg=trouveRegistre(registres,nom);
  if (foundReg!=NULL) {
    afficheRegistre(foundReg);
  }
  printf("\n------------------------------------\nTest traduction registre\n------------------------------------\n");
  nomReg="HI";
  nomReg=traduitRegistre(registres, nomReg);
  printf("Traduction %s\n", nomReg);
  return 0;
}
