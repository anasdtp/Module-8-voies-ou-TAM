#include <Arduino.h>


#define TEMPS_DUN_TON 34 //en ms

#define TON_0 2653
#define TON_1 1505
#define TON_2 1603
#define TON_3 1707
#define TON_4 1818
#define TON_5 1936
#define TON_6 2062
#define TON_7 2196
#define TON_8 2340
#define TON_9 2491
#define TON_A 3214
#define TON_B 1245
#define TON_C 3009
#define TON_D 1327
#define TON_E 2825
#define TON_F 0

//Protocole de communication :



//---------------------------

extern uint8_t adresse_mod;

extern bool waitingForACK; //Flag attente ack
extern uint16_t waitingForACK_cmd;//Variable d'attente ACK


int DFC(uint8_t TON);
uint8_t FDC(int freq);