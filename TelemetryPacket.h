#pragma once
#include <Arduino.h>

#pragma pack(push, 1)
struct TelemetryPacket {
  uint16_t header;      // 0xAA55 — маркер начала пакета
  uint16_t sample;      // номер измерения

  float ax;
  float ay;
  float az;

  float gx;
  float gy;
  float gz;

  uint16_t crc;         // контрольная сумма
};
#pragma pack(pop)

static const uint16_t PACKET_HEADER = 0xAA55;