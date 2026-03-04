#include "Sensors.h"
#include "CanSatNeXT.h"
#include "StateManager.h"
#include "Storage.h"

static unsigned long LEDOnTill = 0;
static float previousPressure = 1000;

void handleFlightSensors() {

  // Ускорение
  float ax, ay, az;
  readAcceleration(ax, ay, az);
  float totalSquared = ax*ax + ay*ay + az*az;

  if (totalSquared < 0.1) {
    LEDOnTill = millis() + 2000;
  }
  digitalWrite(LED, LEDOnTill >= millis());

  // IMU
  float ax2 = readAccelX();
  float ay2 = readAccelY();
  float az2 = readAccelZ();
  float gx  = readGyroX();
  float gy  = readGyroY();
  float gz  = readGyroZ();

  // Температура и давление
  float t = readTemperature();
  float p = readPressure();

  if (previousPressure - 0.1 > p) {
    digitalWrite(LED, HIGH);
  }
  previousPressure = p;

  // LDR
  float ldr = analogReadVoltage(LDR);

  String line =
    String(getSampleCount()) + "," +
    String(ax2,3) + "," + String(ay2,3) + "," + String(az2,3) + "," +
    String(gx,3)  + "," + String(gy,3)  + "," + String(gz,3)  + "," +
    String(p,2)   + "," + String(t,2)   + "," + String(ldr,3) + "\n";

  appendFlightLog(line);
  incrementSampleCount();
}