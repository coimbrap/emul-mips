# Modules

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
