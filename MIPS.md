# MIPS

## La mémoire

- Adresses de 0x0000000 à 0xFFFFFFF (32 bits).
- Big endian : octet de poids le plus fort à l'adresse la plus basse.

## Registres

- 32 registres $0 -> $31
- Chaque registre est associé à un nom mnémonique
- PC sur 32 bits pour l'adresse mémoire de la prochaine instruction incrémenter après l'exécution de chaque instruction.

## Instructions

- Codé sur 32 bits
- [étiquette] [opération] [opérandes]

## Modes d'adressage

**ADD $3, $3, $2**
- Direct : Valeur de l'opérande contenue dans une registre

**ADDI $2, $3, 100**
- Immédiat : Valeur de l'opérande dans l'instruction

**LW $3, 200($2)**
- Indirect : Le registre appelé contient l'adresse mémoire et deux octets contienne l'offset. L'adresse de l'opérande et le contenu du registre
