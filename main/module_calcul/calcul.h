#ifndef _CALCUL_H_
#define _CALCUL_H_

#include "../module_hex/hex.h"
#include "../module_tools/tools.h"
#include "../module_memoire/memoire.h"
#include "../module_registres/registres.h"

#define MASQUE_MAX 0xffffffff /* Valeur maximale d'une instruction en hexa */

/* Prend en entrée une instruction hexadécimale (demandé dans les specifications) */
/* Exécute l'instruction, met à jour les registres et la mémoire et change le PC */
void execInstruction(unsigned long int hex, registre **registres, instruction **instructions, memoire *mem);

void execTypeR(instruction *found, int opcode, int rsI, int rtI, int rdI, int sa, registre *pc, registre **registres, instruction **instructions, memoire *mem);
void execTypeI(instruction *found, int opcode, int rsI, int rtI, int imm, registre *pc, registre **registres, instruction **instructions, memoire *mem);

#endif
