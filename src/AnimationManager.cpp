#include "AnimationManager.h"

#include "OLED.h"

NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> strip(pixelCount);

NeoPixelAnimator animator(maxAnimations);

LerpState pixelColors[pixelCount];

void blendAnimUpdate(const AnimationParam& param) {
  // this gets called for each animation on every time step
  // progress will start at 0.0 and end at 1.0
  // we use the blend function on the RgbColor to mix
  // color based on the progress given to us in the animation
  RgbColor updatedColor = RgbColor::LinearBlend(
    pixelColors[param.index].startingColor,
    pixelColors[param.index].endingColor,
    param.progress);
  // apply the color to the strip
  strip.SetPixelColor(param.index, updatedColor);
}

void setPixelTo(int pixel, RgbColor color, int blendTimeMs) {
  if(!animator.IsAnimationActive(pixel)) {
    pixelColors[pixel].startingColor = pixelColors[pixel].endingColor;
    pixelColors[pixel].endingColor = color;
  } else {
    animator.StopAnimation(pixel);
    pixelColors[pixel].startingColor = strip.GetPixelColor(pixel);
    pixelColors[pixel].endingColor = color;
  }
  animator.StartAnimation(pixel, blendTimeMs, blendAnimUpdate);
}

int16_t registerAnimation(AnimUpdateCallback callback, int durationMs) {
  uint16_t nextAnim;
  if(!animator.NextAvailableAnimation(&nextAnim, pixelCount + 1)) {
    return -1;
  }

  animator.StartAnimation(nextAnim, durationMs, callback);
  return nextAnim;
}

void setRandomSeed() {
  uint32_t seed;

  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delay(1);
  for (int shifts = 3; shifts < 31; shifts += 3) {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }

  randomSeed(seed);
}

void setupAnimation() {
  addStatusMessage("Setup animation");
  strip.Begin();
  strip.Show();

  //setRandomSeed(); // esp8266 appears to have a robust hardware RNG that is enabled unless you foolishly try to seed it, which will cause it to fall back to software PRNG
  addStatusMessage("Animation setup done");
}

void loopAnimation() {
  if (animator.IsAnimating())
  {
    animator.UpdateAnimations();
    strip.Show();
  }
}