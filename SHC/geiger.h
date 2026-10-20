#pragma once
// ============================================================
//  geiger.h — Geiger counter pulse counting (interrupt driven)
// ============================================================

void geiger_begin();
unsigned long geiger_cpm();   // Latest counts-per-minute, refreshed every CPM_PERIOD_MS
