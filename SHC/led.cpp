#include <Arduino.h>
#include "config.h"
#include "led.h"

static bool ledOn = false;
static unsigned long ledOnSince = 0;

void led_begin(){
  pinMode(LED_PIN, OUTPUT);
}

void led_flash(){
  digitalWrite(LED_PIN, HIGH);
  ledOn = true;
  ledOnSince = millis();
}

void led_update(){
  if (ledOn && millis() - ledOnSince >= LED_DELAY) {
    digitalWrite(LED_PIN, LOW);
    ledOn = false;
  }
}
