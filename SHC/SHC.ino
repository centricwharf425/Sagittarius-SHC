// ============================================================
//  Sagittarius SHC — Teensy 4.0 telemetry logger
//
//  Each loop: read sensors -> wait for GPS -> log to SD + serial -> Stabiliation Loop (if outside jetstream)
//
//  config.h     pins, ports, timing
//  telemetry.h  the data record that gets logged
//  sensors.*    BNO055, BME280, INA260
//  gps.*        u-blox GNSS + clock sync
//  geiger.*     Geiger counter CPM
//  logger.*     OpenLog CSV + serial monitor table
//  solenoid.*   Solenoid
//  led.*        LED
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
  //Review: you don't need to set an i2c clock hz it does that automatically
  Wire.setClock(I2C_CLOCK_HZ);

  sensors_begin();
  gps_begin();
  geiger_begin();
  solenoid_begin();
  led_begin();
  Serial.println("All sensors initialized.");

  Serial.println("Setup completed successfully. Waiting for GPS PVT packets...");
}


static void stabilize() {
  // Only with a live 3D fix: a stale altitude from a lost fix is not trusted
  if (!data.gps_fix || data.altitude_agl_m < EXIT_ALTITUDE) return;

  sensors_read_gyro(data);
  if (fabs(data.z_gyro) > STABLE_ZONE_RANGE) {
    if (data.z_gyro < 0) {
      solenoid_left_burst();
    } else {
      solenoid_right_burst();
    }
  }
}

void loop() {
  if (gps_read(data)) {
    sensors_read(data);
    data.cpm = geiger_cpm();
    data.burst_count = solenoid_burst_count();
    print_to_serial(data);
    log_to_sd(data);
    led_flash();
  }

  stabilize();
  solenoid_update();
  led_update();
}
