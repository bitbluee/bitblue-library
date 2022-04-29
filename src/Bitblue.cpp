#include "Arduino.h"
#include "Bitblue.h"

unsigned long previousMillis = 0;
int count = 0;
int output = 26;

//default constructor
Bitblue::Bitblue(int pin)
{
  //pinMode(pin, OUTPUT);
  _pin = pin;
  //default server
  //mqtt_server = "ec2-3-88-174-38.compute-1.amazonaws.com";
  mqtt_server = "ec2-3-87-5-120.compute-1.amazonaws.com";
  //topic = "topic/test1";
  topic = "lnd/dinvoice";
}

//user-defined broker
Bitblue::Bitblue(char* broker, char* topic)
{
  //pinMode(pin, OUTPUT);
  //_pin = pin;
  //default server
  mqtt_server = broker;
  topic = topic;
}

void Bitblue::begin()
{
  Serial.begin(115200);
  //pinMode(pin,OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //publish invoice as soon as up
  //client.publish(topic, "dinvoice");
  flag = 1;
}

void Bitblue::run()
{
if (!client.connected()) {
    reconnect();
  }
  
  client.loop();

  if (flag == 1) {
    client.publish(topic, "1");
    flag = 0;
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    String payload;

    payload += "hello world!";
    payload += " ";
    payload += count;

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(payload);

    // send message, the Print interface can be used to set the message contents
    // in this case we know the size ahead of time, so the message payload can be streamed

    bool retained = false;
    int qos = 1;
    bool dup = false;

    //mqttClient.beginMessage(topic, payload.length(), retained, qos, dup);
    //mqttClient.print(payload);
    //mqttClient.endMessage();

    //client.publish(topic, "hello world!");

    Serial.println();

    count++;
  }
}

void Bitblue::onTransaction()
{
  digitalWrite(_pin, HIGH);
  delay(250);
  //digitalWrite(_pin, LOW);
  //delay(250);  
}

//added 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("WiFiClient")) {
      Serial.println("connected");
      // Subscribe
      //check this
      client.subscribe("lnd/pdone");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Add 1) onPaymentRecv and 2)onPaymentReq
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  
  //if (strcmp((char *)message, "dpr".c_str()) == 0) {
  if (messageTemp == "dpr") {
    Serial.println("Payment request received....");
    flag = 1;
    //client.publish(topic, "dinvoice");
  }

  //if (strcmp((char *)message, "dp") == 0) {
  if (messageTemp == "dp") {
    Serial.println("Payment transaction complete....");
    //this->onTransaction();
    digitalWrite(output, HIGH);
    //send a new invoice ???
    //client.publish(topic, "dinvoice");
    flag=1;
  }

  /*if (!strncmp((char *)message, "dpr", 3)) {
    client.publish(topic, "dinvoice");
  }

  if (!strncmp((char *)message, "dp", 2)) {
    Serial.println("Payment transaction complete....");
  }*/
  
  Serial.println();
}
