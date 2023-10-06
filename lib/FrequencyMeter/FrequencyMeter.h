#include <Arduino.h>
#include "stdio.h"                                                        // Library STDIO
#include "driver/ledc.h"                                                  // Library ESP32 LEDC
#include "driver/pcnt.h"                                                  // Library ESP32 PCNT
#include "soc/pcnt_struct.h"



#define PCNT_COUNT_UNIT       PCNT_UNIT_0                                 // Set Pulse Counter Unit - 0 
#define PCNT_COUNT_CHANNEL    PCNT_CHANNEL_0                              // Set Pulse Counter channel - 0 

#define PCNT_INPUT_SIG_IO     GPIO_NUM_34                                 // Set Pulse Counter input - Freq Meter Input GPIO 34
#define LEDC_HS_CH0_GPIO      GPIO_NUM_33                                 // Saida do LEDC - gerador de pulsos - GPIO_33
#define PCNT_INPUT_CTRL_IO    GPIO_NUM_35                                 // Set Pulse Counter Control GPIO pin - HIGH = count up, LOW = count down  
#define OUTPUT_CONTROL_GPIO   GPIO_NUM_32                                 // Timer output control port - GPIO_32
#define PCNT_H_LIM_VAL        overflow                                    // Overflow of Pulse Counter 

#define IN_BOARD_LED          GPIO_NUM_2                                  // ESP32 native LED - GPIO 2



extern bool            flag          ;                                        // Flag to enable print frequency reading
extern uint32_t        overflow      ;                                       // Max Pulse Counter value
extern int16_t         pulses        ;                                      // Pulse Counter value
extern uint32_t        multPulses    ;                                     // Quantidade de overflows do contador PCNT
extern uint32_t        sample_time   ;                                    // sample time of 1 second to count pulses
extern uint32_t        osc_freq      ;                                   // Oscillator frequency - initial 12543 Hz (may be 1 Hz to 40 MHz)
extern uint32_t        mDuty         ;                                  // Duty value
extern uint32_t        resolution    ;                                 // Resolution value
extern float           frequency     ;                                // frequency value
extern char            buf[32];                                      // Buffer
                                                                    //;
extern esp_timer_create_args_t create_args;                        // Create an esp_timer instance
extern esp_timer_handle_t timer_handle;                           // Create an single timer
                                                                 //;
extern portMUX_TYPE timerMux         ;                          // portMUX_TYPE to do synchronism


void init_osc_freq ();
static void IRAM_ATTR pcnt_intr_handler(void *arg);
void init_PCNT(void) ;
void read_PCNT(void *p) ;
void init_frequencyMeter ();
char *ultos_recursive(unsigned long val, char *s, unsigned radix, int pos);
char *ltos(long val, char *s, int radix);

float FrequencyLoop();