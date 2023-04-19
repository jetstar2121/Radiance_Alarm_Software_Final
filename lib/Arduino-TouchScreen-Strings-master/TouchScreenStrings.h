/*! \mainpage Main Page
 *
 * \section intro_sec Introduction
 * \author Richard Kirkpatrick
 * \date 17 July 2014
 * \copyright GNU Public License.
 *
 * This is the Arduino library for creating geometries shapes for the Seeed Studio TFT touch screen (Version 1).
 * The user can create polygons, rectangles, triangle and circles.
 * See the Wiki documentation page for more info!
 *
 *
 */

#ifndef TouchScreenStrings_h
#define TouchScreenStrings_h

#include "Arduino.h"

/*
----------------------------------------------------------------
				  TouchScreenText Superclass
----------------------------------------------------------------
*/

///@class TouchScreenText
///@brief Base class for drawing text to the touch screen.
class TouchScreenText {
public:

    /*!
    @brief Default constructor for the TouchScreenText superclass
    */
    TouchScreenText();

    /*!
    @class TouchScreenText
    @brief Parameter constructor for the TouchScreenText superclass
    @param  otherTouchScreenText The text that is to be copied.
    */
    TouchScreenText(TouchScreenText& otherTouchScreenText);


    /*!
    @brief Parameter constructor for the TouchScreenText superclass
    @param  myXStart    The x-coordinate for the text instance.
    @param  myYStart    The y-coordinate for the text instance.
    @param  myFontSize  The font size of the text instance.
    @param  myTextColor     The color of the text instance. Default color is WHITE.
    */

    TouchScreenText(const int myXStart, const int myYStart, const int myFontSize, unsigned int myTextColor);

    /*!
    @brief Sets the coordinates of the text.
    @param  myXStart    The x-coordinate for the text instance.
    @param  myYStart    The y-coordinate for the text instance.
    */

    void setTextCoord(const int myXStart, const int myYStart);

    /*!
    @brief Sets the font size of the text.
    @param  myFontSize  The font size of the text instance.
    */
    void setFontSize(const int myFontSize = 2);

    /*!
    @brief Sets the color of the text.
    @param  myTextColor The color of the text instance.
    */
    void setTextColor(unsigned int myTextColor = 0xffff);  // Default color is WHITE.

    /*!
    @brief Gets the xcoordinate of the text.
    @return The x-coordinate for the text instance.
    */
    const int getXStart();

    /*!
    @brief Gets the ycoordinate of the text.
    @return The y-coordinate for the text instance.
    */
    const int getYStart();

    /*!
    @brief Gets the font size of the text.
    @return The font size for the text instance.
    */
    const int getFontSize();

    /*!
    @brief Gets the font color of the text.
    @return The font color for the text instance.
    */
    const int getTextColor();

protected:
  int xstart, ystart; /// Coordinates of the text
  int fontSize; /// Size of the text
  unsigned int textColor; /// Color of the text
};

/*
----------------------------------------------------------------
				  TouchScreenString Subclass
----------------------------------------------------------------
*/

/*!
@class TouchScreenString
@brief Subclass of TouchScreenText. Abstract class for drawing strings to the touch screen.
*/
class TouchScreenString : public TouchScreenText {
 public:
    /*!
    @brief Default constructor for the TouchScreenString subclass.
    @param  text The text that is to be drawn.
    */
    TouchScreenString();

    /*!
    @brief Copy constructor for the TouchScreenString superclass
    @param  otherTouchScreenString The string instance that is to be copied.
    */
    TouchScreenString(TouchScreenString& otherTouchScreenString);

    /*!
    @brief Parameter constructor for the TouchScreenString superclass.
    @param  myText The text that is to be drawn.
    @param  myXStart The x-coordinate for the text instance.
    @param  myYStart The y-coordinate for the text instance.
    @param  myFontSize The font size of the text instance.
    @param  myTextColor The color of the text instance.
    */
    TouchScreenString(char* myText, int myXStart, int myYStart, int myFontSize, unsigned int myTextColor);

    /*!
    @brief Sets the text, coordinates, font size, and text color of the instance.
    @param  myText The text that is to be drawn.
    @param  myXStart The x-coordinate for the text instance.
    @param  myYStart The y-coordinate for the text instance.
    @param  myFontSize The font size of the text instance.
    @param  myTextColor The color of the text instance. Default color is WHITE.
    */
    void setValues(char* myText, int myXStart, int myYStart, int myFontSize, unsigned int myTextColor);

    /*!
    @brief Sets the text of the string instance.
    @param  myText The text that is to be drawn.
    */
    void setText(char* myText);

    /*!
    @brief Returns the text of the string instace.
    @return text The text that is to be drawn.
    */
    char* getText();

    ///@brief Uses the TFT library to draw the string instance.
    void drawText();

    ///@brief Erases the text
    void eraseText();

 private:
    char* text;
};

/*
----------------------------------------------------------------
				  TouchScreenChar Subclass
----------------------------------------------------------------
*/

///@class TouchScreenChar
///@brief Subclass of TouchScreenText.  Abstract class used for drawing char's to the touch screen.
class TouchScreenChar : public TouchScreenText {
 public:
    /*!
    @brief Default constructor for the TouchScreenString subclass.
    */
    TouchScreenChar();

    /*!
    @brief Copy constructor for the TouchScreenString subclass.
    @param  otherTouchScreenChar The char instance that is being copied.
    */
    TouchScreenChar(TouchScreenChar& otherTouchScreenChar);

    /*!
    @brief Parameter constructor for the TouchScreenString superclass.
    @param  myText The char that is to be drawn.
    @param  myXStart The x-coordinate for the char instance.
    @param  myYStart The y-coordinate for the char instance.
    @param  myFontSize The font size of the char instance.
    @param  myTextColor The color of the char instance.
    */
    TouchScreenChar(char myText, int myXStart, int myYStart, int myFontSize, unsigned int myTextColor);

    /*!
    @brief Sets the char, cooordinates, font size, and char color.
    @param  myText The char that is to be drawn.
    @param  myXStart The x-coordinate for the char instance.
    @param  myYStart The y-coordinate for the char instance.
    @param  myFontSize The font size of the char instance.
    @param  myTextColor The color of the char instance.
    */
    void setValues(char myText, int myXStart, int myYStart, int myFontSize, unsigned int myTextColor);

    /*!
    @brief Sets the text of the char instance.
    @param  myText The char that is to be drawn.
    */
    void setText(char myText);

    /*!
    @brief Returns the char of the char instance.
    @return The char that is to be drawn.
    */
    const char getText();

    ///@brief Uses the Seeed Studio library to draw the char.
    void drawText();

    ///@brief Erases the text
    void eraseText();

 private:
    char text;
};


#endif
