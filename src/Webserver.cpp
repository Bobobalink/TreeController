#include "Webserver.h"

#include "OLED.h"
#include "RandomColors.h"
#include "AnimationManager.h"

ESP8266WebServer server(80);

void handleRoot() {
  addStatusMessage("Got root request");
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Heap Free: %d</p>\
  </body>\
</html>",

           hr, min % 60, sec % 60,
           ESP.getFreeHeap()
          );
  server.send(200, "text/html", temp);
}

void handleNotFound() {
  addStatusMessage("Got invalid request");
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void handleCalibrate() {
  if(server.args() != 1)
    return;
}

void setupWebserver() {
  addStatusMessage("Setup webserver");
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.on("/enableRandomFade", HTTP_PUT, [] () {
    addStatusMessage("Got randomFade on");
    enableRandomColors();
  });
  server.on("/disableRandomFade", HTTP_PUT, [] () {
    addStatusMessage("Got randomFade off");
    disableRandomColors();
  });
  server.on("/off", HTTP_PUT, [] () {
    addStatusMessage("Got strip off");
    strip.ClearTo(RgbColor(0, 0, 0));
  });
  server.on("/calibrate", HTTP_PUT, handleCalibrate);

  server.begin();
  addStatusMessage("Webserver setup done");
}

void loopWebserver() {
  server.handleClient();
}
