#include "CanSatNeXT.h"
#include "Config.h"
#include "Storage.h"
#include "StateManager.h"
#include "Sensors.h"
#include "CommandHandler.h"

// --- Глобальные переменные ---
int delayTime = 1000;

unsigned long LEDOnTill = 0;
unsigned long lastSendTime = 0;
const unsigned long sendDataInterval = 50;
bool LED_IS_ON = false;

uint16_t calculateCRC(uint8_t* data, size_t len)
{
  uint16_t crc = 0xFFFF;

  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];

    for (int j = 0; j < 8; j++) {
      if (crc & 1)
        crc = (crc >> 1) ^ 0xA001;
      else
        crc >>= 1;
    }
  }
  return crc;
}

void setup() {
  Serial.begin(115200);
  CanSatInit(7);       // ← Спутник, канал 7

  loadDelaySetting();    // загружаем частоту измерений
  loadState();           // восстанавливаем состояние миссии
  initFlightLog();       // создаем CSV заголовок
}

void loop() {

  handleRemoteCommand(); // обработка команд от наземной станции

  switch(getMissionState()) {

    case WAITING: {
      Serial.println("Waiting for launch...");
      delay(3000);
      setMissionState(FLIGHT);
      break;
    }

    case FLIGHT: {

      // --- Чтение датчиков ---
      float ax, ay, az;
      readAcceleration(ax, ay, az);
      float totalSquared = ax*ax + ay*ay + az*az;

      // --- LED при падении ---
      if (totalSquared < 0.1) {
        LEDOnTill = millis() + 2000;
      }
      digitalWrite(LED, LEDOnTill >= millis());

      // --- Логирование на SD ---
      handleFlightSensors();  // пишет CSV + увеличивает sampleCount + сохраняет state

      // --- Отправка телеметрии на НС ---
      // --- Отправка бинарной телеметрии ---
      if (millis() - lastSendTime >= sendDataInterval) {

        TelemetryPacket packet;

        packet.header = PACKET_HEADER;
        packet.sample = getSampleCount();

        packet.ax = ax;
        packet.ay = ay;
        packet.az = az;

        packet.gx = readGyroX();
        packet.gy = readGyroY();
        packet.gz = readGyroZ();

        packet.crc = calculateCRC(
            (uint8_t*)&packet,
            sizeof(TelemetryPacket) - sizeof(uint16_t)
        );

        sendData((uint8_t*)&packet, sizeof(packet));

        lastSendTime = millis();
      }

      // --- Проверка завершения миссии ---
      if (getSampleCount() >= 1000) {
        setMissionState(FINISHED);
      }

      delay(delayTime);
      break;
    }

    case FINISHED: {
      Serial.println("Mission complete. Stopping...");
      saveState();
      while (1); // останавливаемся
      break;
    }

  }
}

// --- Обработка команд от наземной станции ---
void onDataReceived(String data) {
  Serial.println("Received: " + data);
  data.trim();

  if (data == "LED ON") {
    digitalWrite(LED, HIGH);
    LED_IS_ON = true;
    Serial.println("LED turned ON");
  }
  else if (data == "LED OFF") {
    digitalWrite(LED, LOW);
    LED_IS_ON = false;
    Serial.println("LED turned OFF");
  }
  else if (data.startsWith("DELAY=")) {
    String valStr = data.substring(6);
    valStr.trim();
    int v = valStr.toInt();
    if (v >= MIN_DELAY && v <= MAX_DELAY) {
      delayTime = v;
      saveDelaySetting(); // сохраняем на SD для постоянства между перезагрузками
      Serial.println("New delayTime set: " + String(delayTime));
    } else {
      Serial.println("Invalid delay value: " + valStr);
    }
  }
  else {
    Serial.println("Message only, no action.");
  }
}