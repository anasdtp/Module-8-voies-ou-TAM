#include <8Voies.h>


const int pinVoie1 = 25, pinVoie2 = 23, pinVoie3 = 22, pinVoie4 = 21,
          pinVoie5 = 19, pinVoie6 = 18, pinVoie7 = 17, pinVoie8 = 16;


void init_8Voies(){
    pinMode(pinVoie1, INPUT_PULLDOWN); pinMode(pinVoie2, INPUT_PULLDOWN);
    pinMode(pinVoie3, INPUT_PULLDOWN); pinMode(pinVoie4, INPUT_PULLDOWN); 
    pinMode(pinVoie5, INPUT_PULLDOWN); pinMode(pinVoie6, INPUT_PULLDOWN);
    pinMode(pinVoie7, INPUT_PULLDOWN); pinMode(pinVoie8, INPUT_PULLDOWN);
}

bool Voie1()
{
    static bool waiting = false, pastEtat = true;
    static unsigned long start_time;
    static int Timeout = 1000;

    static bool etat = true;

    etat = !digitalRead(pinVoie1);

    if (waiting)
    {
        if ((millis() - start_time) >= Timeout)
        {
            waiting = false;
        }
    }
    else if (pastEtat != etat)
    {
        pastEtat = etat;
        waiting = true;
        start_time = millis();
    }

    return pastEtat;
}

bool Voie2()
{
    static bool waiting = false, pastEtat = true;
    static unsigned long start_time;
    static int Timeout = 1000;

    static bool etat = true;

    etat = !digitalRead(pinVoie2);
    if (waiting){
        if ((millis() - start_time) >= Timeout){
            waiting = false;
        }
    }
    else if (pastEtat != etat)
    {
        pastEtat = etat;
        waiting = true;
        start_time = millis();
    }

    return pastEtat; 
}

bool Voie3()
{
    static bool waiting = false, pastEtat = true;
    static unsigned long start_time;
    static int Timeout = 1000;
    
    static bool etat = true;

    etat = !digitalRead(pinVoie3);
    if (waiting){
        if ((millis() - start_time) >= Timeout){
            waiting = false;
        }
    }
    else if (pastEtat != etat)
    {
        pastEtat = etat;
        waiting = true;
        start_time = millis();
    }

    return pastEtat; 
}

bool Voie4()
{
    static bool waiting = false, pastEtat = true;
    static unsigned long start_time;
    static int Timeout = 1000;
    
    static bool etat = true;

    etat = !digitalRead(pinVoie4);
    if (waiting){
        if ((millis() - start_time) >= Timeout){
            waiting = false;
        }
    }
    else if (pastEtat != etat)
    {
        pastEtat = etat;
        waiting = true;
        start_time = millis();
    }

    return pastEtat; 
}

bool Voie5()
{
    static bool waiting = false, pastEtat = true;
    static unsigned long start_time;
    static int Timeout = 1000;
    
    static bool etat = true;

    etat = !digitalRead(pinVoie5);
    if (waiting){
        if ((millis() - start_time) >= Timeout){
            waiting = false;
        }
    }
    else if (pastEtat != etat)
    {
        pastEtat = etat;
        waiting = true;
        start_time = millis();
    }

    return pastEtat; 
}

bool Voie6()
{
    static bool waiting = false, pastEtat = true;
    static unsigned long start_time;
    static int Timeout = 1000;
    
    static bool etat = true;

    etat = !digitalRead(pinVoie6);
    if (waiting){
        if ((millis() - start_time) >= Timeout){
            waiting = false;
        }
    }
    else if (pastEtat != etat)
    {
        pastEtat = etat;
        waiting = true;
        start_time = millis();
    }

    return pastEtat; 
}

bool Voie7()
{
    static bool waiting = false, pastEtat = true;
    static unsigned long start_time;
    static int Timeout = 1000;
    
    static bool etat = true;

    etat = !digitalRead(pinVoie7);
    if (waiting){
        if ((millis() - start_time) >= Timeout){
            waiting = false;
        }
    }
    else if (pastEtat != etat)
    {
        pastEtat = etat;
        waiting = true;
        start_time = millis();
    }

    return pastEtat; 
}

bool Voie8()
{
    static bool waiting = false, pastEtat = true;
    static unsigned long start_time;
    static int Timeout = 1000;
    
    static bool etat = true;

    etat = !digitalRead(pinVoie8);
    if (waiting){
        if ((millis() - start_time) >= Timeout){
            waiting = false;
        }
    }
    else if (pastEtat != etat)
    {
        pastEtat = etat;
        waiting = true;
        start_time = millis();
    }

    return pastEtat; 
}