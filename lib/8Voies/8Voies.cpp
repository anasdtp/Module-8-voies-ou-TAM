#include <8Voies.h>


const int pinVoie1 = 25, pinVoie2 = 23, pinVoie3 = 22, pinVoie4 = 21,
          pinVoie5 = 19, pinVoie6 = 18, pinVoie7 = 17, pinVoie8 = 16;


void init_8Voies(){
    pinMode(pinVoie1, INPUT_PULLDOWN); pinMode(pinVoie2, INPUT_PULLDOWN);
    pinMode(pinVoie3, INPUT_PULLDOWN); pinMode(pinVoie4, INPUT_PULLDOWN); 
    pinMode(pinVoie5, INPUT_PULLDOWN); pinMode(pinVoie6, INPUT_PULLDOWN);
    pinMode(pinVoie7, INPUT_PULLDOWN); pinMode(pinVoie8, INPUT_PULLDOWN);
}

bool Voie1(){return !digitalRead(pinVoie1);} bool Voie2(){return !digitalRead(pinVoie2);} 
bool Voie3(){return !digitalRead(pinVoie3);} bool Voie4(){return !digitalRead(pinVoie4);}
bool Voie5(){return !digitalRead(pinVoie5);} bool Voie6(){return !digitalRead(pinVoie6);} 
bool Voie7(){return !digitalRead(pinVoie7);} bool Voie8(){return !digitalRead(pinVoie8);}