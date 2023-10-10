#include <Arduino.h>

#define TE_5MS  5
#define TE_10MS 10

extern volatile uint16_t mscount, mscount2;

//----------------------------------------------------------------------Timer
void init_Timer();
void onTime();//prototype de la fonction s'exécutent à chaque interruptions
void TempsEchantionnage(int TIME);