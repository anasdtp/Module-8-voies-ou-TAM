#include <reception.h>
#include <Protocole.h>
#include <emission.h>



FreqMessage rxMsg[SIZE_FIFO];

unsigned char FIFO_ecriture = 0;

uint8_t pastFreqToHex = 0xF;//Mis à 0xF car cela correspond à 0 Hz

void incrementUsCountTask(void *parameter)
{
    init_frequencyMeter();

    while (1)
    {
        int freq = (int)FrequencyLoop();

        if (freq)
        {
            uint8_t freqToHex = FDC(freq); //Fonction FDC() à corriger -> Marge d'erreur à revoir
            rxMsg[FIFO_ecriture].trame[rxMsg[FIFO_ecriture].nbFreq] = freqToHex;
            Serial.printf("Frequency : %d Hz -> N°%d : 0x%X\n", freq, rxMsg[FIFO_ecriture].nbFreq, freqToHex);               // Print frequency 
            if (pastFreqToHex != freqToHex && freqToHex != 0xF)
            {
                pastFreqToHex = freqToHex;
                rxMsg[FIFO_ecriture].nbFreq++;
            }
            if (rxMsg[FIFO_ecriture].nbFreq >= 6)
            {
                // Alors nous avons reçu toutes la trame
                rxMsg[FIFO_ecriture].nbFreq = 0;//Pour la prochaine boucle

                // On complete la stucture
                rxMsg[FIFO_ecriture].adr    = (rxMsg[FIFO_ecriture].trame[0] <<  4) | (rxMsg[FIFO_ecriture].trame[1]);
                rxMsg[FIFO_ecriture].cmd1   = (rxMsg[FIFO_ecriture].trame[2] <<  4) | (rxMsg[FIFO_ecriture].trame[3]);
                rxMsg[FIFO_ecriture].cmd0   = (rxMsg[FIFO_ecriture].trame[4] <<  4) | (rxMsg[FIFO_ecriture].trame[5]);

                // On passe alors à l'ecriture de la prochaine trame qui pourrait arriver
                FIFO_ecriture = (FIFO_ecriture + 1) % SIZE_FIFO;
            }
        }


    }
}

void init_Reception(){
    // Créez une tâche pour incrémenter uscount sur le deuxième cœur (CORE_1)
  xTaskCreatePinnedToCore(incrementUsCountTask, "IncrementTask", 2048, NULL, 1, NULL, 1);

}   
//------------------------------------------------------------------------------------------------------------------------


void init_ProcessRxTask(){
    xTaskCreatePinnedToCore(ProcessRxTask, "RxTask", 1000, NULL, 1, NULL, 1);
}

void ProcessRx(){//Tel un module 8 voies pour le moment et non pas comme un TAM. Mais c'est similaire
  static signed char FIFO_lecture=0,FIFO_occupation=0,FIFO_max_occupation=0;

  FIFO_occupation=FIFO_ecriture-FIFO_lecture;
  if(FIFO_occupation<0){FIFO_occupation=FIFO_occupation+SIZE_FIFO;}
  if(FIFO_max_occupation<FIFO_occupation){FIFO_max_occupation=FIFO_occupation;}

  if(!FIFO_occupation){/*Alors pas de message*/return;}

  Serial.printf("Recu F%X%X%XF\n", rxMsg[FIFO_lecture].adr, rxMsg[FIFO_lecture].cmd1, rxMsg[FIFO_lecture].cmd0);
  if(rxMsg[FIFO_lecture].adr == adresse_mod){//Alors cela nous concerne
    //Si j'attends un ACK, alors on valide. Sinon on envoie notre ACK
    if(waitingForACK){
        uint16_t cmd_recu = ((rxMsg[FIFO_lecture].cmd1<<8) & 0xFF00) | (rxMsg[FIFO_lecture].cmd0 & 0xFF);
        if(waitingForACK_cmd == cmd_recu){waitingForACK = false;} //ACK recu!
    }else{
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
