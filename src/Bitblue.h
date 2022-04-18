#ifndef BITBLUE_H
#define BITBLUE_H 

#include "Arduino.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h> 

#define SSID " "
#define PASSWD " "

extern int output;

static WiFiClient espClient;
static PubSubClient client(espClient);
static long lastMsg = 0;
static char msg[50];
static int value = 0;
static int flag = 0;
const char topic[] = "lnd/#";
//topic[] = "lnd/#"

const long interval = 10000;
extern unsigned long previousMillis;
//extern unsigned long previousMillis = 0;

extern int count;

void setup_wifi();
void callback(char* topic, byte* message, unsigned int length);
void reconnect();

class Bitblue 
{
  public:
    Bitblue(int pin);
    Bitblue(char* broker);
    Bitblue(char* broker, char* topic);
    void begin();
    void run();
    void onTransaction();
  private:
    int _pin;
    const char* mqtt_server;
    const char* topic; 
};

#endif
