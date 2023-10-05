#include <Arduino.h>
#include <timerAsserBas.h>
#include <Protocole.h>
#include <signaux.h>
#include <emission.h>
#include <8Voies.h>

uint8_t adresse_mod= 0x21;



uint16_t etatVoiesPrecedent = 0;
int frequency = 0, pastFreq = 0;
int tensionBase = 0;//OV

//---------
const int pinSignal = 33; // Numéro de la pin à laquelle le signal est connecté

volatile unsigned long pulseStartTime = 0;
volatile unsigned long pulseEndTime = 0;
volatile bool pulseDetected = false;

void IRAM_ATTR handleInterrupt() {
  if (digitalRead(pinSignal) == HIGH) {
    pulseStartTime = micros();
  } else {
    pulseEndTime = micros();
    pulseDetected = true;
  }
}
//---------

void setup() {
  Serial.begin(115200);

  init_Timer();

  init_Signaux();

  init_8Voies();

  pinMode(pinSignal, INPUT);

  output(1.16);

  tensionBase = analogRead(pinSignal);// tension de base 

  attachInterrupt(digitalPinToInterrupt(pinSignal), handleInterrupt, RISING);

  uint16_t etatVoies = 0x001*Voie1() | 0x002*Voie2() | 0x004*Voie3() | 0x010*Voie4()
                      |0x020*Voie5() | 0x040*Voie6() | 0x100*Voie7() | 0x200*Voie8();
  etatVoiesPrecedent = etatVoies;
  trame(adresse_mod, 0x8888);
  delay(400);
  trame(adresse_mod, (0xA400 + etatVoies));

  mscount = 0;

  
}

void loop() { 
  // uint16_t etatVoies = 0x001*Voie1() | 0x002*Voie2() | 0x004*Voie3() | 0x010*Voie4()
  //                     |0x020*Voie5() | 0x040*Voie6() | 0x100*Voie7() | 0x200*Voie8();

  // if(etatVoiesPrecedent != etatVoies){
  //   trame(adresse_mod, (0xA400 + etatVoies));
  //   etatVoiesPrecedent = etatVoies;
  // }
  
  // unsigned long pulseDuration = pulseIn(33, HIGH); // Mesure la durée de l'impulsion en microsecondes
  
  
  
  if (pulseDetected) {
    pulseDetected = false;
    unsigned long pulseDuration = pulseEndTime - pulseStartTime;
    // Calcule la fréquence en Hertz (fréquence = 1 / période)
    if(pulseDuration){frequency = 1000000.0 / pulseDuration;}
    if(pastFreq != FDC(frequency)){
      pastFreq = FDC(frequency);
      Serial.printf("Fréquence du signal : %d Hz, TON : %X \n", frequency, FDC(frequency));
    }
    
  }
  

  // TempsEchantionnage(200);
}
