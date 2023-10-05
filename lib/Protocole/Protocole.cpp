#include <Protocole.h>

/*Digital to frequence converter ; TON sur 4 bits*/
int DFC(uint8_t TON){
  switch (TON)
  {
  case 0:
    return TON_0;
    break;

  case 1:
    return TON_1;
    break;

  case 2:
    return TON_2;
    break;

  case 3:
    return TON_3;
    break;

  case 4:
    return TON_4;
    break;

  case 5:
    return TON_5;
    break;

  case 6:
    return TON_6;
    break;

  case 7:
    return TON_7;
    break;

  case 8:
    return TON_8;
    break;
    
  case 9:
    return TON_9;
    break;

  case 0x0A:
    return TON_A;
    break;

  case 0x0B:
    return TON_B;
    break;

  case 0x0C:
    return TON_C;
    break;

  case 0x0D:
    return TON_D;
    break;

  case 0x0E:
    return TON_E;
    break;

  case 0x0F:
    return TON_F;
    break;
  
  
  default:
    break;
  }

  return TON_F;
}

/*frequence to digital converter*/
uint8_t FDC(int freq) {
    const int frequencies[16] = {2653, 1505, 1603, 1707, 1818, 1936, 2062, 2196, 2340, 2491, 3214, 1245, 3009, 1327, 2825, 0};
    const int marginOfError = 100; // Marge d'erreur de fréquence en Hz

    uint8_t result = 0xF; // Valeur par défaut en cas de fréquence non reconnue

    for (int i = 0; i < 16; ++i) {
        if (abs(freq - frequencies[i]) <= marginOfError) {
            result = i;
            break;
        }
    }

    return result;
}