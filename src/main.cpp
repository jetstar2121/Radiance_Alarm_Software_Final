// Main File of Radiance_Alarm Software
// Created by Patrick Hanley

#include <includes.h>

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts(11, A4, A5, 10, 300);

#define BUFFPIXEL 20



uint16_t read16(File f)
{
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f)
{
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

void bmpDraw(char *filename, int x, int y)
{

  File bmpFile;
  int bmpWidth, bmpHeight;            // W+H in pixels
  uint8_t bmpDepth;                   // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;            // Start of image data in file
  uint32_t rowSize;                   // Not always = bmpWidth; may have padding
  uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[BUFFPIXEL];      // pixel out buffer (16-bit per pixel)
  uint8_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean goodBmp = false;            // Set to true on valid header parse
  boolean flip = true;                // BMP is stored bottom-to-top
  int w, h, row, col;
  uint8_t r, g, b;
  uint32_t pos = 0, startTime = millis();
  uint8_t lcdidx = 0;
  boolean first = true;

  if ((x >= tft.width()) || (y >= tft.height()))
    return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL)
  {
    Serial.println(F("File not found"));
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42)
  { // BMP signature
    Serial.println(F("File size: "));
    Serial.println(read32(bmpFile));
    (void)read32(bmpFile);            // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: "));
    Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: "));
    Serial.println(read32(bmpFile));
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1)
    {                             // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: "));
      Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0))
      { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0)
        {
          bmpHeight = -bmpHeight;
          flip = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())
          w = tft.width() - x;
        if ((y + h - 1) >= tft.height())
          h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

        for (row = 0; row < h; row++)
        { // For each scanline...
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos)
          { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col = 0; col < w; col++)
          { // For each column...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer))
            { // Indeed
              // Push LCD buffer to the display first
              if (lcdidx > 0)
              {
                tft.pushColors(lcdbuffer, lcdidx, first);
                lcdidx = 0;
                first = false;
              }
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++] = tft.color565(r, g, b);
          } // end pixel
        }   // end scanline
        // Write any remaining data to LCD
        if (lcdidx > 0)
        {
          tft.pushColors(lcdbuffer, lcdidx, first);
        }
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if (!goodBmp)
    Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

void set_text(int x, int y, uint16_t color, int size)
{
  tft.setCursor(x, y), tft.setTextColor(color), tft.setTextSize(size);
}

#pragma region WIP
void homeScreen()
{
  tft.fillScreen(BLACK);
  tft.drawPixel(15, 31, WHITE);
  tft.drawPixel(95, 325, WHITE);
  tft.drawPixel(106, 149, WHITE);
  tft.drawPixel(198, 206, WHITE);
  tft.drawPixel(202, 455, WHITE);
  tft.drawPixel(199, 89, WHITE);
  tft.drawPixel(246, 58, WHITE);
  tft.drawPixel(286, 121, WHITE);
  tft.drawPixel(37, 231, WHITE);
  tft.drawPixel(231, 438, WHITE);
  tft.drawPixel(178, 131, WHITE);
  tft.drawPixel(297, 56, WHITE);
  tft.drawPixel(233, 160, WHITE);
  tft.drawPixel(58, 25, WHITE);
  tft.drawPixel(179, 300, WHITE);
  tft.drawPixel(178, 284, WHITE);

  tft.drawPixel(293, 418, WHITE);
  tft.drawPixel(207, 467, WHITE);
  tft.drawPixel(84, 270, WHITE);
  tft.drawPixel(115, 40, WHITE);
  tft.drawPixel(291, 99, WHITE);
  tft.drawPixel(173, 173, WHITE);
  tft.drawPixel(174, 28, WHITE);
  tft.drawPixel(276, 272, WHITE);
  tft.drawPixel(221, 377, WHITE);
  tft.drawPixel(453, 8, WHITE);
  tft.drawPixel(242, 104, WHITE);
  tft.drawPixel(36, 236, WHITE);
  tft.drawPixel(210, 196, WHITE);
  tft.drawPixel(320, 137, WHITE);
  tft.drawPixel(43, 320, WHITE);
  tft.drawPixel(107, 281, WHITE);

  tft.drawPixel(40, 115, WHITE);
  tft.drawPixel(99, 291, WHITE);
  tft.drawPixel(173, 173, WHITE);
  tft.drawPixel(28, 174, WHITE);
  tft.drawPixel(272, 276, WHITE);
  tft.drawPixel(377, 221, WHITE);
  tft.drawPixel(8, 453, WHITE);

  tft.setCursor(0, 0), tft.setTextColor(WHITE), tft.setTextSize(3);

  tft.fillRoundRect(5, 0, 470, 50, 50, ORANGE);
  tft.setCursor(80, 15), tft.setTextColor(WHITE), tft.setTextSize(3);
  tft.println("WELCOME TO RADIANCE");
  set_text(5, 311, WHITE, 1);
  tft.println("Programmed by Patrick Hanley");
}
#pragma endregion WIP

