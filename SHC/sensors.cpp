#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BME280.h>
#include <Adafruit_INA260.h>
#include "sensors.h"

static Adafruit_BNO055 bno;  // Default I2C address 0x28
static Adafruit_BME280 bme;
static Adafruit_INA260 ina260;

// Print an error and stop forever.
static void halt(const char *msg) {
  Serial.println(msg);
  while (1);
}

void sensors_begin() {
  if (!bno.begin()) halt("No BNO055 detected...");
  if (!bme.begin()) halt("No BME280 detected...");
  if (!ina260.begin()) halt("No INA260 detected...");
}

void sensors_read(Telemetry &t) {
  sensors_event_t event;
  bno.getEvent(&event);
  t.x_orient = event.orientation.x;
  t.y_orient = event.orientation.y;
  t.z_orient = event.orientation.z;

  t.x_gyro = event.gyro.x;
  t.y_gyro = event.gyro.y;
  t.z_gyro = event.gyro.z;

  t.temp_c = bme.readTemperature();
  t.humidity_pct = bme.readHumidity();
  t.pressure_hpa = bme.readPressure() / 100.0F;  // Pa -> hPa

  t.current_ma = ina260.readCurrent();
  t.bus_voltage_mv = ina260.readBusVoltage();
  t.power_mw = ina260.readPower();
}
