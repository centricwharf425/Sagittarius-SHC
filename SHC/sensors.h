#pragma once
// ============================================================
//  sensors.h — BNO055 (orientation), BME280 (environment),
//              INA260 (power)
// ============================================================
#include "telemetry.h"

void sensors_begin();                  // Halts if any sensor is missing
void sensors_read(Telemetry &t);
