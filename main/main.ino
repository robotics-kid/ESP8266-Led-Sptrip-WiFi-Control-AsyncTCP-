

#include <ESP8266WiFi.h>
//#include <ESPAsyncTCP.h>
#include <DNSServer.h>
//#include <vector>
#include <FastLED.h>
#include <LittleFS.h>

#include "Settings.h"
#include "StripManage.h"
//#include "SPIFFSInteract.ino"

//static std::vector<AsyncClient*> clients; // a list to hold all clients

char recv[argsLen];
char preRecv[argsLen];
char mSend[argsLen];
bool RequestRecieved = true;
bool WiFiConnection = false;

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

  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);
  Serial.println(WiFi.softAPConfig(serverIP, serverIP, IPAddress(255, 255, 255, 0)) ? "Ready" : "Error");
  /*WiFi SetUp*/
  // create access point
  while (!WiFi.softAP(SERVER_HOST_NAME, PASSWORD, 6, false, MAX_CLIENTS)) {
    delay(500);
  }

  server.begin();
  server.setNoDelay(true);
  client.setTimeout(5000);

  /*    // start dns server
      if (!DNS.start(DNS_PORT, SERVER_HOST_NAME, WiFi.softAPIP()))
        Serial.printf("\n Failed to start DNS service \n");

      AsyncServer* server = new AsyncServer(TCP_PORT); // start listening on tcp port 7050
      server->onClient(&handleNewClient, server);
      server->begin();
    //}*/
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

  //Tokenizer(toSPIFFS);
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
  client = server.available(); // Starting WiFi Server

  if (client) // If client is available
  {
    // Printing message of sucefull connection
    if (client.connected())
      Serial.println("Client Connected");

    // Starting client handle(while client is connected)
    while (client.connected())
    {
      memset(recv, '\0', sizeof(recv));

      /*Serial.println();
        Serial.print("mSend: ");
        Serial.println(mSend);*/
      //sendRecieve(); //Function sendRecieve - takes date to send and - return recieved date {String sendRecieve(String, *WiFIClient)}
      client.readStringUntil('!').toCharArray(recv, argsLen); // Converting tmp string to char array recv
      if (!strcmp(preRecv, recv)) {
        //Serial.println("Spam");
        continue;
      }
      /*Serial.print("recv: ");
        Serial.write(recv);
        Serial.println();*/

      //Serial.println(millis());
      char toSPIFFS[argsLen];
      strcpy(toSPIFFS, recv);

      memset(WiFiHandler, '\0', sizeof(handler)*argsLen); // Filling WiFiHandler with NULL values

      switch (Tokenizer(recv))
      {
        case 1: effectHandler(toSPIFFS);
        case 2: handshaking(mSend);
      }

      //Serial.println(millis());
    }
    strcpy(preRecv, recv);
  }
  Serial.println("Client disconnected");
  client.stop();
}
