# Spécification v1
tabkeau de 32
## Prend une instruction et la met en hexadecimal

- Nom de l'opération associé à l'opcode -> Définir type de stockage

- Lecture du nom de l'opération
  - Tant que on ne rencontre pas une caractère on avance (commence par espace).
  - Tant qu'on ne rencontre pas un espace on lit le nom de l'opération.
  - On associe ce nom d'opération avec 6 bit d'opcode

- On défini le type d'instruction en fonction de l'opcode

En fonction du type d'instruction

### Cas R

- Tant que  pas \0 ou \n ou un #
- Tant que c'est pas un dollars on avance on lit rs puis rt puis rd

Remplissage :
- 31 -> 26 : 000000

Différents cas en fonction du type de type R

#### Cas 1
##### (rs/rt/rd/0/func)
- ADD : 100000 (25/21 20/16 15/11 10/6 5/0)
- AND : 100100 (25/21 20/16 15/11 10/6 5/0)
- XOR : 100110 (25/21 20/16 15/11 10/6 5/0)
- OR : 100101 (25/21 20/16 15/11 10/6 5/0)
- SLT : 101010 (25/21 20/16 15/11 10/6 5/0)
- SUB : 100010 (25/21 20/16 15/11 10/6 5/0)
##### (0/0/0/0/func)
- NOP : 000000 (25/21 20/16 15/11 10/6 5/0)
##### (0/rt/rd/sa/func)
- SLL : 000000 (25/21 20/16 15/11 10/6 5/0)

#### Cas 2
##### (0/R1/rt/rd/sa/func)
- ROTR : 000010 (25/22 21 20/16 15/11 10/6 5/0)
##### (0/R0/rt/rd/sa/func)
- SRL : 000010 (25/22 21 20/16 15/11 10/6 5/0)

#### Cas 3
##### (rs/rt/0/func)
- MULT : 011000 (25/21 20/16 15/6 5/0)
- DIV : 011010 (25/21 20/16 15/6 5/0)

#### Cas 4
##### (rs/0/sa/func)
- JR : 001000 (25/21 20/11 10/6 5/0)

#### Cas 5
##### (0/rd/0/func)
- MFHI : 010000 (25/16 15/11 10/6 5/0)
- MFLO : 010010 (25/16 15/11 10/6 5/0)

#### Cas 6
##### (code/func)
- SYSCALL : 001100 (25/6 5/0)
