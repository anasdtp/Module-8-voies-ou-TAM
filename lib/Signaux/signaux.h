#include <Arduino.h>

extern const int pinSortie;

void init_Signaux();
void output(double output);
void sinusoide(double freq, unsigned long duree);
void ondeCarree(double freq, unsigned long duree);