#ifndef Bitblue_h 
#define Bitblue_h 

#include "Arduino.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h> 

static WiFiClient espClient;
static PubSubClient client(espClient);
static long lastMsg = 0;
static char msg[50];
static int value = 0;
static int flag = 0;

const long interval = 10000;
unsigned long previousMillis = 0;

int count = 0;

class Bitblue 
{
  public:
    Bitblue(int pin);
    void begin();
    void run();
    void onTransaction();
  private:
    int _pin;
    const char* mqtt_server;
    const char topic[]; 
};

#endif
