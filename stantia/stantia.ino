#include "CanSatNeXT.h"
#include "TelemetryPacket.h"

TelemetryPacket packet;

packet.sample = getSampleCount();
packet.ax = readAccelX();
packet.ay = readAccelY();
packet.az = readAccelZ();
packet.gx = readGyroX();
packet.gy = readGyroY();
packet.gz = readGyroZ();

sendData((uint8_t*)&packet, sizeof(packet));

String messages[] = {
  "LED ON",
  "LED OFF",
  "Hello CanSat",
  "Do nothing",
  "Get ready!"
};

void setup() {
  Serial.begin(115200);
  GroundStationInit(40);   // ← СТАНЦИЯ
  randomSeed(analogRead(0));
}

void loop() {
  delay(400);

  int i = random(0, sizeof(messages) / sizeof(messages[0]));
  sendData(messages[i]);

  Serial.println("Sent: " + messages[i]);
}

// --- ПРИЁМ ДАННЫХ СО СПУТНИКА ---
void onDataReceived(String data)
{
  Serial.println("From CanSat: " + data);
}