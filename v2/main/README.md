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
