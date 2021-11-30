#include "Arduino.h"
#include "Bitblue.h"

Bitblue::Bitblue(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  mqtt_server = "ec2-3-88-174-38.compute-1.amazonaws.com";
  topic[] = "topic/test1";
}

void Bitblue::begin()
{
  Serial.begin(115200);
  pinMode(output,OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void Bitblue::run()
{
if (!client.connected()) {
    reconnect();
  }
  
  client.loop();

  if (flag == 1) {
    client.publish(topic, "dinvoice");
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

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

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
    this.onTransaction();
//    digitalWrite(output, HIGH);
  }

  /*if (!strncmp((char *)message, "dpr", 3)) {
    client.publish(topic, "dinvoice");
  }

  if (!strncmp((char *)message, "dp", 2)) {
    Serial.println("Payment transaction complete....");
  }*/
  
  Serial.println();
}