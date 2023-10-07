#include <reception.h>
#include <Protocole.h>



FreqMessage rxMsg[SIZE_FIFO];

unsigned char FIFO_ecriture = 0;

uint8_t pastFreqToHex = 0;

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
            printf("Frequency : %d Hz -> N°%d : 0x%X\n", freq, rxMsg[FIFO_ecriture].nbFreq, freqToHex);               // Print frequency 
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