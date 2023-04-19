/*
   Strings.cpp - Library for creating strings for touch screen.
  14 June 2014: Created by Richard Kirkpatrick
   Revision Log:
*/

#include "Arduino.h"
#include "TouchScreenStrings.h"

#include <stdint.h>  // Library for the Seeed Studio TFT Touch Shield
#include <TouchScreen.h>  // Library for the Seeed Studio TFT Touch Shield
#include <TFT.h>      // Library for the Seeed Studio TFT Touch Shield

/*
----------------------------------------------------------------
                   TouchScreenText Superclass
----------------------------------------------------------------
*/

TouchScreenText::TouchScreenText() : xstart(0), ystart(0), fontSize(1), textColor(0xffff)
{

}

TouchScreenText::TouchScreenText(TouchScreenText& otherTouchScreenText)
{
   xstart = otherTouchScreenText.xstart;
   ystart = otherTouchScreenText.ystart;
   fontSize = otherTouchScreenText.fontSize;
   textColor = otherTouchScreenText.textColor;
}


TouchScreenText::TouchScreenText(const int myXStart, const int myYStart, const int myFontSize, unsigned int myTextColor)
    : xstart(myXStart), ystart(myYStart), fontSize(myFontSize), textColor(myTextColor)
{

}


void TouchScreenText::setTextCoord(const int myXStart, const int myYStart)
{
   xstart = myXStart;
   ystart = myYStart;
}


void TouchScreenText::setFontSize(const int myFontSize)
{
   fontSize = myFontSize;
}


void TouchScreenText::setTextColor(unsigned int myTextColor)
{
   textColor = myTextColor;
}


const int TouchScreenText::getXStart()
{
   return xstart;
}


const int TouchScreenText::getYStart()
{
   return ystart;
}


const int TouchScreenText::getFontSize()
{
   return fontSize;
}


const int TouchScreenText::getTextColor()
{
   return textColor;
}


/*
----------------------------------------------------------------
		Functions for the TouchScreenString Subclass
----------------------------------------------------------------
*/

TouchScreenString::TouchScreenString() : TouchScreenText(), text("")
{

}


TouchScreenString::TouchScreenString(TouchScreenString& otherTouchScreenString)
{
  text = otherTouchScreenString.text;
  xstart = otherTouchScreenString.xstart;
  ystart = otherTouchScreenString.ystart;
  fontSize = otherTouchScreenString.fontSize;
  textColor = otherTouchScreenString.textColor;
}


TouchScreenString::TouchScreenString(char* myText, int myXStart, int myYStart, int myFontSize, unsigned int myTextColor)
    : text(myText), TouchScreenText(myXStart, myYStart, myFontSize, myTextColor)
{

}


void TouchScreenString::setValues(char* myText, int myXStart, int myYStart, int myFontSize, unsigned int myTextColor)
{
   text = myText;
   xstart = myXStart;
   ystart = myYStart;
   fontSize = myFontSize;
   textColor = myTextColor;
}


void TouchScreenString::setText(char* myText)
{
   text = myText;
}


char* TouchScreenString::getText()
{
   return text;
}


void TouchScreenString::drawText()
{
   Tft.drawString(text, xstart, ystart, fontSize, textColor);
}

void TouchScreenString::eraseText()
{
    Tft.drawString(text, xstart, ystart, fontSize, 0x0000);
}


/*
----------------------------------------------------------------
		Functions for the TouchScreenChar Subclass
----------------------------------------------------------------
*/

TouchScreenChar::TouchScreenChar() : TouchScreenText(), text('0')
{

}


TouchScreenChar::TouchScreenChar(TouchScreenChar& otherTouchScreenChar)
{
  text = otherTouchScreenChar.text;
  xstart = otherTouchScreenChar.xstart;
  ystart = otherTouchScreenChar.ystart;
  fontSize = otherTouchScreenChar.fontSize;
  textColor = otherTouchScreenChar.textColor;
}


TouchScreenChar::TouchScreenChar(char myText, int myXStart, int myYStart, int myFontSize, unsigned int myTextColor)
    : text(myText), TouchScreenText(myXStart, myYStart, myFontSize, myTextColor)
{

}


void TouchScreenChar::setValues(char myText, int myXStart, int myYStart, int myFontSize, unsigned int myTextColor)
{
   text = myText;
   xstart = myXStart;
   ystart = myYStart;
   fontSize = myFontSize;
   textColor = myTextColor;
}


void TouchScreenChar::setText(char myText)
{
   text = myText;
}


const char TouchScreenChar::getText()
{
   return text;
}


void TouchScreenChar::drawText()
{
   Tft.drawChar(text, xstart, ystart, fontSize, textColor);
}

void TouchScreenChar::eraseText()
{
    Tft.drawChar(text, xstart, ystart, fontSize, 0x0000);
}



