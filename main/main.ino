
#include <ESP8266WiFi.h>
//#include <ESPAsyncTCP.h>
//#include <DNSServer.h>
//#include <vector>
#include <FastLED.h>
#include <LittleFS.h>

#include "Settings.h"
#include "StripManage.h"

//static std::vector<AsyncClient*> clients; // a list to hold all clients

char recv[argsLen];
char preRecv[argsLen];
//char mSend[argsLen];
//bool RequestRecieved = true;
//bool WiFiConnection = false;
uint8_t previousCountOfSoftAPConectedStations = 0;

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
  client.setTimeout(500);

  /*    // start dns server
      if (!DNS.start(DNS_PORT, SERVER_HOST_NAME, WiFi.softAPIP()))
        Serial.printf("\n Failed to start DNS service \n");

      AsyncServer* server = new AsyncServer(TCP_PORT); // start listening on tcp port 7050
      server->onClient(&handleNewClient, server);
      server->begin();
    //}*/
  /* Initializing leds. Based on LEDS TYPE */
#if LEDS_TYPE == 1
  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(ledsRGB, getRGBWsize(PREDEFINED_NUM_LEDS)); // Set up for SK6812
#else
  FastLED.addLeds<STRIP_NAME, DATA_PIN, COLOR_ORDER>(leds, PREDEFINED_NUM_LEDS); // Set up for all non white leds
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

  if (!ReadSPIFFS(fromSPIFFS)){
    return;
  }

  strcpy(toSPIFFS, fromSPIFFS);
  Serial.println();
  Serial.print("from SPIFFS: ");
  Serial.println(fromSPIFFS);

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
      memset(WiFiHandler, '\0', sizeof(WiFiHandler)); // Filling WiFiHandler with NULL values

      
      //sendRecieve(); //Function sendRecieve - takes date to send and - return recieved date {String sendRecieve(String, *WiFIClient)}
      String tmp = client.readStringUntil('!');
      
      tmp.toCharArray(recv, argsLen); // Converting tmp string to char array recv
      
      if(WiFi.softAPgetStationNum() < previousCountOfSoftAPConectedStations){
        Serial.println("Socket have been closed due all clients disconnection");
        //client.stop();
        //ESP.restart();
      }

      if (!strcmp(preRecv, recv)) {
        //Serial.println("Spam");
        continue;
      }
      Serial.println(recv);
      
      char toSPIFFS[argsLen];
      strcpy(toSPIFFS, recv);

      switch (Tokenizer(recv))
      {
        case 1: effectHandler(toSPIFFS);
      }

      strcpy(preRecv, toSPIFFS);
      previousCountOfSoftAPConectedStations = WiFi.softAPgetStationNum();
    }
    
  }
  Serial.println("Client disconnected");
  //client.stop();
}