boolean timer(int minutes, int seconds)
{
  int totalSeconds = 0;
  totalSeconds = seconds + (minutes * 60);

  for (int i = totalSeconds; i < 0; i--)
  {
    delay(1000);
  }

  return true;
}

class Button
{
public:
  bool status;
  uint16_t offColor;
  uint16_t onColor;
};
class Square : public Button
{
public:
  int xPos;
  int yPos;
  int width;
  int height;

  Square(int xPos, int yPos, int width, int height, uint16_t offColor, uint16_t onColor)
  {
    status = false;
    this->xPos = xPos;
    this->yPos = yPos;
    this->width = width;
    this->height = height;
    this->offColor = offColor;
    this->onColor = onColor;
  }

  void set_status(bool stat)
  {
    this->status = stat;
  }

  void activate()
  {
    if (this->status)
    {
      tft.fillRect(this->xPos, this->yPos, this->width, this->height, this->onColor);
    }
    else
    {
      tft.fillRect(this->xPos, this->yPos, this->width, this->height, this->offColor);
    }
  }
};
class RoundRect : public Button
{
public:
  int xPos;
  int yPos;
  int width;
  int height;
  int radius;

  RoundRect()
  {
    status = false;
    this->xPos = 0;
    this->yPos = 0;
    this->width = 0;
    this->height = 0;
    this->radius = 0;
    this->offColor = RED;
    this->onColor = GREEN;
  }

  RoundRect(int xPos, int yPos, int width, int height, int radius, uint16_t offColor, uint16_t onColor)
  {
    status = false;
    this->xPos = xPos;
    this->yPos = yPos;
    this->width = width;
    this->height = height;
    this->radius = radius;
    this->offColor = offColor;
    this->onColor = onColor;
  }

  void set_status(bool stat)
  {
    this->status = stat;
  }

  void activate()
  {
    if (this->status)
    {
      tft.fillRoundRect(this->xPos, this->yPos, this->width, this->height, this->radius, this->onColor);
    }
    else
    {
      tft.fillRoundRect(this->xPos, this->yPos, this->width, this->height, this->radius, this->offColor);
    }
  }
};
class Dropdown : public RoundRect
{
public:
  String menu_options[10];

  Dropdown(int xPos, int yPos, int width, int height, int radius, uint16_t offColor, uint16_t onColor, String options[10])
  {
    status = false;
    this->xPos = xPos;
    this->yPos = yPos;
    this->width = width;
    this->height = height;
    this->radius = radius;
    this->offColor = offColor;
    this->onColor = onColor;
    memcpy(menu_options, options, 10);
  }
  void activate()
  {
    if (ts.pressure() > 0)
    {
      if (this->status)
      {

        tft.fillRoundRect(this->xPos, this->yPos, this->width, this->height + 150, this->radius - 25, this->onColor);
        tft.drawRoundRect(this->xPos, this->yPos, this->width, this->height, this->radius, BLACK);
      }
      else
      {
        tft.fillRoundRect(this->xPos, this->yPos, this->width, this->height + 150, this->radius - 25, BLUE);
        tft.fillRoundRect(this->xPos, this->yPos, this->width, this->height, this->radius, this->offColor);
      }
    }
  }
};

bool hours_active;
bool minutes_active;

