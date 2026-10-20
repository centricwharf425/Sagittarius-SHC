#include <Arduino.h>
#include "config.h"


void led_begin(){
  pinMode(LED_PIN, OUTPUT);
}

void led_flash(){
  digitalWrite(LED_PIN, HIGH);
  delay(LED_DELAY);
  digitalWrite(LED_PIN, LOW);
}
