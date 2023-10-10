#include <Arduino.h>
#include <freertos\FreeRTOS.h>
#include <freertos\task.h>

#include <FrequencyMeter.h>

#define SIZE_FIFO 32

typedef struct FreqMessage{
    uint8_t trame[6]={0};     //[adresse1][adresse0][cmd3][cmd2][cmd1][cmd0] //sur 4 bits chacunes
    // uint8_t nbFreq = 0;      //Nombres de fréquences reçu, avec 6 le maximum

    uint8_t adr = 0x00;     //[adresse1][adresse0] //sur 4 bits chacunes
    uint8_t cmd1 = 0x00;   //[cmd3][cmd2]         //sur 4 bits chacunes
    uint8_t cmd0 = 0x00;  //[cmd1][cmd0]         //sur 4 bits chacunes
}FreqMessage;

extern FreqMessage rxMsg[SIZE_FIFO];
extern unsigned char FIFO_ecriture;

void init_Reception();

void init_ProcessRxTask();
void ProcessRx();
void ProcessRxTask(void *parameter);