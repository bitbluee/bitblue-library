#include <Arduino.h>
#include <Bitblue.h>

Bitblue bb(26);

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  bb.begin();
}