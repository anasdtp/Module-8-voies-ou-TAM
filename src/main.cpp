#include <Arduino.h>
#include <timerAsserBas.h>
#include <Protocole.h>
#include <signaux.h>
#include <emission.h>
#include <8Voies.h>

#include <reception.h>







uint16_t etatVoies = 0, etatVoiesPrecedent = 0;

//---------
//const int pinSignal = 33; // Numéro de la pin à laquelle le signal est connecté

// void freqMesure(int Valeur_Capteur);
void setup() {
  Serial.begin(921600);
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

  if(trameWAck(adresse_mod, 0x88, 0x88)){
    // delay(400);//Attendre l'ACK au lieu du delay(400)
    // trameWAckV2(adresse_mod, (0xA400 + etatVoies));
  }
  
  
  mscount = 0;

}

void loop() { 
  etatVoies = 0x001*Voie1() | 0x002*Voie2() | 0x004*Voie3() | 0x010*Voie4()
             |0x020*Voie5() | 0x040*Voie6() | 0x100*Voie7() | 0x200*Voie8();

  if(etatVoiesPrecedent != etatVoies){
    trameWAckV2(adresse_mod, (0xA400 + etatVoies));
    etatVoiesPrecedent = etatVoies;
  }
  
  /* //Test lecture freq toutes les 34 ms comme en situation reelle
  int trameTempo[6] = {TON_2, TON_1, TON_A, TON_4, TON_0, TON_1};
  for(int i=0; i<6; i++){
    
    osc_freq = trameTempo[i];
    init_osc_freq ();
    delay(34);
  }
  osc_freq = trameTempo[0];
  init_osc_freq ();
  //*/

  //test Lecture Freq n° Deux : 
  // freqMesure(analogRead(pinSignal));  //********

  TempsEchantionnage(400);
}



//On Hold parce que FrequencyMeter.h fonctionne bien pour le moment

// //Variables pour mesure frequence avec capteur infrarouge
// int Dernier_marqueur_Temps=0;
// int Valeur_Capteur;
// int Temps_Passe_Depuis_Lecture=0;

// int Etat_Signal=LOW;
// int Etat_Signal_Temporaire;
// int nbFrontMontantEtDescendant=0;

// int Sensibilite_Capteur=2000; //Sensibilité (tester la valeur en faisant une lecture de l'analog0)
// int Nombres_marques_Blanches=1; // Nombre de zones blanches par rotation
// long Intervalle_Lecture=34 * 1000; // Délai entre les lectures en microsecondes

// void freqMesure(int Valeur_Capteur)
// {
//   static double freq = 0;

//   Temps_Passe_Depuis_Lecture=micros()-Dernier_marqueur_Temps ;
//   //Allumage de la led de la carte pour affichage etat capteur

//   if(Valeur_Capteur<Sensibilite_Capteur) {Etat_Signal=LOW; }
//   else {Etat_Signal=HIGH;}

//   //Compte du nombre de changement d'état du capteur
//   if(Etat_Signal_Temporaire!=Etat_Signal)
//   {
//     nbFrontMontantEtDescendant++;
//     Etat_Signal_Temporaire=Etat_Signal;
//   }

//   //Serial.println(Valeur_Capteur);

//   if(Temps_Passe_Depuis_Lecture>=Intervalle_Lecture)
//   {//Si on a bien patienté durant le délai de lecture
//     double periode = Intervalle_Lecture*1e6/(nbFrontMontantEtDescendant/2.0);
//     if(periode){freq = 1.0/periode;}else{freq = 0;}
//     Serial.println("Hz");
//     Serial.println(freq);

//     nbFrontMontantEtDescendant=0;
//     Dernier_marqueur_Temps=micros();
    
//   }

// }