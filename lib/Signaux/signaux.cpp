#include <signaux.h>

const int pinSortie = 26;

void init_Signaux(){
  pinMode(pinSortie, OUTPUT);
}

/*output en tension entre 0 et 3.3V*/
void output(double output){
  if(output<0){output = 0;}else if(output>3.3){output = 3.3;}
  uint8_t value = (uint8_t)(output*255.0/3.3);
  dacWrite(pinSortie, value);
  return;
}

/*fonction bloquante faisant la sin pendant une durée en ms*/
void sinusoide(double freq, unsigned long duree){
  unsigned long startTime = micros(); // Obtient le temps de début en centieme de microsecondes

  while ((micros()) - startTime < (duree * 1e3)) { // Continue tant que la durée n'est pas écoulée, en centieme de microsecondes
    output(1.52 + 0.76 * sin(2 * PI * freq * micros()/1e6 ));
  
  }
  output(1.16);

  
  return;
}

/*fonction bloquante faisant le signal carre pendant une durée en ms*/
void ondeCarree(double freq, unsigned long duree) {//duree en ms
  unsigned long startTime = micros(); // Obtient le temps de début en centieme de microsecondes
  unsigned long periode = (1e6 / freq), demiPeriode = periode/2.0; // Période en centieme de microsecondes
  
  while ((micros()) - startTime < (duree * 1e3)) { // Continue tant que la durée n'est pas écoulée, en centieme de microsecondes
    unsigned long tempsPasse = (micros()) - startTime;
    unsigned long demiPeriodeActuelle = tempsPasse % (2*demiPeriode);

    if (demiPeriodeActuelle < (demiPeriode)) {
      // La valeur de l'onde carrée est haute pendant la moitié de la période, puis basse pendant l'autre moitié
      output(2.2); // Valeur maximale (pour un signal carré, vous pouvez ajuster cela selon vos besoins)
    } else {
      output(0.68); // Valeur minimale (pour un signal carré, vous pouvez ajuster cela selon vos besoins)
    }
  }

  output(1.16);
}
