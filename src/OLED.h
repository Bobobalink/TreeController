#ifndef OLED_H
#define OLED_H

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define NUM_TEXT_COLS 22 // size of the screen in characters (128 / 6 I think)
#define NUM_TEXT_ROWS 4

#define SDA             4 // i2c pins for OLED bus
#define SCL             5
#define OLED_RESET     16 // Reset pin # (or -1 if sharing Arduino reset pin)

extern Adafruit_SSD1306 oled;

void setupOLED();
void loopOLED();

// adds a status message to the screen, scrolling the old ones up
void addStatusMessage(const char* message);

#endif
