#include "Wifi.h"

#include <ESP8266WiFi.h>
#include "OLED.h"

#define STASSID "The Kale House"
#define STAPSK  "Time for a walk"

const char* ssid = STASSID;
const char* password = STAPSK;

unsigned long lastMessTime = 0;
char conMess[30];

void setupWifi() {
  addStatusMessage("Setup Wifi");

  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    oled.setCursor(0, 0);
    oled.clearDisplay();
    oled.println(F("\n  Connection Failed!"));
    oled.display();
    delay(5000);
    ESP.restart();
  }

  addStatusMessage("Wifi setup done");
}

void loopWifi() {
  if(WiFi.status() != WL_CONNECTED) {
    addStatusMessage("Connection lost!");
    WiFi.reconnect();
  } else {
    if(millis() - lastMessTime > 10000) { // don't spam the log, but keep the IP address fresh in it
      strcpy(conMess, "IP: ");
      strcat(conMess, WiFi.localIP().toString().c_str());
      addStatusMessage(conMess);
      lastMessTime = millis();
    }
  }
}
