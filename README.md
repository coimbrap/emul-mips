## Git

### Gestion par branche

- On ne touche pas à la branche main
- On crée une branche une fois que c'est fini ET stable on merge main et la branche

#### Gestion des conflits
- On pull le main sur notre branche
- On gère les conflits (merge) à la main via atom
```
<<<<<<<

=======

>>>>>>>
```
- On fusionne notre branche et le main une fois le conflit résolu


### Mode interactif

Utilisateur rentre une instruction
On l'exécute et on en demande une nouvelle

On prend que des instructions en séquence

### Mode non-intéractif

#### Option -pas

L'utilisateur valide le passage à l'instruction suivante

1. On affiche la traduction de l'instruction hexadecimale
2. On exécute l'instruction
3. On modifie les valeurs dans les registres et dans la mémoire
4. On affiche l'état de tout les registres

### Rendu

1. Écrire des programmes MIPS pour tester l'application. Vision des tests efficace
2. Version 1 : traduit sous forme textuelle des instructions MIPS hexadecimale
3. Tester la v1 avec les exemples
4. Version 2 :
  - Un module gérant la mémoire
  - Un module gérant les registres
  Réflexion à expliquer.
5. Développer et tester
