#pragma once
// ============================================================
//  logger.h — output: CSV rows to the OpenLog SD card,
//             readable table to the USB serial monitor
// ============================================================
#include "telemetry.h"

void logger_begin();
void log_to_sd(const Telemetry &t);       // Writes the CSV header before the first row
void print_to_serial(const Telemetry &t);
