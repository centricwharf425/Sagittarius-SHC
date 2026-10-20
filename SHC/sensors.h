#pragma once
// ============================================================
//  sensors.h — BNO055 (orientation), BME280 (environment),
//              INA260 (power)
// ============================================================
#include "telemetry.h"

void sensors_begin();  // Missing sensors are reported; their fields read NAN
void sensors_read(Telemetry &t);
void sensors_read_gyro(Telemetry &t);
