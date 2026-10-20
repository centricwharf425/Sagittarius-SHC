#pragma once
// ============================================================
//  telemetry.h — one snapshot of every reading we log
// ============================================================

struct Telemetry {
  // Time (UTC, from GPS). "NOSYNC" until the clock is set.
  char date[12];
  char time[9];

  // BME280 — environment
  float temp_c;
  float humidity_pct;
  float pressure_hpa;

  // BNO055 — orientation (degrees)
  float x_orient;
  float y_orient;
  float z_orient;

  // BNO055 - gyroscope (radians)
  float x_gyro;
  float y_gyro;
  float z_gyro;

  // INA260 — power
  float current_ma;
  float bus_voltage_mv;
  float power_mw;

  // u-blox GNSS — position (double: float only holds ~7 digits)
  double latitude;
  double longitude;
  float altitude_m;       // Above sea level
  float altitude_agl_m;   // Above the launch site
  bool gps_fix;           // True only with a live 3D fix

  // Geiger counter — counts per minute
  unsigned long cpm;

  unsigned int burst_count;
};
