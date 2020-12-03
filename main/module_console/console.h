#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "../module_hex/hex.h"
#include "../module_calcul/calcul.h"
#include "../module_registres/registres.h"
#include "../module_memoire/memoire.h"


void insererSegment(prog *segments, int programCounter, unsigned long int tradHex, char* instruction);
void afficherSegments(prog *segments, int pc);
void afficherSegmentPc(prog *segments, int pc, int type);
void libereSegments(prog *segments);
void parseFichier(char *input, char* output, int mode, instruction **instructions, registre** registres, memoire *mem, prog *segments);

#endif
