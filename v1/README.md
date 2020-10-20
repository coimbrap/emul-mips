# Spécification v1

## Prend une instruction et la met en hexadécimal

Toutes les informations sur les 28 opérateurs seront stocké dans un tableau remplit de la manière suivante :

```
 Tableau regroupant les 28 opérations

   0     1                       27
+-----+-----+------------------+-----+
|  +  |  +  |                  |  +  |
+-----+-----+------------------+-----+
   |     |                        |
   |     |                        |
   |     |                        |
   |     |                        |
   v     v                        v

  Structure remplit comme ci-dessous

       +--------------------+
       |                    |
       |     Nom ASCII      |
       |                    |
       +--------------------+
       |                    |
       |       opCode       |
       |                    |
       +--------------------+
       |                    |
       | Type d'instruction |
       |                    |
       +--------------------+
       |                    |
       |   Ordre des bits   |
       |                    |
       +--------------------+
       |                    |
       |  Remplissage type  |
       |                    |
       +--------------------+
```

Fonctionnement général :

- On lit le nom de l'opération
- On cherche la structure correspondant à cette opération
- On traduit en hexadécimal selon le type d'instruction

Les instructions sont séparées en 3 familles que nous avons recoupé en groupes et sous-groupes comme décrit ci-dessous.

### Instruction de type R

Les bits de 31 à 26 contiennent des 0.

Différents cas en fonction du type de type R

#### Cas 1
```
                      Cas 1 - Type R

 31      26 25    21 20    16 15    11 10     6 5        0
+----------+--------+--------+--------+--------+----------+
|          |        |        |        |        |          |
|  000000  |   rs   |   rt   |   rd   |   0    |   func   | SG 1
|          |        |        |        |        |          |
|  000000  |   0    |   0    |   0    |   0    |   func   | SG 2
|          |        |        |        |        |          |
|  000000  |   0    |   rt   |   rd   |   sa   |   func   | SG 3
|          |        |        |        |        |          |
+----------+--------+--------+--------+--------+----------+
```

##### SG 1 : (0/rs/rt/rd/0/func)
- ADD : 100000 (25/21 20/16 15/11 10/6 5/0)
- AND : 100100 (25/21 20/16 15/11 10/6 5/0)
- XOR : 100110 (25/21 20/16 15/11 10/6 5/0)
- OR : 100101 (25/21 20/16 15/11 10/6 5/0)
- SLT : 101010 (25/21 20/16 15/11 10/6 5/0)
- SUB : 100010 (25/21 20/16 15/11 10/6 5/0)
##### SG 2 : (0/0/0/0/0/func)
- NOP : 000000 (25/21 20/16 15/11 10/6 5/0)
##### SG 3 : (0/0/rt/rd/sa/func)
- SLL : 000000 (25/21 20/16 15/11 10/6 5/0)

#### Cas 2
```
                        Cas 2 - Type R

 31      26 25 22 21    21 20    16 15    11 10       6 5       0
+----------+-----+--------+--------+--------+---------+----------+
|          |     |        |        |        |         |          |
|  000000  |  0  |   R1   |   rt   |   rd   |   sa    |   func   |  SG 1
|          |     |        |        |        |         |          |
|  000000  |  0  |   R0   |   rt   |   rd   |   sa    |   func   |  SG 2
|          |     |        |        |        |         |          |
+----------+-----+--------+--------+--------+---------+----------+
```

##### SG 1 : (0/0/R1/rt/rd/sa/func)
- ROTR : 000010 (25/22 21 20/16 15/11 10/6 5/0)
##### SG 2 : (0/0/R0/rt/rd/sa/func)
- SRL : 000010 (25/22 21 20/16 15/11 10/6 5/0)

#### Cas 3
```
                  Cas 3 - Type R

 31      26 25    21 20    16 15    6 5        0
+----------+--------+--------+-------+----------+
|          |        |        |       |          |
|  000000  |   rs   |   rt   |   0   |   func   | SG 1
|          |        |        |       |          |
+----------+--------+--------+-------+----------+
```
##### SG 1 : (0/rs/rt/0/func)
- MULT : 011000 (25/21 20/16 15/6 5/0)
- DIV : 011010 (25/21 20/16 15/6 5/0)

