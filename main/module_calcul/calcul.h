#ifndef _CALCUL_H_
#define _CALCUL_H_

#include "../module_hex/hex.h"
#include "../module_tools/tools.h"
#include "../module_memoire/memoire.h"
#include "../module_registres/registres.h"

#define MASQUE_MAX 0xffffffff /* Valeur maximale d'une instruction en hexa */

/* prend en entrée un pointeur vers la mémoire et le fichier des instructions hex */
/* remplit la partie segment assembleur de la mémoire à partir des valeurs hex */
/* retourne la valeur de la dernière case mémoire contenant une instruction (vMax du PC) */
int chargeProgramme(memoire *mem, const char* progHex);

/* Prend en entrée une instruction hexadécimale (demandé dans les specifications) */
/* Exécute l'instruction, met à jour les registres et la mémoire et change le PC */
void execInstruction(unsigned long int hex, registre **registres, instruction **instructions, memoire *mem);

/* prend en entrée un pointeur vers : la mémoire,les registres et les instructions et le fichier des segments asm */
/* exécute le programme en faisant appel aux sous fonctions */
void execProgramme(memoire *mem, registre** registres, instruction** instructions, char* prog);

#endif
