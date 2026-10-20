#pragma once
// ============================================================
//  config.h — every pin, port, and timing setting in one place
// ============================================================

// ---- Serial ports ----
#define USB_BAUD 115200
#define OPENLOG_SERIAL Serial4  // Teensy pin 17 (TX4) -> OpenLog RX
#define OPENLOG_BAUD 9600       // OpenLog factory default
#define OPENLOG_TX_BUFFER 512   // Extra TX buffer so a CSV row doesn't stall the loop at 9600 baud

// ---- I2C ----
#define I2C_CLOCK_HZ 400000

// ---- Geiger counter ----
#define GEIGER_PIN 5
#define CPM_PERIOD_MS 60000  // How often counts are rolled into CPM

// ---- GPS ----
#define LOG_FALLBACK_MS 1000  // Log interval if the GNSS is missing at boot

// ---- JET STREAM ----
#define EXIT_ALTITUDE 100  // Meters above the launch site (first 3D fix) before stabilizing


// ---- LEDS ----
#define LED_PIN 8
#define LED_DELAY 50  // 1/20th of a second in miliseconds

// ---- SOLENOID PINS ---
#define SOLENOID_LEFT_PIN 3
#define SOLENOID_RIGHT_PIN 4

// ---- STABILIZATION ---
#define STABLE_ZONE_RANGE 1     // Range of ± 1 rad/s
#define BURST_DELAY 500         // Burst of 500 miliseconds
#define BURST_COOLDOWN_MS 1500  // Wait after a burst before deciding again
#define MAX_BURST 200           // Gas Burst
