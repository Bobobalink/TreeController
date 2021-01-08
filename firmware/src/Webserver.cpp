#include "Webserver.h"

#include "OLED.h"
#include "RandomColors.h"
#include "AnimationManager.h"

ESP8266WebServer server(80);

void handleRoot() {
  addStatusMessage("Got root request");
  char temp[1200];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 1200,

           "<html>\
  <head>\
    <title>ESP8266 Demo</title>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Heap Free: %d</p>\
    <button class=\"putReqs\" id=\"enableRandomFade\">Enbale Random Fade</button><br>\
    <button class=\"putReqs\" id=\"disableRandomFade\">Disable Random Fade</button><br>\
    <button class=\"putReqs\" id=\"off\">Strip Off</button><br>\
    <input class=\"setLED\" id=\"setInd\" type=\"number\" min=0 max=400></input>\
    <button class=\"setLED\" id=\"setBut\">Cal Ind</button><br>\
    <script type=\"text/javascript\">\
      const buttons = document.getElementsByClassName('putReqs');\
      for (let i=0, len=buttons.length|0; i<len; i=i+1|0) {\
        let button = buttons[i];\
        button.addEventListener('click', async _ => {\
          let xhttp = new XMLHttpRequest();\
          xhttp.open('POST', button.id, true);\
          xhttp.send();\
        });\
      }\
      const buttonSend = document.getElementById('setBut');\
      buttonSend.addEventListener('click', async _ => {\
        let xhttp = new XMLHttpRequest();\
        xhttp.open('POST', 'calibrate', true);\
        xhttp.send(document.getElementById('setInd').value);\
      });\
    </script>\
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
  if(server.args() != 1) {
    addStatusMessage("got illegal calibrate message");
    server.send(400, "text/plain", "wrong arg count");
    return;
  }
  int ind = atoi(server.arg(0).c_str());
  char buf[100];
  sprintf(buf, "got calibrate: %d", ind);
  addStatusMessage(buf);
  turnStripOff(0);
  setPixelTo(ind, RgbColor(0, 0, 255), 0);
  server.send(200, "text/plain", "done");
}

void setupWebserver() {
  addStatusMessage("Setup webserver");
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.on("/enableRandomFade", HTTP_POST, [] () {
    addStatusMessage("Got randomFade on");
    enableRandomColors();
    server.send(200, "text/plain", "done");
  });
  server.on("/disableRandomFade", HTTP_POST, [] () {
    addStatusMessage("Got randomFade off");
    disableRandomColors();
    server.send(200, "text/plain", "done");
  });
  server.on("/off", HTTP_POST, [] () {
    addStatusMessage("Got strip off");
    turnStripOff(500);
    server.send(200, "text/plain", "done");
  });
  server.on("/calibrate", HTTP_POST, handleCalibrate);

  server.begin();
  addStatusMessage("Webserver setup done");
}

void loopWebserver() {
  server.handleClient();
}
