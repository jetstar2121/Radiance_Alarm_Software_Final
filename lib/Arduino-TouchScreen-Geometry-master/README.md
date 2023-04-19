# Arduino-TouchScreen-Geometry
============================

Library for drawing geometric shapes (polygons, rectangles, triangles, circles) to the Seeed Studio TFT touch screen

## Introduction:
This is a library created for the Arduino Uno and Seeed Studio TFT touch screen (Version 1).  Here is a link to the touch screen hardware:

http://www.seeedstudio.com/wiki/2.8''_TFT_Touch_Shield_V1.0

The user can create geometric shapes which include polygons, triangles, squares and circles.  See the example file for a tutorial on how to do this. The code documentation under the HTML folder also contains the code documentation as well.

## Required Files:
The TFT library comes with the standard Arduino library.  However, you will also need the following library:
1. TouchScreen library (Link: https://github.com/adafruit/Touch-Screen-Library )

## Installation Instructions:
1. Download the zipfile.
2. Create a folder called TouchScreenGeometry under your Arduino libraries folder.
3. Install the TouchScreenGeometry.cpp and TouchScreenGeometry.h files under the newly created folder.
4. All other files are (HTML, Latex, etc.) are not required but are recommended.  They contain the code documentation.

## Documentatation:
The code was thoroughly documented using DOxygen. Please see the HTML folder for detailed code documentation.

## Examples/Tutorials:
See TouchScreenGeometry_example.ino

## Bugs:
There are no known bugs at this time.  If you see any bugs, please contact me and I will do my best to resolve them.  
You may also feel free to resolve them yourself.  Just check with me before you submit any updates.

## Upgrades:
We become better programmers by always looking for ways to improve our code.  Thus, I will always look for ways to improve this code.  If you have any suggestions for improvement or would like to make improvements yourself, please let me know.

Current planned upgrades are:
1. Add rotation methods to Polygon class.
2. Update move methods so that the movement appears more smoother.
3. Add ellipse class.
4. Add fill methods to all geometric classes (Polygon, Triangle, etc.).



