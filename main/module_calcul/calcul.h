#ifndef _CALCUL_H_
#define _CALCUL_H_

#include "../module_hex/hex.h"
#include "../module_memoire/memoire.h"
#include "../module_registres/registres.h"

#define INIT_PC 0xDDE0;

int validHex(char* hex);
int checkBinVal(int* bin, int offset, char* valeur);
void traduitHex(char* hex, registre** registres, instruction** instructions, memoire *mem, int* pc);



int chargeProgramme(memoire *mem, const char* progHex);
void execProgramme(memoire *mem, registre** registres, instruction** instructions, char* prog);

#endif
