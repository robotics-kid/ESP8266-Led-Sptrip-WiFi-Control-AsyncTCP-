#pragma once

//Led strip definitions
#define DATA_PIN              D6
#define LEDS_TYPE             1                 // 1 - SK6812; 2 - WS2812B; 3 - WS2812; 4 - WS2811
#define MAX_BRIGHTNESS        250
#define COLOR_ORDER           RGB               // GRB - WS2812B
#define WIDTH                 1
#define HEIGHT                1

//Calculating number of leds
#if WIDTH == 1 and HEIGHT == 1
#define PREDEFINED_NUM_LEDS 300
#else
#define PREDEFINED_NUM_LEDS WIDTH*HEIGHT
#endif
//Soft AP configurations
#define SERVER_HOST_NAME "FoViBalTLight_01"
#define PASSWORD "123456789"

//Server IP and maximum connected clients
IPAddress serverIP(192, 168, 4, 1);     // Default ESP IP address
#define MAX_CLIENTS 4                 // Maximum clients connection

//Ports definition
#define TCP_PORT 6295

//Protocol preferences
const char root_previx[] =    "FoViBalTLight";  // root previx of protocol (root_previx:F:1:H:230..)
const size_t argsLen =        100;              // maximum recieve date length
const size_t argsArrayLength =  20;
const size_t argsPrevixLen =  4;
const char del[3] =           ":;";             // Default protocol value delimiter

//Nmae of file to store last started effect
const char* SPIFFS_file_name =     "/effects.txt";

//Protocol parsing struct definition

//Defining an array of handled values
uint8_t WiFiHandler[argsArrayLength];
uint16_t ledsDynamicNumber = 30;

WiFiServer server(TCP_PORT); // Setting up server
WiFiClient serverClients[MAX_CLIENTS]; // Limit num of conected clients
WiFiClient client;
