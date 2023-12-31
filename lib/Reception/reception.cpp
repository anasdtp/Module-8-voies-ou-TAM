#include <reception.h>
#include <Protocole.h>
#include <emission.h>
#include <timerAsserBas.h>




FreqMessage rxMsg[SIZE_FIFO];

unsigned char FIFO_ecriture = 0;

volatile uint8_t nbFreq = 0;      //Nombres de fréquences reçu, avec 6 le maximum
volatile uint8_t pastFreqToHex = 0xF;//Mis à 0xF car cela correspond à 0 Hz
volatile uint32_t start_time_RxMsg_W = 0, start_time_RxMsg_T = 0;//Wait and Timeout

void gestionRx(void *parameter)// Rappel : on ne peut pas utiliser timeMs() dans une interruption!!
{
    static const uint8_t waitingTime = 30/*ms*/, waiting = 1;
    static const int Timeout = 400;//ms  
    static bool activateTimeout = false;  
    static uint8_t etat_reception = 0;
    
    init_frequencyMeter();

    while (1)
    {
        int freq = (int)FrequencyLoop();

        switch (etat_reception)
        {
        case 0:
        {
          if(activateTimeout && (timeMs() - start_time_RxMsg_T) > Timeout){
            activateTimeout = false;
            nbFreq = 0;
            pastFreqToHex = 0xF;
            Serial.printf("Timeout, reset nbFreq\n");
          }
          
          if (freq)
          {
            uint8_t freqToHex = FDC(freq); // Fonction FDC() à corriger -> Marge d'erreur à revoir

            if (freqToHex != 0xF && pastFreqToHex != freqToHex)
            {
              rxMsg[FIFO_ecriture].trame[nbFreq] = ((freqToHex == 0xE && nbFreq) ? rxMsg[FIFO_ecriture].trame[nbFreq - 1] : freqToHex);
              pastFreqToHex = freqToHex;
              nbFreq++;
              activateTimeout = true;
              start_time_RxMsg_T = timeMs();
            }
            Serial.printf("Frequency : %d Hz -> N°%d : 0x%X\n", freq, (nbFreq-1), freqToHex); // Print frequency
            if (nbFreq >= 6)
            {
              // Alors nous avons reçu toutes la trame
              nbFreq = 0;
              pastFreqToHex = 0xF; // Pour la prochaine boucle
              // activateTimeout = false;

              rxMsg[FIFO_ecriture].adr  = (rxMsg[FIFO_ecriture].trame[0] << 4) | (rxMsg[FIFO_ecriture].trame[1]);
              rxMsg[FIFO_ecriture].cmd1 = (rxMsg[FIFO_ecriture].trame[2] << 4) | (rxMsg[FIFO_ecriture].trame[3]);
              rxMsg[FIFO_ecriture].cmd0 = (rxMsg[FIFO_ecriture].trame[4] << 4) | (rxMsg[FIFO_ecriture].trame[5]);

              // On passe alors à l'ecriture de la prochaine trame qui pourrait arriver
              FIFO_ecriture = (FIFO_ecriture + 1) % SIZE_FIFO;

              etat_reception = waiting;
              start_time_RxMsg_W = timeMs();
            }

          }
          
        }
        break;
        case waiting:
        {
          if ((timeMs() - start_time_RxMsg_W) > waitingTime)
          {
            etat_reception = 0;
            Serial.printf("Waiting Time done\n");
          }
        }
        break; 

        default:
          break;
        }
    }
}

void init_Reception(){
    // Créez une tâche pour incrémenter uscount sur le deuxième cœur (CORE_1)
  xTaskCreatePinnedToCore(gestionRx, "GestionRxTask", 2048, NULL, 1, NULL, 1);

  init_ProcessRxTask();

}   
//------------------------------------------------------------------------------------------------------------------------


void init_ProcessRxTask(){
    xTaskCreatePinnedToCore(ProcessRxTask, "RxTask", 2048, NULL, 1, NULL, 1);
}

void ProcessRx(){//Tel un module 8 voies pour le moment et non pas comme un TAM. Mais c'est similaire
  static signed char FIFO_lecture=0,FIFO_occupation=0,FIFO_max_occupation=0;

  FIFO_occupation=FIFO_ecriture-FIFO_lecture;
  if(FIFO_occupation<0){FIFO_occupation=FIFO_occupation+SIZE_FIFO;}
  if(FIFO_max_occupation<FIFO_occupation){FIFO_max_occupation=FIFO_occupation;}

  if(!FIFO_occupation){/*Alors pas de message*/return;}

  Serial.printf("Recu F%2X%2X%2XF\n", rxMsg[FIFO_lecture].adr, rxMsg[FIFO_lecture].cmd1, rxMsg[FIFO_lecture].cmd0);
  if(rxMsg[FIFO_lecture].adr == adresse_mod){//Alors cela nous concerne
    Serial.printf("Trame qui nous concerne, adresse : %2X\n", rxMsg[FIFO_lecture].adr);
    //Si j'attends un ACK, alors on valide. Sinon on envoie notre ACK
    if(waitingForACK){
        Serial.printf("waitingForACK\n");
        uint16_t cmd_recu = ((rxMsg[FIFO_lecture].cmd1<<8) & 0xFF00) | (rxMsg[FIFO_lecture].cmd0 & 0xFF);
        if(waitingForACK_cmd == cmd_recu){waitingForACK = false; Serial.printf("ACK recu! \n");} //ACK recu!
    }else{
        Serial.printf("On envoie l'ACK :\n");
        trame(rxMsg[FIFO_lecture].adr, rxMsg[FIFO_lecture].cmd1, rxMsg[FIFO_lecture].cmd0); // On envoie l'ACK
    }
    

    //Si c'est un TAM on se contente d'afficher ensuite la trame sur un LCD

    //On traite maintenant la commande
    switch (rxMsg[FIFO_lecture].cmd1)
    {
    case 0xD9:
      if(rxMsg[FIFO_lecture].cmd0 == adresse_mod){
        // trame(adresse_mod, 0x88, 0x88);
        // delay(400);//Attendre l'ACK //A reflechir
        // trameV2(adresse_mod, (0xA400 + etatVoies));
      }
      break;
    
    default:
      break;
    }

  }



  //Fin traitement du msg, msg suivant
  FIFO_lecture=(FIFO_lecture+1)%SIZE_FIFO;
}

void ProcessRxTask(void *parameter){

  while(1){

    ProcessRx();

  }
}
