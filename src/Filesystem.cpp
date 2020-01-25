#include "Filesystem.h"

#include "OLED.h"

void setupFilesystem() {
  addStatusMessage("init FS");
  if(!LittleFS.begin()) {
    addStatusMessage("FS init failed!!!!");
    delay(5000);
  }
  addStatusMessage("done");
}
