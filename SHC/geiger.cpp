#include <Arduino.h>
#include "config.h"
#include "geiger.h"

static volatile unsigned long counts = 0;  // Incremented by the ISR
static unsigned long cpmSnapshot = 0;
static unsigned long lastRollover = 0;

static void onPulse() {
  counts++;
}

void geiger_begin() {
  //Review: is the geiger counter an input only, or do you need to send info to it to initialize it?
  pinMode(GEIGER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(GEIGER_PIN), onPulse, FALLING);
}

unsigned long geiger_cpm() {
  unsigned long now = millis();
  if (now - lastRollover >= CPM_PERIOD_MS) {
    lastRollover = now;
    noInterrupts();
    cpmSnapshot = counts;
    counts = 0;
    interrupts();
  }
  return cpmSnapshot;
}
