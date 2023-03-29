// Functions File of Radiance_Alarm Software
// Created by Patrick Hanley

#include <includes.h>

// Displays intro GIF
unsigned long display_gif() {
  long start = micros();


  return micros() - start;
}

// Enters home screen menu
unsigned long home_screen() {
  long start = micros();

  tft.fillScreen(MAGENTA);

  return micros() - start;
}