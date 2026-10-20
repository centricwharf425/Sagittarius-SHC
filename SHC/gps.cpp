#include <Arduino.h>
#include <TimeLib.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "config.h"
#include "gps.h"

static SFE_UBLOX_GNSS gnss;
static bool gpsOk = false;        // False if the GNSS didn't answer at boot
static bool clockSynced = false;  // True once the clock has been set from GPS
static bool launchAltSet = false;
static float launchAlt_m = 0;     // MSL altitude at the first 3D fix
static unsigned long lastFallback = 0;

static time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

// Set the Teensy RTC and TimeLib from a valid GPS fix (UTC).
static void syncClock() {
  if (!(gnss.getGnssFixOk() && gnss.getTimeValid() && gnss.getDateValid())) return;

  tmElements_t tm;
  tm.Year = gnss.getYear() - 1970;  // TimeLib counts years from 1970
  tm.Month = gnss.getMonth();
  tm.Day = gnss.getDay();
  tm.Hour = gnss.getHour();
  tm.Minute = gnss.getMinute();
  tm.Second = gnss.getSecond();

  time_t t = makeTime(tm);
  Teensy3Clock.set(t);
  setTime(t);
  clockSynced = true;
}

void gps_begin() {
  if (!gnss.begin()) {
    Serial.println("No u-blox GNSS detected... logging without GPS");
    return;
  }
  gpsOk = true;
  gnss.setAutoPVT(true);
  setSyncProvider(getTeensy3Time);
}

bool gps_read(Telemetry &t) {
  if (!gpsOk) {
    // No GNSS: still return true every LOG_FALLBACK_MS so the other sensors keep logging
    if (millis() - lastFallback < LOG_FALLBACK_MS) return false;
    lastFallback = millis();
    t.gps_fix = false;
    return true;
  }

  if (!gnss.getPVT()) return false;

  syncClock();

  uint8_t fixType = gnss.getFixType();  // 3 = 3D, 4 = GNSS + dead reckoning
  t.gps_fix = gnss.getGnssFixOk() && (fixType == 3 || fixType == 4);

  t.latitude = gnss.getLatitude() / 10000000.0;  // 1e-7 deg -> deg
  t.longitude = gnss.getLongitude() / 10000000.0;
  t.altitude_m = gnss.getAltitudeMSL() / 1000.0;  // mm -> m

  // The first 3D fix becomes the launch-site reference
  if (t.gps_fix && !launchAltSet) {
    launchAlt_m = t.altitude_m;
    launchAltSet = true;
  }
  t.altitude_agl_m = launchAltSet ? t.altitude_m - launchAlt_m : 0;

  if (clockSynced) {
    snprintf(t.date, sizeof(t.date), "%04d-%02d-%02d", year(), month(), day());
    snprintf(t.time, sizeof(t.time), "%02d:%02d:%02d", hour(), minute(), second());
  } else {
    snprintf(t.date, sizeof(t.date), "NOSYNC");
    snprintf(t.time, sizeof(t.time), "NOSYNC");
  }
  return true;
}