Square button1(0, 0, 100, 100, RED, GREEN);
// SETUP
void setup(void)
{

  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.8\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is ");
  Serial.print(tft.width());
  Serial.print("x");
  Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();

  if (identifier == 0x9325)
  {
    Serial.println(F("Found ILI9325 LCD driver"));
  }
  else if (identifier == 0x9328)
  {
    Serial.println(F("Found ILI9328 LCD driver"));
  }
  else if (identifier == 0x7575)
  {
    Serial.println(F("Found HX8347G LCD driver"));
  }
  else if (identifier == 0x9341)
  {
    Serial.println(F("Found ILI9341 LCD driver"));
  }
  else if (identifier == 0x8357)
  {
    Serial.println(F("Found HX8357D LCD driver"));
  }
  else
  {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);

  Serial.println(F("Benchmark                Time (microseconds)"));

  tft.setRotation(1);

#pragma region stars -Wno-unknown-pragmas
  tft.fillScreen(BLACK);
  tft.drawPixel(15, 31, WHITE);
  tft.drawPixel(95, 325, WHITE);
  tft.drawPixel(106, 149, WHITE);
  tft.drawPixel(198, 206, WHITE);
  tft.drawPixel(202, 455, WHITE);
  tft.drawPixel(199, 89, WHITE);
  tft.drawPixel(246, 58, WHITE);
  tft.drawPixel(286, 121, WHITE);
  tft.drawPixel(37, 231, WHITE);
  tft.drawPixel(231, 438, WHITE);
  tft.drawPixel(178, 131, WHITE);
  tft.drawPixel(297, 56, WHITE);
  tft.drawPixel(233, 160, WHITE);
  tft.drawPixel(58, 25, WHITE);
  tft.drawPixel(179, 300, WHITE);
  tft.drawPixel(178, 284, WHITE);

  tft.drawPixel(293, 418, WHITE);
  tft.drawPixel(207, 467, WHITE);
  tft.drawPixel(84, 270, WHITE);
  tft.drawPixel(115, 40, WHITE);
  tft.drawPixel(291, 99, WHITE);
  tft.drawPixel(173, 173, WHITE);
  tft.drawPixel(174, 28, WHITE);
  tft.drawPixel(276, 272, WHITE);
  tft.drawPixel(221, 377, WHITE);
  tft.drawPixel(453, 8, WHITE);
  tft.drawPixel(242, 104, WHITE);
  tft.drawPixel(36, 236, WHITE);
  tft.drawPixel(210, 196, WHITE);
  tft.drawPixel(320, 137, WHITE);
  tft.drawPixel(43, 320, WHITE);
  tft.drawPixel(107, 281, WHITE);

  tft.drawPixel(40, 115, WHITE);
  tft.drawPixel(99, 291, WHITE);
  tft.drawPixel(173, 173, WHITE);
  tft.drawPixel(28, 174, WHITE);
  tft.drawPixel(272, 276, WHITE);
  tft.drawPixel(377, 221, WHITE);
  tft.drawPixel(8, 453, WHITE);

  tft.drawPixel(31, 15, WHITE);
  tft.drawPixel(325, 95, WHITE);
  tft.drawPixel(149, 106, WHITE);
  tft.drawPixel(206, 198, WHITE);
  tft.drawPixel(455, 202, WHITE);
  tft.drawPixel(89, 199, WHITE);
  tft.drawPixel(58, 246, WHITE);
  tft.drawPixel(121, 286, WHITE);
  tft.drawPixel(231, 37, WHITE);
  tft.drawPixel(438, 231, WHITE);
  tft.drawPixel(131, 178, WHITE);
  tft.drawPixel(56, 297, WHITE);
  tft.drawPixel(160, 233, WHITE);
  tft.drawPixel(25, 58, WHITE);
  tft.drawPixel(300, 179, WHITE);
  tft.drawPixel(284, 178, WHITE);

#pragma endregion stars -Wno-unknown-pragmas

  tft.setCursor(0, 0), tft.setTextColor(WHITE), tft.setTextSize(3);

  tft.fillRoundRect(5, 0, 470, 50, 50, ORANGE);
  tft.setCursor(80, 15), tft.setTextColor(WHITE), tft.setTextSize(3);
  tft.println("WELCOME TO RADIANCE");
  set_text(5, 311, WHITE, 1);
  tft.println("Programmed by Patrick Hanley");

  // tft.fillRoundRect(0, BOXSIZE*4, BOXSIZE*4, BOXSIZE, 100, ORANGE);
  // tft.fillRoundRect(0, BOXSIZE*5, BOXSIZE*4, BOXSIZE, 100, ORANGE);
  //  tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, WHITE);

  // tft.drawRoundRect(100, BOXSIZE*3, BOXSIZE*4, BOXSIZE, 100, WHITE);

  bmpDraw("woof.bmp", 0, 0);

  tft.fillRoundRect(150, BOXSIZE * 2, BOXSIZE * 1.5, BOXSIZE, 100, ORANGE);
  set_text(160, 93, WHITE, 2);

  tft.print(12);

  tft.fillRoundRect(205, BOXSIZE * 2, BOXSIZE * 1.5, BOXSIZE, 100, ORANGE);

  set_text(215, 93, WHITE, 2);

  tft.print("00");

  delayMicroseconds(2000);
}


