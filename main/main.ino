
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <DNSServer.h>
#include <vector>
#include <FastLED.h>

#include "Settings.h"
#include "StripManage.h"

static DNSServer DNS;

static std::vector<AsyncClient*> clients; // a list to hold all clients
char recv[argsLen];

void setup() {
  Serial.begin(115200);
  delay(20);

  /*WiFi SetUp*/
  // create access point
  while (!WiFi.softAP(SSID, PASSWORD, 6, false, MAX_CLIENTS)) {
    delay(500);
  }

  // start dns server
  if (!DNS.start(DNS_PORT, SERVER_HOST_NAME, WiFi.softAPIP()))
    Serial.printf("\n Failed to start DNS service \n");

  AsyncServer* server = new AsyncServer(TCP_PORT); // start listening on tcp port 7050
  server->onClient(&handleNewClient, server);
  server->begin();

  /* Initializing leds. Based on LEDS TYPE */
#if LEDS_TYPE == 1
  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(ledsRGB, getRGBWsize(NUM_LEDS)); // Set up for SK6812
#else
  FastLED.addLeds<STRIP_NAME, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS); // Set up for all non white leds
#endif

  /* Configure leds color correction */
  ledsCorrection();

  /* Mounting SPIFFS on ESP */
  if (!SPIFFS.begin()) // Starting SPIFFS
  {
    Serial.printf("\n Error mounting the SPIFFS file system \n");
    return;
  }
}

void loop() {
  DNS.processNextRequest();

  char toSPIFFS[argsLen];
  strcpy(toSPIFFS, recv);

  //Serial.printf("1: %d\n", millis()); // 229633
  if (Tokenizer(recv)) {
    //Serial.printf("2: %d\n", millis()); // 229634
    effectHandler(toSPIFFS);
    //Serial.printf("3: %d\n", millis()); // 229647
  }
}
