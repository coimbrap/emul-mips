#include "module_hex/hex.h"
#include "module_tools/tools.h"
#include <string.h>
#include <stdio.h>

int main() {
  int l=0,offsetBin=0;
  char *binNum="11101001";
  char *nomFichier="tests_emul/test.txt";
  char *listeope="module_hex/listeOpe.txt";
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
  parseLigne(ligne,binParse);
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
  return 0;
}