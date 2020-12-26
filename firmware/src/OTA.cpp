#include "OTA.h"
#include "OLED.h"
#include "AnimationManager.h"

#include <ArduinoOTA.h>

void setupOTA() {
  addStatusMessage("setup OTA");

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("treeController");

  ArduinoOTA.setPassword("dasBlinkenLightsen");

  ArduinoOTA.onStart([]() {
    strip.ClearTo(RgbColor(0));
    strip.Show();
    addStatusMessage("Starting OTA update");
  });
  
  ArduinoOTA.onEnd([]() {
    addStatusMessage("OTA update complete");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR) {
      addStatusMessage("OTA failed: AUTH");
    } else if (error == OTA_BEGIN_ERROR) {
      addStatusMessage("OTA failed: BEGIN");
    } else if (error == OTA_CONNECT_ERROR) {
      addStatusMessage("OTA failed: CONNECT");
    } else if (error == OTA_RECEIVE_ERROR) {
      addStatusMessage("OTA failed: RECEIVE");
    } else if (error == OTA_END_ERROR) {
      addStatusMessage("OTA failed: END");
    }
  });
  
  ArduinoOTA.begin();
  
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  addStatusMessage("OTA setup done");
}

void loopOTA() {
  ArduinoOTA.handle();
}
