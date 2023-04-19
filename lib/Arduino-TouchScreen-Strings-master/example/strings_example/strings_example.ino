/* 
  Title: TouchScreenStrings Tutorial
  About: Example/tutorial file for how to create strings and chars with the TouchScreenStrings library
  Author: Richard Kirkpatrick
  Date: 20 July 2014
*/

// Libraries
#include <stdint.h>  
#include <TouchScreen.h>  // Library for the Seeed Studio TFT Touch Shield 
#include <TFT.h>      // Library for the Seeed Studio TFT Touch Shield 
#include <TouchScreenStrings.h> // Library for drawing strings for the keypad

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1
#define TS_MINX 140
#define TS_MAXX 900
#define TS_MINY 120
#define TS_MAXY 940

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


void setup() 
{
  Tft.init();             // Initializes the TFT library
}

void loop() 
{
  // Define the touchscreen strings
  TouchScreenString string1("Hello,", 20, 40, 2, RED);
  TouchScreenString string2("Arduino World!", 10, 100, 2, BLUE);
  // Draw the strings
  string1.drawText();
  string2.drawText();
  delay(1000);
  
  // Redefine string2's variables
  string1.eraseText();
  string1.setText("Hola,");
  string1.setFontSize(3);
  string1.drawText();
  // Change string1's color to green
  string2.setTextColor(GREEN);
  string2.drawText();
  delay(1000);
  
  // Test the TouchScreenChar class
  TouchScreenChar char1('$', 100, 200, 3, WHITE);
  char1.drawText();
  delay(1000);
  // Update the char values
  char1.eraseText();
  char1.setTextColor(YELLOW);
  char1.setTextCoord(120, 220);
  char1.setFontSize(4);
  char1.drawText();
  delay(5000);
  
  // Erase the screen
  eraseScreen();
}

void eraseScreen() 
{
  // Erases entire screen
  Tft.fillRectangle(0, 0, 240, 320, BLACK);
}
