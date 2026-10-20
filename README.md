# Sagittarius SHC

Teensy 4.0 telemetry logger. Reads environment, orientation, power, GPS, and
radiation data, then logs it as CSV to an OpenLog SD card and prints a table to
the USB serial monitor.

## Hardware

| Part      | Measures                         | Bus             |
|-----------|----------------------------------|-----------------|
| BNO055    | Orientation (x/y/z degrees)      | I2C             |
| BME280    | Temperature, humidity, pressure  | I2C             |
| INA260    | Current, bus voltage, power      | I2C             |
| u-blox GNSS | Position, altitude, UTC time   | I2C             |
| Geiger counter | Counts per minute (CPM)     | Pin 4 (interrupt) |
| OpenLog   | SD card CSV logging              | Serial4 (pin 17 TX) |

## Code layout (`SHC/`)

| File          | What it does                                  |
|---------------|-----------------------------------------------|
| `SHC.ino`     | `setup()` and `loop()`; start here            |
| `config.h`    | All pins, baud rates, and timing constants    |
| `telemetry.h` | The `Telemetry` struct: one row of logged data |
| `sensors.*`   | BNO055, BME280, INA260 setup and reads        |
| `gps.*`       | GNSS reads and syncing the clock from GPS     |
| `geiger.*`    | Interrupt pulse counting and CPM              |
| `logger.*`    | CSV output to OpenLog, table to serial        |

## Build & upload

Dependencies are pinned in `SHC/sketch.yaml`.

```sh
arduino-cli compile --profile teensy40 SHC
arduino-cli upload  --profile teensy40 SHC
```

## CSV columns

`Date,Time,Timestamp_ms,Temp_C,Humidity_percent,Pressure_hPa,X_Orient,Y_Orient,Z_Orient,Current_mA,Bus_Voltage_mV,Power_mW,Latitude,Longitude,Altitude_m,CPM`

Date and Time show `NOSYNC` until the GPS has a valid fix.
