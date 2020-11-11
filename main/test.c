#include "module_hex/hex.h"

#include "module_tools/tools.h"

#include "module_registres/registres.h"
#include "module_memoire/memoire.h"
#include "module_calcul/calcul.h"

#include <string.h>
#include <stdio.h>

int main() {
  /* Partie 1 */
/*  int l=0,offsetBin=0;
  char *binNum="11101001";
  char *listeope="src/listeOpe.txt";
  char *ligne="SLL $10,$23,24";
  instruction *instructions[NB_OPERATIONS+1];
  instruction *found=NULL;
  int bin[TAILLE_BIT_OPERATION];
  int binParse[TAILLE_BIT_OPERATION];
  int binHex[TAILLE_BIT_OPERATION]={0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0};
  int binHex2[TAILLE_BIT_OPERATION]={0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0};
  char hex[TAILLE_HEX_OPERATION]={0,0,0,0,0,0,0}; */
  /* Partie 2 */
  registre* registres[NB_REGISTRE];
  registre* foundReg=NULL;
  char* nomReg="s0";
  char *nom="ADD";
  int binReg[]={0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0};

  char *listereg="src/listeReg.txt";

  /* Tests partie 1 */
/*  for (l=0;l<TAILLE_BIT_OPERATION;l++) {
    bin[l]=0;
  }
  printf("\n----------------------------------\nTest Remplissage Structure\n----------------------------------\n");
  remplissageStructInstruction(instructions,listeope);
  afficheStructInstruction(instructions);

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
  offsetBin=3;
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
  afficheHex(hex);*/
  printf("\n------------------------------------\nTest recherche et écriture structure\n------------------------------------\n");
  remplissageStructRegistre(registres,listereg);
  afficheRegistres(registres);
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
  nom="12";
  printf("\nOn cherche %s\n", nom);
  foundReg=trouveRegistre(registres,nom);
  if (foundReg!=NULL) {
    changeRegistre(foundReg,binReg);
    afficheRegistre(foundReg);
  }


  printf("\n------------------------------------\nTest traduction registre\n------------------------------------\n");
  nomReg="HI";
  nomReg=traduitRegistre(registres, nomReg);
  printf("Traduction %s\n", nomReg);
  memoire mem=NULL;
  int m=0;

  int* binTmp=NULL;
  char hex[8];
  binTmp=decToBin(0xffffffff,NB_BIT_MEMOIRE);
  afficheBin(binTmp,32);
  binaryToHex(binTmp,hex);
  afficheHex(hex);
  long int test=0xffffffff;
  printf("%ld\n", test);
  printf("%ld\n", hexToDec(hex));

  printf("\n------------------------------------\nTest de la liste chaînée\n------------------------------------\n");
  /* Insertion en tête */
  afficherMemoire(&mem);
  insertion(0x4,decToBin(2,NB_BIT_MEMOIRE),&mem);
  afficherMemoire(&mem);
  insertion(0x0,decToBin(1,NB_BIT_MEMOIRE),&mem);
  afficherMemoire(&mem);
  /* Insertion en queue */
  insertion(0xffec,decToBin(4,NB_BIT_MEMOIRE),&mem);
  afficherMemoire(&mem);
  insertion(0xfffc,decToBin(7,NB_BIT_MEMOIRE),&mem);
  afficherMemoire(&mem);
  /* Insertion au millieu */
  insertion(0xfff8,decToBin(6,NB_BIT_MEMOIRE),&mem);
  afficherMemoire(&mem);
  insertion(0xfff4,decToBin(5,NB_BIT_MEMOIRE),&mem);
  afficherMemoire(&mem);
  insertion(0x101c,decToBin(3,NB_BIT_MEMOIRE),&mem);
  afficherMemoire(&mem);
  /* Test doublons */
  insertion(0xfffc,decToBin(17,NB_BIT_MEMOIRE),&mem);
  afficherMemoire(&mem);
  insertion(0x0,decToBin(10,NB_BIT_MEMOIRE),&mem);
  afficherMemoire(&mem);
  insertion(0xffec,decToBin(14,NB_BIT_MEMOIRE),&mem);
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
  printf("On libère\n");
  insertion(0x4,decToBin(2220,NB_BIT_MEMOIRE),&mem);
  insertion(0x0,decToBin(11202,NB_BIT_MEMOIRE),&mem);
  insertion(0xffec,decToBin(4203,NB_BIT_MEMOIRE),&mem);
  insertion(0xfffc,decToBin(21003,NB_BIT_MEMOIRE),&mem);
  insertion(0xfff8,decToBin(0xffffffff,NB_BIT_MEMOIRE),&mem);
  insertion(0xfff4,decToBin(30001,NB_BIT_MEMOIRE),&mem);
  insertion(0x101c,decToBin(20140,NB_BIT_MEMOIRE),&mem);
  afficherMemoire(&mem);
  liberation(&mem);
  afficherMemoire(&mem);
  printf("\n------------------------------------\nVerification hexa\n------------------------------------\n");
  validHex("ae4f5dE");
  validHex("aK4f5dE");
  validHex("FFFFFFF");
  validHex("0000000");
  validHex("GGGGGGG");
  printf("\n------------------------------------\nTraduit hexa\n------------------------------------\n");

  traduitHex("00000000"); /* NOP cas full 0 */
  traduitHex("3C140010"); /*LUI $20,16*/
  traduitHex("02C2B020"); /* ADD $22,$22,$2 */
  traduitHex("00284A42"); /* ROTR $9,$8,9 */
  traduitHex("00084A42"); /* SRL $9,$8,9 */
  traduitHex("00118C00"); /* SLL $17,$17,16 cas particulier (opcode 000000)*/

  traduitHex("01F00018"); /* MULT $15,$16 */
  traduitHex("03E00008"); /* JR $ra */
  traduitHex("0000A812"); /* MFLO $21 */
  traduitHex("0000000C"); /* SYSCALL */

  traduitHex("200FED03"); /* ADDI $15,$0,9956611 */

  char hexT[8];
  binaryToHex(binReg,hexT);
  printf("%s\n", hexT);
  printf("Dec : %ld\n",hexToDec(hexT));

  /*char *hex=NULL;
  char bin[8]={'1','1','1','1','0','0','0','0'};

  int bin2[]={1,1,1,1,0,0};

  hex=decToHex(52);
  if(hex!=NULL) {
    printf("Char : %s\n",hex);
  }

  printf("%d\n", tabToInt(bin2,5));
  printf("Dec : %d\n",binToDec(bin2,5));*/


  return 0;
}
