#pragma once
// ============================================================
//  gps.h — u-blox GNSS position + keeping the clock in sync
// ============================================================
#include "telemetry.h"

void gps_begin();                 // Halts if the GNSS is missing
bool gps_read(Telemetry &t);      // true when a fresh PVT packet arrived
