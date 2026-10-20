// ============================================================
//  Sagittarius SHC — Teensy 4.0 telemetry logger
//
//  Each loop: read sensors -> wait for GPS -> log to SD + serial
//
//  config.h     pins, ports, timing
//  telemetry.h  the data record that gets logged
//  sensors.*    BNO055, BME280, INA260
//  gps.*        u-blox GNSS + clock sync
//  geiger.*     Geiger counter CPM
//  logger.*     OpenLog CSV + serial monitor table
// ============================================================
#include <Wire.h>
#include "config.h"
#include "telemetry.h"
#include "sensors.h"
#include "gps.h"
#include "geiger.h"
#include "logger.h"
#include "solenoid.h"
#include "led.h"

Telemetry data = { "NOSYNC", "NOSYNC" };

void setup() {
  Serial.begin(USB_BAUD);
  while (!Serial && millis() < 4000);  // Wait up to 4 s for a serial monitor
  Serial.println("Code has begun initialization...");

  logger_begin();

  Wire.begin();
  Wire.setClock(I2C_CLOCK_HZ);

  sensors_begin();
  gps_begin();
  geiger_begin();
  Serial.println("All sensors initialized.");

  Serial.println("Setup completed successfully. Waiting for GPS PVT packets...");
}

void loop() {
  sensors_read(data);
  data.cpm = geiger_cpm();

  if (gps_read(data)) {
    print_to_serial(data);
    log_to_sd(data);
  } else {
    Serial.println("Get PVT False...");
  }

  led_flash();

  if (data.altitude_m >= EXIT_ALTITUDE) {
    // Begin Stabilization
    if (not(data.z_gyro <= STABLE_ZONE_RANGE)) {
      // Stabilize
      if (data.z_gyro < 0) {  // If negative then burst in the left direction
        solenoid_left_burst();
      }

      if (data.z_gyro > 0) {  // If positive then burst in the left direction
        solenoid_right_burst();
      }
    }
  }
}
