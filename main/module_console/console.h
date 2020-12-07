#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "../module_hex/hex.h"
#include "../module_calcul/calcul.h"
#include "../module_registres/registres.h"
#include "../module_memoire/memoire.h"
#include "../module_table/table.h"

/* prend en entrée la liste chaîné des segments, le program counteur la version hexa et le string de l'instruction uniforisé */
/* ajoute cette instruction à la liste chaîné */
void insererSegment(prog *segments, int programCounter, unsigned long int tradHex, char* instruction) {

/* prend en entrée la liste chaîné des segments et le program counteur */
/* affiche l'ensemble des segments assembleur contenu dans la liste chainée */
void afficherSegments(prog *segments, int pc) {

/* prend en entrée la liste chaîné des segments, le program counteur et le type */
/* type=1 : pc,hex,char type=0 : char */
/* affiche l'ensemble des segments assembleur contenu dans la liste chainée */
void afficherSegmentPc(prog *segments, int pc, int type) {

/* prend en entrée la liste chaîné des segments */
/* libère l'ensemble de la liste chainée pour éviter les fuites mémoire */
void libereSegments(prog *segments) {

/* prend en entrée le fichier d'entrée, le fichier de sortie (fichier d'entrée pas forcément utilisé) */
/* ainsi que les structures de stockage des instructions, des registres, la mémoire, les segments pour l'afficage, la table des symboles */
/* ET le Mode: 0: pàp, 1: auto et 2: intéractif */
/* parse ligne après ligne le fichier et s'occupe de tout l'affichage et des saisie utilisateur */
void parseFichier(char *input, char* output, int mode, instruction **instructions, registre** registres, memoire *mem, prog *segments, symtable *symbols) {
  
#endif
