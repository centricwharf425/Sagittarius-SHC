#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BME280.h>
#include <Adafruit_INA260.h>
#include "sensors.h"

static Adafruit_BNO055 bno;  // Default I2C address 0x28
static Adafruit_BME280 bme;
static Adafruit_INA260 ina260;

// A missing sensor is skipped instead of halting, so the rest still log
static bool bnoOk = false;
static bool bmeOk = false;
static bool inaOk = false;

void sensors_begin() {
  bnoOk = bno.begin();
  if (!bnoOk) Serial.println("No BNO055 detected... continuing without it");
  bmeOk = bme.begin();
  if (!bmeOk) Serial.println("No BME280 detected... continuing without it");
  inaOk = ina260.begin();
  if (!inaOk) Serial.println("No INA260 detected... continuing without it");
}

void sensors_read_gyro(Telemetry &t) {
  if (!bnoOk) {
    // NAN never passes the stable-zone check, so no BNO055 means no bursts
    t.x_gyro = t.y_gyro = t.z_gyro = NAN;
    return;
  }
  sensors_event_t event;
  bno.getEvent(&event, Adafruit_BNO055::VECTOR_GYROSCOPE);  // rad/s

  t.x_gyro = event.gyro.x;
  t.y_gyro = event.gyro.y;
  t.z_gyro = event.gyro.z;
}

void sensors_read(Telemetry &t) {
  if (bnoOk) {
    sensors_event_t event;
    bno.getEvent(&event);
    t.x_orient = event.orientation.x;
    t.y_orient = event.orientation.y;
    t.z_orient = event.orientation.z;
  } else {
    t.x_orient = t.y_orient = t.z_orient = NAN;
  }

  sensors_read_gyro(t);

  if (bmeOk) {
    t.temp_c = bme.readTemperature();
    t.humidity_pct = bme.readHumidity();
    t.pressure_hpa = bme.readPressure() / 100.0F;  // Pa -> hPa
  } else {
    t.temp_c = t.humidity_pct = t.pressure_hpa = NAN;
  }

  if (inaOk) {
    t.current_ma = ina260.readCurrent();
    t.bus_voltage_mv = ina260.readBusVoltage();
    t.power_mw = ina260.readPower();
  } else {
    t.current_ma = t.bus_voltage_mv = t.power_mw = NAN;
  }
}
