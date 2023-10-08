#include <Arduino.h>

void trame(uint8_t adresse, uint8_t commande1, uint8_t commande0);
void trameV2(uint8_t adresse, uint16_t commande);

bool trameWAck(uint8_t adresse, uint8_t commande1, uint8_t commande0);
bool trameWAckV2(uint8_t adresse, uint16_t commande);