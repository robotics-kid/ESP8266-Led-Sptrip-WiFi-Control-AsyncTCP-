#pragma once

//Definition of led types
//=======================
//Led strip type definition (1 - strip; 2 - matrix)
#if WIDTH == 1 or HEIGHT == 1
#define STRIP_TYPE 1
#else
#define STRIP_TYPE 2
#endif

//Led arrays definition
CRGBW ledsW[PREDEFINED_NUM_LEDS];
CRGB* ledsRGB = (CRGB*)&ledsW[0];               // For WHITE LEDS ONLY
CRGB leds[PREDEFINED_NUM_LEDS];                            // For all NONE WHITE LEDS

//Leds controler name definition
#if LEDS_TYPE == 2
#define STRIP_NAME WS2812B
#elif LEDS_TYPE == 3
#define STRIP_NAME WS2812
#elif LEDS_TYPE == 4
#define STRIP_NAME WS2811
#endif
//=======================

/* Configure leds color correction */
static void ledsCorrection() {
  switch (LEDS_TYPE) {
    case 1:
      FastLED.setCorrection(UncorrectedColor); // SK6812
      FastLED.setCorrection(Typical8mmPixel); // SK6812
    case 2:
      FastLED.setCorrection(TypicalPixelString); //FOR WS2812B
      FastLED.setTemperature(CoolWhiteFluorescent); //FOR WS2812B
    case 3:
    case 4:
      FastLED.setCorrection(UncorrectedColor); // WS2811
      FastLED.setTemperature(CarbonArc); //WS2811
  }

  FastLED.setBrightness(MAX_BRIGHTNESS); // Setting maximum alowed brightness
}
