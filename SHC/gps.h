#pragma once
// ============================================================
//  gps.h — u-blox GNSS position + keeping the clock in sync
// ============================================================
#include "telemetry.h"

void gps_begin();                 // If the GNSS is missing, logging continues without it
bool gps_read(Telemetry &t);      // true when a fresh PVT packet arrived (or the fallback timer fired)
