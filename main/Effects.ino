
/* Fills led strip with solid color from recieved data */
void colorEffect()
{
  if (LEDS_TYPE == 1) {
    fill_solid(ledsW, NUM_LEDS, CRGBW(WiFiHandler[1].handlerVal, WiFiHandler[2].handlerVal, WiFiHandler[3].handlerVal, WiFiHandler[4].handlerVal));
  }
  else
  {
    fill_solid(leds, NUM_LEDS, CRGB(WiFiHandler[1].handlerVal, WiFiHandler[2].handlerVal, WiFiHandler[3].handlerVal));
  }
  FastLED.show();
}

// Fills led strip with white color with adjustable color temperature(SK6812 only)
void whiteEffect()
{
  if (LEDS_TYPE == 1)
  {

    WiFiHandler[1].handlerVal = map(WiFiHandler[1].handlerVal, 1800, 6500, -255, 765); // Map recieved kelvin values into my pseudo range from 0 - 510
    WiFiHandler[2].handlerVal = map(WiFiHandler[2].handlerVal, 0, 255, 0, MAX_BRIGHTNESS); // Map recieved brightnes value into brightness towards MAX_BRIGHTNES

    FastLED.setBrightness(WiFiHandler[2].handlerVal);

    if (WiFiHandler[1].handlerVal < 0 && WiFiHandler[1].handlerVal >= -255)
    {
      fill_solid(ledsW, NUM_LEDS, CRGBW(WiFiHandler[1].handlerVal * -1, 0, 0, 255));
    }
    else if (WiFiHandler[1].handlerVal >= 0 && WiFiHandler[1].handlerVal <= 255)
    {
      fill_solid(ledsW, NUM_LEDS, CRGBW(WiFiHandler[1].handlerVal, WiFiHandler[1].handlerVal, WiFiHandler[1].handlerVal, 255));
    }
    else if (WiFiHandler[1].handlerVal > 255 && WiFiHandler[1].handlerVal <= 510)
    {
      fill_solid(ledsW, NUM_LEDS, CRGBW(255, 255, 255, 255 - (WiFiHandler[1].handlerVal - 255)));
    }
    else if (WiFiHandler[1].handlerVal > 510 && WiFiHandler[1].handlerVal <= 765)
    {
      fill_solid(ledsW, NUM_LEDS, CRGBW(510 - (WiFiHandler[1].handlerVal - 255), 510 - (WiFiHandler[1].handlerVal - 255), 255, 0));
    }
    FastLED.show();
  }
}

// Fills led strip with static gradient with recieved colors
void gradientEffect_2Val()
{
  if (LEDS_TYPE == 1) {
    fill_gradient_RGBW(ledsW, NUM_LEDS, CRGBW( WiFiHandler[1].handlerVal,  WiFiHandler[2].handlerVal,  WiFiHandler[3].handlerVal,  WiFiHandler[4].handlerVal),
                       CRGBW( WiFiHandler[5].handlerVal,  WiFiHandler[6].handlerVal,  WiFiHandler[7].handlerVal,  WiFiHandler[8].handlerVal));
  }
  else
  {
    fill_gradient_RGB(leds, NUM_LEDS, CRGB( WiFiHandler[1].handlerVal,  WiFiHandler[2].handlerVal,  WiFiHandler[3].handlerVal),
                      CRGB( WiFiHandler[5].handlerVal,  WiFiHandler[6].handlerVal,  WiFiHandler[7].handlerVal));
  }
  FastLED.show();
}

uint8_t mix(uint8_t a, uint8_t b, uint16_t pct, uint16_t range) {
  if (pct <= 0) return a;
  if (pct >= range) return b;
  return (((uint32_t)a * (range - pct)) / range) + (((uint32_t)b * pct) / range);
}
