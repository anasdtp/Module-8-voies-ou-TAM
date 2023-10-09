#include <emission.h>
#include <Protocole.h>
#include <signaux.h>

void trame(uint8_t adresse, uint8_t commande1, uint8_t commande0){         //Exemple : 0x21 0x88 ox88
  uint8_t adr1  = (adresse  >> 4)&0x0F, adr0 = adresse    &0x0F,
          cmd3  = (commande1>> 4)&0x0F, cmd2 = commande1  &0x0F,
          cmd1  = (commande0>> 4)&0x0F, cmd0 = commande0  &0x0F;

  //Pas deux frequences similaires succesives : 
  if(adr0 == adr1)  {adr0 = 0x0E;}
  if(cmd3 == adr0)  {cmd3 = 0x0E;}
  if(cmd2 == cmd3)  {cmd2 = 0x0E;}
  if(cmd1 == cmd2)  {cmd1 = 0x0E;}
  if(cmd0 == cmd1)  {cmd0 = 0x0E;}                                         //Exemple : 0x21 0x8E ox8E

  ondeCarree(DFC(adr1), TEMPS_DUN_TON);   //Most significant adr
  ondeCarree(DFC(adr0), TEMPS_DUN_TON);   //Least significant adr

  ondeCarree(DFC(cmd3), TEMPS_DUN_TON);   //Most significant cmd
  ondeCarree(DFC(cmd2), TEMPS_DUN_TON);  //second Most significant cmd
  ondeCarree(DFC(cmd1), TEMPS_DUN_TON);  //second Least significant cmd
  ondeCarree(DFC(cmd0), TEMPS_DUN_TON);   //Least significant cmd         //Exemple : Au tam : F218E8EF
}

void trameV2(uint8_t adresse, uint16_t commande){
  trame(adresse, ((commande>>8)&0xFF), (commande&0xFF));
}

bool trameWAck(uint8_t adresse, uint8_t commande1, uint8_t commande0){//Fonction utilisable uniquement dans loop() je pense
  const int TimeOut = 400, MaxRepeat = 4;

  waitingForACK = true;
  waitingForACK_cmd = ((commande1<<8) & 0xFF00) | (commande0 & 0xFF);

  trame(adresse, commande1, commande0);

  delay(100);
  
  int start_time = millis(), i = 0;
  while(waitingForACK){
    if((millis() - start_time)>TimeOut){
      i++;
      if(i>MaxRepeat){
        waitingForACK = false; // Arrêtez d'attendre après le nombre maximal de répétitions
        return false;
        }
      
      trame(adresse, commande1, commande0);
      start_time = millis();
    }
  }

  return true;
}

bool trameWAckV2(uint8_t adresse, uint16_t commande){
  return trameWAck(adresse, ((commande>>8)&0xFF), (commande&0xFF));
}
