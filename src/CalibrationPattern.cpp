#include "CalibrationPattern.h"

#include "AnimationManager.h"
#include "OLED.h"

void setCalibrationPattern(int ind) {
    strip.ClearTo(0, 0, 0);
    strip.SetPixelColor(ind, RgbColor(0, 0, 255));
}
