#include "module_registres/registres.h"
#include <stdio.h>

int main() {
  registre* registres[NB_REGISTRE];
  remplissageStructRegiste(registres,"module_registres/listeReg.txt");
  afficheRegistres(registres);

  return 0;
}
