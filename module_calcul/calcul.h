#ifndef _CALCUL_H_
#define _CALCUL_H_

#include "../module_hex/hex.h"
#include "../module_tools/tools.h"
#include "../module_memoire/memoire.h"
#include "../module_registres/registres.h"

#define MASQUE_MAX 0xffffffff /* Valeur maximale d'une instruction en hexa */

/* prend en entrée tout les paramètres d'une instruction de type R (rs,rt,rd et sa) */
/* l'opcode, la structure de l'instruction, le pc et l'ensemble des registres */
/* exécute l'instruction, met à jour les registres et la mémoire et change le PC */
void execTypeR(int opcode, instruction *instr, int rsI, int rtI, int rdI, int sa, registre *pc, registre **registres);

/* prend en entrée tout les paramètres d'une instruction de type I (rs,rt et imm) */
/* l'opcode, la structure de l'instruction, le pc et l'ensemble des registres */
/* exécute l'instruction, met à jour les registres et la mémoire et change le PC */
void execTypeI(int opcode, instruction *instr, int rsI, int rtI, int imm, registre *pc, registre **registres, memoire *mem);

/* prend en entrée tout le paramètre d'une instruction de type J (index) */
/* l'opcode, la structure de l'instruction, le pc et l'ensemble des registres */
/* exécute l'instruction, met à jour le registre ra et change le PC */
void execTypeJ(int opcode, instruction *instr, int index, registre *pc, registre **registres);

/* Prend en entrée une instruction hexadécimale (demandé dans les spécifications) */
/* Appelle la fonction associé au type d'instruction qui s'occupe du reste */
void execInstruction(unsigned long int hex, registre **registres, instruction **instructions, memoire *mem);

#endif
