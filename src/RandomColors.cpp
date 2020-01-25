#include "RandomColors.h"

#include "AnimationManager.h"

int16_t nextColorAnimation;

void pickRandom(AnimationParam param)
{
  if(param.state == AnimationState_Completed) {
    float luminance = 0.4;
    
    // pick random count of pixels to animate
    uint16_t count = random(4);
    while (count > 0)
    {
      // pick a random pixel
      uint16_t pixel = random(pixelCount);

      // pick random time and random color
      // we use HslColor object as it allows us to easily pick a color
      // with the same saturation and luminance 
      uint16_t fadeTime = random(300, 900);

      if(!animator.IsAnimationActive(pixel)) {
        setPixelTo(pixel, HslColor(random(360) / 360.0f, 1.0f, luminance), fadeTime);
        count--;
      } else {
        yield(); // make sure we don't stall out the wifi stack and stuff while waiting for an LED to be free
      }
    }

    animator.ChangeAnimationDuration(nextColorAnimation, random(7));
    animator.RestartAnimation(nextColorAnimation); // start a new animation to do this again
  }
}

void enableRandomColors() {
  nextColorAnimation = registerAnimation(pickRandom, random(7));
}

void disableRandomColors() {
  animator.StopAnimation(nextColorAnimation);
}
