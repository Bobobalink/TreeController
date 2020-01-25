#include "OLED.h"

char curMessages[NUM_TEXT_ROWS][NUM_TEXT_COLS + 1]; // buffer of current messages, add one to cols to include null terminator

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupOLED() {
  Wire.begin(SDA, SCL);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C, true, false); // Address 0x3C for 128x32

  oled.clearDisplay();
  oled.setTextSize(1);                                    // Normal 1:1 pixel scale
  oled.setTextColor(SSD1306_WHITE, SSD1306_BLACK);        // Draw white text with a black background
  oled.setTextWrap(false);
  oled.setCursor(0,0);

  memset(curMessages, 0, sizeof(char) * NUM_TEXT_ROWS * (NUM_TEXT_COLS + 1)); // make sure the message buffer is actually empty
}

uint8_t spinnerState = 0;
uint8_t spinnerSubState = 0;
char spinnerStates[] = {'-', '\\', '|', '/'};

void addBusySpinner() {
  if(spinnerSubState == 256) {
    spinnerSubState = 0;
    spinnerState++;
    if(spinnerState == 3)
      spinnerState = 0;
    
    oled.drawChar(120, 0, spinnerStates[spinnerState], SSD1306_WHITE, SSD1306_BLACK, 1);
    oled.display();
  }
  spinnerSubState++;
}

// adds a status message to the screen, scrolling the old ones up
void addStatusMessage(const char* message) {
  memmove(curMessages[0], curMessages[1], sizeof(char) * 26 * 3); // copy all the current text up one row
  strncpy(curMessages[3], message, NUM_TEXT_COLS + 1); // copy the new message into the bottom slot, which should now be a duplicate of the one above it

  oled.clearDisplay();
  oled.setCursor(0, 0);
  for(int i = 0; i < NUM_TEXT_ROWS; i++) {
    oled.println(curMessages[i]);
  }
  oled.display();
}

void loopOLED() {
  addBusySpinner();
}
