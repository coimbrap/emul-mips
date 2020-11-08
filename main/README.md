# Projet CS351 : emul-mips

## Partie 1 : Traduction d'une instruction en une valeur hexadécimale

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
- SW : 101011 (25/21 20/16 15/0)


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

# Spécifications plus avancé

Tout d'abord pour la v1 notre code ne supportera pas les étiquettes et les directives, nous ne prendrons donc pas en compte les instructions de type J.

Dans le cas d'une valeur décimale supérieure à 2^16 ou inférieure à -2^15 on ne garanti pas le résultat car overflow.

Pour la traduction des registres mnémonique nous utilisons un bout du module registres que nous détaillerons dans le rendu suivant (en résumé c'est presque la même chose que pour la mémoire des opcode).

## Obtention de la valeur hexadécimale d'une instruction

Nous n'allons pas parler de la manière de récupérer une instruction dans cette partie.

Une instruction désigne par exemple : "ADD $20,$20,$3"

**On décompose l'opération comme ci-dessous :**

- Remplissage de la mémoire des instructions (structure décrite plus haut) à l'aide d'un fichier
- Uniformisation de l'instruction (enlève les espaces en trop et les commentaires)
- Récupération de l'opération (ADD/NOP/SLL...)
- Recherche dans la mémoire de la structure mémoire correspondant à l'opération
- Traduction des registres mnémonique
- Écriture de l'opcode dans le tableau de la représentation binaire de l'opération
- Recherche de toutes les opérandes et écriture dans le tableau de la représentation binaire en fonction du sous-type d'instruction
- Transformation du tableau binaire en un tableau hexadécimal
- Écriture de la valeur hexadécimale de l'opération dans un fichier

Pour chaque tiret il y a au moins une fonction. Le fonctionnement de nos fonctions est décrit dans les commentaires du code.

## Structure du code

Nous avons divisé le code est 3 modules,
- Le module hex qui regroupe l'essentiel du code
- Le module tools qui regroupe des fonctions qui nous servirons tout au long du projet (traductions binaire, affichage de tableaux...)
- Le module registres qui permet de traduire les noms mnémonique en valeur chiffrée. Ce module est la version 0 du module que nous utiliserons dans la deuxième partie. Nous avons choisi d'en faire un module pour plus de simplicité.
