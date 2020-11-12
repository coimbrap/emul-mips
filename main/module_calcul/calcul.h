#ifndef _CALCUL_H_
#define _CALCUL_H_

#include "../module_hex/hex.h"

int validHex(char* hex);
int checkBinVal(int* bin, int offset, char* valeur);
void traduitHex(char* hex);
int decValue(int* binTab);

#endif