#pragma region timer_buttons
//Hours OFF
void drawHours()
{
  tft.fillRoundRect(150, BOXSIZE * 2, BOXSIZE * 1.5, BOXSIZE * 4, 40, BLACK);
  tft.fillRect(90, 140, 120, 160, BLACK);
  tft.fillRoundRect(150, BOXSIZE * 2, BOXSIZE * 1.5, BOXSIZE, 100, ORANGE);
  set_text(160, 93, WHITE, 2);

  tft.print(12);
}

//Hours ON
void drawActiveHours()
{
  tft.fillRoundRect(150, BOXSIZE * 2, BOXSIZE * 1.5, BOXSIZE * 4, 40, ORANGE);
  set_text(160, 93, WHITE, 2);

  tft.fillRect(90, 140, 120, 160, ORANGE);

  int count = 0;

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      tft.drawRect(90 + (BOXSIZE * j), 140 + (BOXSIZE * i), BOXSIZE, BOXSIZE, WHITE);
      set_text(90 + (BOXSIZE * j), 143 + (BOXSIZE * i), WHITE, 2);
      if (count <= 9)
      {
        tft.print(count);
      }
      else if (count == 10)
      {
        tft.print("BCS");
      }
      else if (count == 11)
      {
        tft.print("CHK");
      }
      count++;
    }
  }
}

//Minutes OFF
void drawMinutes()
{
  tft.fillRoundRect(211, BOXSIZE * 2, BOXSIZE * 1.5, BOXSIZE * 4, 40, BLACK);
  tft.fillRect(211, 140, 120, 160, BLACK);

  tft.fillRoundRect(211, BOXSIZE * 2, BOXSIZE * 1.5, BOXSIZE, 100, ORANGE);
  set_text(221, 93, WHITE, 2);
  tft.print("00");
}


//Minutes ON
void drawActiveMinutes()
{
  tft.fillRoundRect(211, BOXSIZE * 2, BOXSIZE * 1.5, BOXSIZE * 4, 40, ORANGE);
  set_text(221, 93, WHITE, 2);

  tft.fillRect(211, 140, 120, 160, ORANGE);

  int count = 0;

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      tft.drawRect(211 + (BOXSIZE * j), 140 + (BOXSIZE * i), BOXSIZE, BOXSIZE, WHITE);
      set_text(214 + (BOXSIZE * j), 143 + (BOXSIZE * i), WHITE, 2);
      if (count <= 9)
      {
        tft.print(count);
      }
      else if (count == 10)
      {
        tft.print("BCS");
      }
      else if (count == 11)
      {
        tft.print("CHK");
      }
      count++;
    }
  }
}

void check_clock() {
    if (hours_active)
  {
    drawActiveHours();
  }
  else
  {
    drawHours();
  }
    if (minutes_active)
  {
    drawActiveMinutes();
  }
  else
  {
    drawMinutes();
  }
}

void toggle_hours() {
  hours_active =! hours_active;
  check_clock();
}
void toggle_minutes() {
  minutes_active =! minutes_active;
  check_clock();
}
void set_hours(bool status) {
  hours_active = status;
}
void set_minutes(bool status) {
  minutes_active = status;
}
#pragma endregion timer_buttons

// Dropdown(int xPos, int yPos, int width, int height, int radius, uint16_t offColor, uint16_t onColor, String options[10])
//  LOOP
void loop(void)
{



  toggle_hours();
  delay(3000);
  toggle_minutes();


  
}
