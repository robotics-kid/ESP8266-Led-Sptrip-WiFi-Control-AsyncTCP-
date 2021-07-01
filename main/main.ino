

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <DNSServer.h>
#include <vector>
#include <FastLED.h>
#include <LittleFS.h>

#include "Settings.h"
#include "StripManage.h"

static DNSServer DNS;
WiFiClient client;

static std::vector<AsyncClient*> clients; // a list to hold all clients

char recv[argsLen];
char mSend[argsLen];
bool RequestRecieved = true;
bool WiFiConnection = false;
bool msg_recieved = false;

void setup() {
  Serial.begin(115200);
  delay(20);
  
  /*Try connection to existed network*/
  
  /*if ( WiFi.begin(SERVER_HOST_NAME, PASSWORD) == WL_CONNECTED) {

    Serial.println("Connected to wifi");
    Serial.println("\nStarting connection...");

    // if you get a connection, report back via serial:
    if (client.connect(server, TCP_PORT)) {
      Serial.println("connected");
      //      // Make a HTTP request:
      //      client.println("GET /search?q=arduino HTTP/1.0");
      //      client.println();
      //      client.readStringUntil('!');
      WiFiConnection = true;
    }
  }
  else {*/

    /*WiFi SetUp*/
    // create access point
    while (!WiFi.softAP(SERVER_HOST_NAME, PASSWORD, 6, false, MAX_CLIENTS)) {
      delay(500);
    }

    // start dns server
    if (!DNS.start(DNS_PORT, SERVER_HOST_NAME, WiFi.softAPIP()))
      Serial.printf("\n Failed to start DNS service \n");

    AsyncServer* server = new AsyncServer(TCP_PORT); // start listening on tcp port 7050
    server->onClient(&handleNewClient, server);
    server->begin();
  //}
  /* Initializing leds. Based on LEDS TYPE */
#if LEDS_TYPE == 1
  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(ledsRGB, getRGBWsize(NUM_LEDS)); // Set up for SK6812
#else
  FastLED.addLeds<STRIP_NAME, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS); // Set up for all non white leds
#endif

  /* Configure leds color correction */
  ledsCorrection();

  /* Mounting SPIFFS on ESP */
  if (!LittleFS.begin()) // Starting SPIFFS
  {
    Serial.printf("\n Error mounting the SPIFFS file system \n");
    return;
  }

  char fromSPIFFS[argsLen];
  char toSPIFFS[argsLen];

  ReadSPIFFS(fromSPIFFS);

  strcpy(toSPIFFS, fromSPIFFS);
  Serial.println();
  Serial.print("from SPIFFS: ");
  Serial.println(fromSPIFFS);

  Serial.print("TOKENIZER RETURN: ");
  Serial.println(Tokenizer(fromSPIFFS));
  if (Tokenizer(fromSPIFFS) == 1) {
    Serial.println("Effect extracted from SPIFFS");
    effectHandler(toSPIFFS);
  }
}

void loop() {
  DNS.processNextRequest();

  if (msg_recieved){
    char toSPIFFS[argsLen];
    strcpy(toSPIFFS, recv);
    memset(WiFiHandler, '\0', sizeof(handler)*argsLen); // Filling WiFiHandler with NULL values
  
    switch (Tokenizer(recv))
      {
        case 1: effectHandler(toSPIFFS);
        case 2: handshaking(mSend);
      }
      msg_recieved = false;
  }
}
