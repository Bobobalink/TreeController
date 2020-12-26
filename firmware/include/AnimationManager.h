#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const uint16_t pixelCount = 401;
const uint16_t maxAnimations = 512; // including one animation per pixel

extern NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> strip;

extern NeoPixelAnimator animator;

struct LerpState
{
    RgbColor startingColor;
    RgbColor endingColor;
};

void turnStripOff(int blendTimeMs);
void setPixelTo(int pixel, RgbColor color, int blendTimeMs);
int16_t registerAnimation(AnimUpdateCallback callback, int durationMs);

void blendAnimUpdate(const AnimationParam& param);
void setRandomSeed();
void setupAnimation();
void loopAnimation();

#endif
