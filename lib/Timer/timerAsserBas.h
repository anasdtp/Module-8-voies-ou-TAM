#include <Arduino.h>

#define TE_5MS 5

extern volatile uint16_t mscount;

//----------------------------------------------------------------------Timer
void init_Timer();
void onTime();//prototype de la fonction s'exécutent à chaque interruptions