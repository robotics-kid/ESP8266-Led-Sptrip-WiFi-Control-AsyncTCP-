
/* Fills led strip with solid color from recieved data */
void colorEffect()
{
  if (LEDS_TYPE == 1) {
    fill_solid(ledsW, ledsDynamicNumber, CRGBW(WiFiHandler[1], WiFiHandler[2], WiFiHandler[3], WiFiHandler[4]));
  }
  else
  {
    fill_solid(leds, ledsDynamicNumber, CRGB(WiFiHandler[1], WiFiHandler[2], WiFiHandler[3]));
  }
  FastLED.show();
}

// Fills led strip with white color with adjustable color temperature(SK6812 only)
void whiteEffect()
{
  int16_t temp = 0;
  if (LEDS_TYPE == 1)
  {
    temp = WiFiHandler[1];
    temp *= 100;
    temp = map(temp, 1800, 6500, -255, 765); // Map recieved kelvin values into my pseudo range from 0 - 510
    WiFiHandler[2] = map(WiFiHandler[2], 0, 255, 0, MAX_BRIGHTNESS); // Map recieved brightnes value into brightness towards MAX_BRIGHTNES

    FastLED.setBrightness(WiFiHandler[2]);

    if (temp < 0 && temp >= -255)
    {
      fill_solid(ledsW, ledsDynamicNumber, CRGBW(temp * -1, 0, 0, 255));
    }
    else if (temp >= 0 && temp <= 255)
    {
      fill_solid(ledsW, ledsDynamicNumber, CRGBW(temp, temp, temp, 255));
    }
    else if (temp > 255 && temp <= 510)
    {
      fill_solid(ledsW, ledsDynamicNumber, CRGBW(255, 255, 255, 255 - (temp - 255)));
    }
    else if (temp > 510 && temp <= 765)
    {
      fill_solid(ledsW, ledsDynamicNumber, CRGBW(510 - (temp - 255), 510 - (temp - 255), 255, 0));
    }
    FastLED.show();
  }
}

// Fills led strip with static gradient with recieved colors
void gradientEffect_2Val()
{
  if (LEDS_TYPE == 1) {
    fill_gradient_RGBW(ledsW, ledsDynamicNumber, CRGBW( WiFiHandler[1],  WiFiHandler[2],  WiFiHandler[3],  WiFiHandler[4]),
                       CRGBW( WiFiHandler[5],  WiFiHandler[6],  WiFiHandler[7],  WiFiHandler[8]));
  }
  else
  {
    fill_gradient_RGB(leds, ledsDynamicNumber, CRGB( WiFiHandler[1],  WiFiHandler[2],  WiFiHandler[3]),
                      CRGB( WiFiHandler[5],  WiFiHandler[6],  WiFiHandler[7]));
  }
  FastLED.show();
}

uint8_t mix(uint8_t a, uint8_t b, uint16_t pct, uint16_t range) {
  if (pct <= 0) return a;
  if (pct >= range) return b;
  return (((uint32_t)a * (range - pct)) / range) + (((uint32_t)b * pct) / range);
}
