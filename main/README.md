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

# Modules

Nous avons segmenté notre v1 en deux modules, un module hex qui permet de transformer une instruction assembleur en un code hexadécimal et un module tools qui contient des fonctions que l'on utilisera dans tout les modules de notre programme.

## Module registres

L’objectif de ce module est de fournir une implémentation des registres d'un processeur MIPS en C. Les fonctions de base de ce module permettront de lire la valeur d'un registre et de changer la valeur d'un registre.

Pour la représentation en mémoire de ces registres nous avons procédé de la manière suivante :

Un structure représentant un registre organisé de la manière suivante :
```
+----------------------+
|                      |
|  Numéro du registre  |
|                      |
+----------------------+
|                      |
|       Nom ASCII      |
|                      |
+----------------------+
|                      |
|  Valeur du registre  |
|                      |
+----------------------+
```

Le numéro du registre, le nom mnémonique et la valeur du registre seront initialisé par une fonction à l'aide d'un fichier. Par défaut la valeur du registre sera initialisé à une valeur arbitraire pour éviter des valeurs aléatoire dans l'affichage dû au malloc.

Afin de pouvoir utiliser la même structure pour les 32 registres GPR et pour les registres spécialisés si le numéro du registre vaut -1 on à affaire à un registre spécialisés. Dans ce cas c'est le nom ASCII qui fera aussi d'identifiant pour le registre.

- Le numéro du registre sera un nombre entier entre -1 et 31 on le stockera sous la forme d'un int.
- Le nom mnémonique sera un tableau de 5 char car le plus long est zero.
- La valeur du registre sera un tableau contenant 32 cases pour les 32 bits de leur valeur en binaire. On le stockera sous la forme d'un tableau d'entier.

Il y a donc 35 registres pour notre implémentation. Nous allons donc utilisé un tableau de 35 cases avec chaque case pointant vers une structure décrite ci-dessus associé à un registre.

## Module mémoire

Une case mémoire est composée de 32bits.

La mémoire est stocké entre les adresses 0x00000000 et 0xFFFFFFFF.

Les instructions pouvant accéder à la mémoire dans notre implémentation sont LW et SW. Elles permettent de faire le lien entre un registre et la mémoire en transférant un mot de 32bits soit 4octets.

Ces 4 octets devront être stocké à la suite dans la mémoire en respectant la norme Big Endian (octet de poids fort à l'adresse la plus basse). De plus l'adresse du premier octet devra être divisible par 4.

En résumé la quantité de mémoire utilisé par un programme n'est pas prédictible.

La première question soulevé est la suivante :
Une adresse pointe vers une case pouvant stocké un octet (8bits) or dans notre cas nous stockons uniquement des mots soit 4 octets (32 bits).
Faut-il avoir une mémoire géré par mots (bloc de 4 octets) ou une mémoire géré par octet ?

Vu que pour notre implémentation nous n'utilisons pas les directives et que les deux seules opérations accédant à la mémoire sont LW et SW. Nous allons donc faire une implémentation de la mémoire simplifié, pour cela nous allons utilisé la map mémoire suivante.

```
0xFFFF +------------------------+
       |                        |
       |          Pile          |
       |                        |
0x2000 +------------------------+
       |                        |
       |        Mémoire         |
       |                        |
0x0000 +------------------------+
```

La mémoire accessible avec les instructions *LW* et *SW* se trouvera donc entre les adresses 0x0000 et 0x2000 et sera remplit des adresses bases aux adresses hautes.

Pour la pile elle se remplit par le bas donc elle se trouvera entre 0xFFFF et 0x2000.

Vu que la mémoire ne sera pas trop utilisé nous allons utilisé une listes chaînée dynamiquement alloué pour utiliser le moins de mémoire possible.

On aura donc la map mémoire qui contiendra :
- Une liste pour la mémoire
- Une pile descendante

L'utilisation de la pile est géré par le registre *sp* qui pointe vers l'adresse du haut de la pile, on peut donc utiliser la même implématation pour le pile et pour la mémoire seule l'utilisation varie.

Pour l'implémentation nous avons retenu celle d'une liste chaîné. Chaque maillon de la liste sera composé comme décrit ci-dessous :
- L'adresse de la case mémoire du premier octet du mot (un multiple de 4)
- La valeur du mot sous forme de tableau
- Un pointeur vers l'adresse suivante, si ce pointeur vaut NULL c'est la fin de la mémoire

Pour une question des questions de simplicité on veillera à ce que les adresses reste dans l'ordre.

En résumé un maillon contiendra un mot et sera désigné par l'adresse de la case mémoire du premier octet du mot.

*Recherche documentaire sur les liens suivant :*
*https://www-soc.lip6.fr/trac/sesi-almo/chrome/site/docs/ALMO-mips32-archi-asm.pdf*

*https://tdinfo.phelma.grenoble-inp.fr/2Aproj/ressources/PHELMA_ProjetInformatique2A_2019-20.pdf*

*http://www-id.imag.fr/~briat/perso.html/NACHOS/NACHOS_DOC/CA225b.html*
