#include <Arduino.h>
#include <timerAsserBas.h>
#include <Protocole.h>
#include <signaux.h>
#include <emission.h>
#include <8Voies.h>

#include <reception.h>






uint8_t adresse_mod= 0x21;
uint16_t etatVoies = 0, etatVoiesPrecedent = 0;

//---------
//const int pinSignal = 33; // Numéro de la pin à laquelle le signal est connecté
void TraitementMsgLoop();
void freqMesure(int Valeur_Capteur);
void setup() {
  Serial.begin(115200);
  //Timer
  init_Timer(); //********
  //Signaux
  init_Signaux();
  //Voies
  init_8Voies();

  

  init_Reception();


  //Tension de base en sortie
  output(1.16);

  //Acquisition tension de base en entrée, test 2
  // tensionBase = analogRead(pinSignal);  //********

  uint16_t etatVoies = 0x001*Voie1() | 0x002*Voie2() | 0x004*Voie3() | 0x010*Voie4()
                      |0x020*Voie5() | 0x040*Voie6() | 0x100*Voie7() | 0x200*Voie8();
  etatVoiesPrecedent = etatVoies;

  trame(adresse_mod, 0x88, 0x88);
  delay(400);//Attendre l'ACK
  trameV2(adresse_mod, (0xA400 + etatVoies));

  mscount = 0;

  
}

void loop() { 
  etatVoies = 0x001*Voie1() | 0x002*Voie2() | 0x004*Voie3() | 0x010*Voie4()
             |0x020*Voie5() | 0x040*Voie6() | 0x100*Voie7() | 0x200*Voie8();

  if(etatVoiesPrecedent != etatVoies){
    trameV2(adresse_mod, (0xA400 + etatVoies));
    etatVoiesPrecedent = etatVoies;
  }
  

  //test Lecture Freq n° Deux : 
  // freqMesure(analogRead(pinSignal));  //********

  TraitementMsgLoop();

  TempsEchantionnage(200);
}


void TraitementMsgLoop(){//Tel un module 8 voies pour le moment et non pas comme un TAM. Mais c'est similaire
  static signed char FIFO_lecture=0,FIFO_occupation=0,FIFO_max_occupation=0;

  FIFO_occupation=FIFO_ecriture-FIFO_lecture;
  if(FIFO_occupation<0){FIFO_occupation=FIFO_occupation+SIZE_FIFO;}
  if(FIFO_max_occupation<FIFO_occupation){FIFO_max_occupation=FIFO_occupation;}

  if(!FIFO_occupation){/*Alors pas de message*/return;}
  Serial.printf("Recu %X %X %X\n", rxMsg[FIFO_lecture].adr, rxMsg[FIFO_lecture].cmd1, rxMsg[FIFO_lecture].cmd0);
  if(rxMsg[FIFO_lecture].adr == adresse_mod){//Alors cela nous concerne
    trame(rxMsg[FIFO_lecture].adr, rxMsg[FIFO_lecture].cmd1, rxMsg[FIFO_lecture].cmd0); // On envoie l'ACK

    //Si c'est un TAM on se contente d'afficher ensuite la trame sur un LCD

    //On traite maintenant la commande
    switch (rxMsg[FIFO_lecture].cmd1)
    {
    case 0xD9:
      if(rxMsg[FIFO_lecture].cmd0 == adresse_mod){
        trame(adresse_mod, 0x88, 0x88);
        delay(400);//Attendre l'ACK //A reflechir
        trameV2(adresse_mod, (0xA400 + etatVoies));
      }
      break;
    
    default:
      break;
    }

  }



  //Fin traitement du msg, msg suivant
  FIFO_lecture=(FIFO_lecture+1)%SIZE_FIFO;
}




//Variables pour mesure frequence avec capteur infrarouge
int Dernier_marqueur_Temps=0;
int Valeur_Capteur;
int Temps_Passe_Depuis_Lecture=0;

int Etat_Signal=LOW;
int Etat_Signal_Temporaire;
int nbFrontMontantEtDescendant=0;

int Sensibilite_Capteur=2000; //Sensibilité (tester la valeur en faisant une lecture de l'analog0)
int Nombres_marques_Blanches=1; // Nombre de zones blanches par rotation
long Intervalle_Lecture=34 * 1000; // Délai entre les lectures en microsecondes

void freqMesure(int Valeur_Capteur)
{
  static double freq = 0;

  Temps_Passe_Depuis_Lecture=micros()-Dernier_marqueur_Temps ;
  //Allumage de la led de la carte pour affichage etat capteur

  if(Valeur_Capteur<Sensibilite_Capteur) {Etat_Signal=LOW; }
  else {Etat_Signal=HIGH;}

  //Compte du nombre de changement d'état du capteur
  if(Etat_Signal_Temporaire!=Etat_Signal)
  {
    nbFrontMontantEtDescendant++;
    Etat_Signal_Temporaire=Etat_Signal;
  }

  //Serial.println(Valeur_Capteur);

  if(Temps_Passe_Depuis_Lecture>=Intervalle_Lecture)
  {//Si on a bien patienté durant le délai de lecture
    double periode = Intervalle_Lecture*1e6/(nbFrontMontantEtDescendant/2.0);
    if(periode){freq = 1.0/periode;}else{freq = 0;}
    Serial.println("Hz");
    Serial.println(freq);

    nbFrontMontantEtDescendant=0;
    Dernier_marqueur_Temps=micros();
    
  }

}