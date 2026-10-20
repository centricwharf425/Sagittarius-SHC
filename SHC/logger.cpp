#include <Arduino.h>
#include "config.h"
#include "logger.h"

static const char CSV_HEADER[] =
  "Date,Time,Timestamp_ms,Temp_C,Humidity_percent,Pressure_hPa,"
  "X_Orient,Y_Orient,Z_Orient,X_Gyro,Y_Gyro,Z_Gyro,"
  "Current_mA,Bus_Voltage_mV,Power_mW,"
  "Latitude,Longitude,Altitude_m,Altitude_AGL_m,GPS_Fix,CPM,Burst_Count";

static const char TABLE_HEADER[] =
  "| Date       | Time     | Runtime_ms | Temp (*C) | Humidity (%) | Pressure (hPa) | X Orient | Y Orient | Z Orient | Z Gyro   | Current | Bus Voltage | Power | latitude | longitude | altitude | AGL (m)  | Fix | CPM   | Bursts |";
static const char TABLE_RULE[] =
  "|------------|----------|------------|-----------|--------------|----------------|----------|----------|----------|----------|---------|-------------|-------|----------|-----------|----------|----------|-----|-------|--------|";

static uint8_t openlogTxBuffer[OPENLOG_TX_BUFFER];

// The header is written just before the first row, not in setup():
// the OpenLog is still mounting the SD card at boot and would drop it.
static bool headerWritten = false;

void logger_begin() {
  OPENLOG_SERIAL.begin(OPENLOG_BAUD);
  OPENLOG_SERIAL.addMemoryForWrite(openlogTxBuffer, sizeof(openlogTxBuffer));
}

void log_to_sd(const Telemetry &t) {
  if (!headerWritten) {
    OPENLOG_SERIAL.println(CSV_HEADER);
    headerWritten = true;
  }

  OPENLOG_SERIAL.printf("%s,%s,%lu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f,%.7f,%.7f,%.2f,%.2f,%d,%lu,%u\n",
                        t.date, t.time, millis(),
                        t.temp_c, t.humidity_pct, t.pressure_hpa,
                        t.x_orient, t.y_orient, t.z_orient,
                        t.x_gyro, t.y_gyro, t.z_gyro,
                        t.current_ma, t.bus_voltage_mv, t.power_mw,
                        t.latitude, t.longitude, t.altitude_m,
                        t.altitude_agl_m, t.gps_fix,
                        t.cpm, t.burst_count);
}

void print_to_serial(const Telemetry &t) {
  Serial.println();
  Serial.println(TABLE_HEADER);
  Serial.println(TABLE_RULE);
  Serial.printf("| %-10s | %-8s | %-10lu | %-9.2f | %-12.1f | %-14.2f | %-8.2f | %-8.2f | %-8.2f | %-8.3f | %-7.2f | %-11.2f | %-5.2f | %-8.5f | %-9.5f | %-8.2f | %-8.2f | %-3s | %-5lu | %-6u |\n",
                t.date, t.time, millis(),
                t.temp_c, t.humidity_pct, t.pressure_hpa,
                t.x_orient, t.y_orient, t.z_orient, t.z_gyro,
                t.current_ma, t.bus_voltage_mv, t.power_mw,
                t.latitude, t.longitude, t.altitude_m, t.altitude_agl_m,
                t.gps_fix ? "yes" : "no", t.cpm, t.burst_count);
  Serial.println(TABLE_RULE);
}
