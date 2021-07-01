#pragma once

//Led strip definitions
#define DATA_PIN              6
#define LEDS_TYPE             1                 // 1 - SK6812; 2 - WS2812B; 3 - WS2812; 4 - WS2811
#define MAX_BRIGHTNESS        250
#define COLOR_ORDER           RGB               // GRB - WS2812B
#define WIDTH                 1
#define HEIGHT                37

//Calculating number of leds
#define NUM_LEDS WIDTH*HEIGHT

//Soft AP configurations
#define SERVER_HOST_NAME "FoViBalTLight_01"
#define PASSWORD "123456789"

//Server host name and maximum connected clients
#define MAX_CLIENTS 4                 // Maximum clients connection

//Ports definition
#define TCP_PORT 6295
#define DNS_PORT 53

//Protocol preferences
const char root_previx[] =    "FoViBalTLight";  // root previx of protocol (root_previx:F:1:H:230..)
const size_t argsLen =        100;              // maximum recieve date length
const size_t argsPrevixLen =  4;
const char del[3] =           ":;";             // Default protocol value delimiter

//Nmae of file to store last started effect
const String SPIFFS_file_name =     "/effects.txt";

//Protocol parsing struct definition
struct handler {
  char handlerChar[argsPrevixLen];
  short handlerVal;
};

//Defining an array of handled values
handler WiFiHandler[argsLen];
