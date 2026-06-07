#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal_I2C.h>

// BME280 sensor
Adafruit_BME280 bme; // I2C

// LCD I2C: Address 0x27, size 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Magnetic sensor
#define MAGNETIC_SENSOR_PIN 2

volatile int pulseCount = -1;
unsigned long previousMillis = 0;
const unsigned long interval = 30000; // 30 seconds
const unsigned long debounceDelay = 10;
unsigned long lastPulseTime = 0;
bool lastSensorState = HIGH;

// Global atmospheric pressure reference
double atmoPressure = 101325.0;  // default value, will be updated in setup

/* Flow rate calculation
double CalFlowRate(double rpm, double k) {
 return rpm * k;
}
*/

// Mass flow rate calculation
double CalMassFlowRate(double rpm) {
  double factor = 3.29e-3;
  double n = 0.278;
  return factor * pow(rpm, n);  // kg/s
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();

  // BME280 check
  if (!bme.begin(0x76)) {
    lcd.setCursor(0, 0);
    lcd.print("BME280 not found");
    Serial.println("BME280 not found!");
    while (1);
  }

  // Capture atmospheric pressure at startup
  atmoPressure = bme.readPressure();
  Serial.print("Atmospheric Pressure set to: ");
  Serial.println(atmoPressure);

  pinMode(MAGNETIC_SENSOR_PIN, INPUT_PULLUP);
}

void loop() {
  int sensorState = digitalRead(MAGNETIC_SENSOR_PIN);
  unsigned long currentTime = millis();

  // Count pulses with debounce
  if (lastSensorState == HIGH && sensorState == LOW) {
    if ((currentTime - lastPulseTime) > debounceDelay) {
      pulseCount++;
      lastPulseTime = currentTime;
    }
  }
  lastSensorState = sensorState;

  // Every 30 seconds
  if (currentTime - previousMillis >= interval) {
    previousMillis = currentTime;

    Serial.print("Pulse count: ");
    Serial.println(pulseCount);

    int PRM = pulseCount / 2;
    int rpm = (PRM * 60000) / interval;
    pulseCount = 0;

    double currPressure = bme.readPressure(); // Absolute Pa
    double pressure = (currPressure - atmoPressure) * -1; // Gauge pressure
    double temperature = bme.readTemperature();   // °C

    double massFlowrate = CalMassFlowRate(rpm);

    // Optional: Update k based on measurements (experimental tuning)
    // k = (massFlowrate * 287.05 * (temperature + 273.15)) / (currPressure * rpm);

    // Serial Output
    Serial.print("RPM: ");
    Serial.println(rpm);

    Serial.print("Mass Flowrate: ");
    Serial.print(massFlowrate, 4);
    Serial.println(" kg/s");

    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.print(" Pa | Temp: ");

    Serial.print(temperature);
    Serial.println(" C");

    // LCD Output
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("M.F:");
    lcd.print(massFlowrate, 3);
    lcd.print("kg/s");

    lcd.setCursor(0, 1);
    lcd.print("P:");
    lcd.print(pressure, 0);
    lcd.print("Pa ");
    lcd.print("T:");
    lcd.print(temperature, 1);
    lcd.print("C");

    delay(2000);
  }
}