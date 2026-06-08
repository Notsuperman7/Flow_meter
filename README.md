# Air Flow Monitoring System

This project is an Arduino-based air flow monitoring system that measures rotational speed, pressure, temperature, and estimated mass flow rate. It uses a magnetic sensor to calculate RPM, a BME280 sensor to read pressure and temperature, and a 16x2 I2C LCD to display the measured values in real time.

## Features

* Measures RPM using a magnetic sensor
* Reads temperature and pressure using a BME280 sensor
* Calculates estimated mass flow rate
* Displays mass flow rate, pressure, and temperature on an I2C LCD
* Prints sensor readings to the Serial Monitor
* Uses debounce logic for more stable pulse counting

## Hardware Components

* Arduino board
* BME280 pressure and temperature sensor
* 16x2 I2C LCD
* Magnetic sensor / Hall-effect sensor
* Magnet attached to the rotating part
* Jumper wires
* Power supply

## Libraries Required

Install the following libraries from the Arduino Library Manager:

```cpp
Adafruit BME280 Library
Adafruit Unified Sensor
LiquidCrystal_I2C
Wire
```

## Pin Configuration

| Component       | Arduino Pin |
| --------------- | ----------- |
| Magnetic Sensor | D2          |
| BME280 SDA      | SDA         |
| BME280 SCL      | SCL         |
| LCD SDA         | SDA         |
| LCD SCL         | SCL         |

The BME280 sensor uses I2C address `0x76`, and the LCD uses I2C address `0x27`.

## How It Works

The magnetic sensor detects pulses from a rotating part. The Arduino counts these pulses over a fixed time interval and converts them into RPM.

The BME280 sensor measures the current air pressure and temperature. At startup, the system stores the atmospheric pressure as a reference value. The pressure difference is then calculated as gauge pressure.

The mass flow rate is estimated using an empirical equation based on RPM:

```cpp
massFlowRate = 3.29e-3 * rpm^0.278
```

The final values are displayed on the LCD and printed to the Serial Monitor.

## Output Values

The system displays:

* Mass flow rate in kg/s
* Gauge pressure in Pa
* Temperature in °C
* RPM through the Serial Monitor

Example Serial Monitor output:

```text
RPM: 120
Mass Flowrate: 0.0125 kg/s
Pressure: 35 Pa | Temp: 26.4 C
```

## Important Note

In the code, the interval is set to:

```cpp
const unsigned long interval = 30000;
```

This means the reading updates every **30 seconds**.
If you want the update time to be 20 seconds, change it to:

```cpp
const unsigned long interval = 20000;
```

## Applications

This project can be used for:

* Air flow measurement experiments
* Ventilation system monitoring
* Fan performance testing
* Educational fluid mechanics or instrumentation projects
* Low-cost environmental measurement systems

## Future Improvements

* Add SD card data logging
* Add calibration for more accurate mass flow rate
* Add wireless monitoring using ESP32 or Wi-Fi module
* Display RPM directly on the LCD
* Add graphical dashboard support

## Author

Developed by **Nour Eldin**.