#### Cas 4
```
                Cas 4 - Type R

 31      26 25    21 20   11 10     6 5        0
+----------+--------+-------+--------+----------+
|          |        |       |        |          |
|  000000  |   rs   |   0   |   sa   |   func   | SG 1
|          |        |       |        |          |
+----------+--------+-------+--------+----------+
```
##### SG 1 : (0/rs/0/hint/func)
- JR : 001000 (25/21 20/11 10/6 5/0)

#### Cas 5
```
                Cas 5 - Type R

 31      26 25   21 20    11 10    6 5        0
+----------+-------+--------+-------+----------+
|          |       |        |       |          |
|  000000  |   0   |   rd   |   0   |   func   |  SG 1
|          |       |        |       |          |
+----------+-------+--------+-------+----------+
```
##### SG 1 : (0/0/rd/0/func)
- MFHI : 010000 (25/16 15/11 10/6 5/0)
- MFLO : 010010 (25/16 15/11 10/6 5/0)

#### Cas 6
```
          Cas 6 - Type R

 31      26          6 5        0
+----------+----------+----------+
|          |          |          |
|  000000  |   code   |   func   |  SG 1
|          |          |          |
+----------+----------+----------+
```
##### SG 1 : (0/code/func)
- SYSCALL : 001100 (25/6 5/0)


### Instruction de type I
#### Cas 1
```
                Cas 1 - Type I

 31      26 25    21 20    16 15            0
+----------+--------+--------+---------------+
|          |        |        |               |
|  opcode  |   rs   |   rt   |   immediate   | SG 1
|          |        |        |               |
|  opcode  |   rs   |   0    |   immediate   | SG 2
|          |        |        |               |
|  opcode  |   0    |   rt   |   immediate   | SG 3
|          |        |        |               |
|  opcode  |  base  |   rt   |    offset     | SG 4
|          |        |        |               |
+----------+--------+--------+---------------+

```
##### SG 1 : (opcode/rs/rt/immediate)
- ADDI : 001000 (25/21 20/16 15/0)
- BEQ : 000100 (25/21 20/16 15/0)
- BNE : 000101 (25/21 20/16 15/0)

##### SG 2 : (opcode/rs/0/immediate)
- BGTZ : 000111 (25/21 20/16 15/0)
- BLEZ : 000110 (25/21 20/16 15/0)

##### SG 3 : (opcode/0/rt/immediate)
- LUI : 001111 (25/21 20/16 15/0)

##### SG 4 : (opcode/base/rt/offset)
- LW : 100011 (25/21 20/16 15/0)


### Instruction de type J
#### Cas 1
```
      Cas 1 - Type J

 31      26 25          0
+----------+-------------+
|          |             |
|  opcode  | instr_index | SG 1
|          |             |
+----------+-------------+
```
##### SG 1 : (opcode/instr_index)
- J : 000010 (25/0)
- JAL : 000011 (25/0)


Il y a donc 16 possibilités en tout.

### Les fonctions

Fonctions :

On a :
Un tableau de taille 26 qui contient les opérations,

Une case = Une structure contenant :
  - L'opcode
  - Le type d'instruction
  - L'ordre des bits (Code du cas)
  - Le remplissage à faire (Code du SG)

Un tableau de taille 32 correspondant aux bits en binaire de l'opération
Un tableau de taille 8 correspondant à la valeur hexadécimale de l'opération

- Lecture du nom de l'opération
  - Tant que on ne rencontre pas une caractère on avance (commence par espace).
  - Tant qu'on ne rencontre pas un espace on lit le nom de l'opération.
  - On associe ce nom d'opération avec 6 bit d'opcode

Fonctions annexe :
- infoOp* informationsNom(char nom[]);
Prend en entrée le nom de l'instruction et qui retourne un pointeur vers la case du tableau correspondant à cet opérateur

- void binVersHex(int bin[32], int hex[8]);
Prend en entrée le tableau de la représentation binaire de l'opération et l'adresse du tableau contenant la représentation hexadécimale

- char* enleveEspaces(char *s);
Enlève tout les espaces d'une chaine de caractère et retourne un pointeur vers la chaine sans espace

- char* enleveCommentaires(char *s)
Enlève tout ce qui est après un # et retourne un pointeur vers la chaine sans commentaires

- void parseFichier(char *nomFichier)
Permet de lire ligne à ligne le fichier le parse et exécute la fonction suivante

- void parseOperation(char *ope)
Transforme une expression en plusieurs blocs et rempli le tableau binaire.

Le séparateur est dans un premier temps le dollars la virgule
