#include <emission.h>
#include <Protocole.h>
#include <signaux.h>

void trame(uint8_t adresse, uint16_t cmd){
  uint8_t adrL  = adresse &0x0F,  adrM  = (adresse>> 4)&0x0F,
          cmdL  = cmd     &0x0F,  cmdL2 = (cmd    >> 4)&0x0F,
          cmdM2 = (cmd>>8)&0x0F,  cmdM  = (cmd    >>12)&0x0F;

  if(adrL  == adrM) {adrL  = 0x0E; }
  if(cmdM  == adrL) {cmdM  = 0x0E; }
  if(cmdM2 == cmdM) {cmdM2 = 0x0E; }
  if(cmdL2 == cmdM2){cmdL2 = 0x0E; }
  if(cmdL  == cmdL2){cmdL  = 0x0E; }

  ondeCarree(DFC(adrM), TEMPS_DUN_TON);   //Most significant adr
  ondeCarree(DFC(adrL), TEMPS_DUN_TON);   //Least significant adr

  ondeCarree(DFC(cmdM), TEMPS_DUN_TON);   //Most significant cmd
  ondeCarree(DFC(cmdM2), TEMPS_DUN_TON);  //second Most significant cmd
  ondeCarree(DFC(cmdL2), TEMPS_DUN_TON);  //second Least significant cmd
  ondeCarree(DFC(cmdL), TEMPS_DUN_TON);   //Least significant cmd
}

