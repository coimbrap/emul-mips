#include "calcul.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../module_tools/tools.h"

int validHex(char* hex) {
  int len=strlen(hex);
  int valid=0,i=0,ret=-1;
  printf("LEN %d \n", len);
  for (i=0;i<len;i++) {
    printf("%c|%d\n", hex[i],valid);
    if ((hex[i]>='0' && hex[i]<='9') || (hex[i]>='a' && hex[i]<='f') || (hex[i]>='A' && hex[i]<='F'))  {
      valid++;
    }
    printf("%c -> %d\n", hex[i],valid);

  }
  if (valid==len) {
    printf("Expression hexadécimale valide %s\n", hex);
    ret=1;
  }
  else {
    printf("Expression hexadécimale non valide %s\n", hex);
    ret=0;
  }
  return ret;
}

/* Prend en entrée une instruction hexadécimale (demandé dans les specifications) */
void traduitHex(char* instruction) {
  int len=strlen(instruction);
  int i=0;


  if (len==9) {
    printf("Correct\n");
  }
  else {
    printf("Format de l'instruction incorrect\n");
  }
  printf("%s\n", instruction);

}
