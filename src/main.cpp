#include <Arduino.h>

#define PIN_SIGNAL GPIO_NUM_36
#define PIN_POWER GPIO_NUM_11

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SIGNAL, INPUT);
  pinMode(PIN_POWER, OUTPUT);
  digitalWrite(PIN_POWER, HIGH);
}

void loop() {
  int adc = analogRead(PIN_SIGNAL);
  Serial.printf("%d", adc);
  delay(100);
}
    