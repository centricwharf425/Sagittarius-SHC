#include <Arduino.h>
#include "config.h"
#include "solenoid.h"


void solenoid_begin() {
  pinMode(SOLENOID_LEFT_PIN, OUTPUT);
  pinMode(SOLENOID_RIGHT_PIN, OUTPUT);
}

void solenoid_left_burst() {
  digitalWrite(SOLENOID_LEFT_PIN, HIGH);
  delay(BURST_DELAY);
  digitalWrite(SOLENOID_LEFT_PIN, LOW);
}

void solenoid_right_burst() {
  digitalWrite(SOLENOID_RIGHT_PIN, HIGH);
  delay(BURST_DELAY);
  digitalWrite(SOLENOID_RIGHT_PIN, LOW);
}
