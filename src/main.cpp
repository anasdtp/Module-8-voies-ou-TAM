#include <Arduino.h>
#include <timerAsserBas.h>
#include <Protocole.h>
#include <signaux.h>
#include <emission.h>

uint8_t adresse_mod= 0x21;

const int pinVoie1 = 25, pinVoie2 = 23, pinVoie3 = 22, pinVoie4 = 21,
          pinVoie5 = 19, pinVoie6 = 18, pinVoie7 = 17, pinVoie8 = 16;

void TempsEchantionnage();
void setup() {
  Serial.begin(115200);
  pinMode(pinVoie1, INPUT_PULLDOWN); pinMode(pinVoie2, INPUT_PULLDOWN);
  pinMode(pinVoie3, INPUT_PULLDOWN); pinMode(pinVoie4, INPUT_PULLDOWN); 
  pinMode(pinVoie5, INPUT_PULLDOWN); pinMode(pinVoie6, INPUT_PULLDOWN);
  pinMode(pinVoie7, INPUT_PULLDOWN); pinMode(pinVoie8, INPUT_PULLDOWN);

  output(1.16);

  
  trame(adresse_mod, 0x8888);
  delay(400);
  trame(adresse_mod, 0xA400);

}

void loop() {
  // ondeCarree(20, 200);
  // ondeCarree(3214, 2000);
  
  TempsEchantionnage();
}


void TempsEchantionnage(){
    if (mscount >= (TE_5MS)) 
  {   
    Serial.println("erreur temp calcul");
    Serial.println(mscount);
  }
  else 
  {
    while (mscount<(TE_5MS));
  }
  //digitalWrite(27, set);//pour mesurer le temps de boucle avec l'oscilloscope
  //set = !set; //temps de boucle = 1/(freq/2)
  mscount = 0; 
}

