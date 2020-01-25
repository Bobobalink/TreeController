#include <ESP8266WiFi.h>

#include "AnimationManager.h"
#include "OLED.h"
#include "OTA.h"
#include "RandomColors.h"
#include "Webserver.h"
#include "Wifi.h"
#include "Filesystem.h"
 
void setup() {
  setupOLED();

  setupWifi();
  
  setupOTA();

  setupAnimation();

  setupWebserver();

  enableRandomColors();

  setupFilesystem();
}

void loop() {
  loopWifi();
  
  loopOLED();
  
  loopOTA();

  loopAnimation();

  loopWebserver();

  yield();
}
