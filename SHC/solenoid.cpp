#include <Arduino.h>
#include "config.h"
#include "solenoid.h"


static int activePin = -1;  // Pin currently firing, -1 when idle
static unsigned long burstStart = 0;
static unsigned long lastBurstEnd = 0;
static unsigned int burstCount = 0;

static void start_burst(int pin) {
  if (activePin != -1) return;
  if (millis() - lastBurstEnd < BURST_COOLDOWN_MS) return;
  if (burstCount >= MAX_BURST) return;

  digitalWrite(pin, HIGH);
  activePin = pin;
  burstStart = millis();
  burstCount++;
}


void solenoid_begin() {
  pinMode(SOLENOID_LEFT_PIN, OUTPUT);
  pinMode(SOLENOID_RIGHT_PIN, OUTPUT);
}

void solenoid_left_burst() {
  start_burst(SOLENOID_LEFT_PIN);
}
void solenoid_right_burst() {
  start_burst(SOLENOID_RIGHT_PIN);
}

void solenoid_update() {
  if (activePin != -1 && millis() - burstStart >= BURST_DELAY) {
    digitalWrite(activePin, LOW);
    activePin = -1;
    lastBurstEnd = millis();
  }
}

unsigned int solenoid_burst_count() {
  return burstCount;
}
