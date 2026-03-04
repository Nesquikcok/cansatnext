#include "CommandHandler.h"
#include "Config.h"
#include "Storage.h"
#include <Arduino.h>

void handleRemoteCommand() {
  if (!Serial.available()) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();

  if (cmd.length() == 0) return;

  for (char c : cmd) {
    if (!isDigit(c)) return;
  }

  int v = cmd.toInt();
  if (v < MIN_DELAY || v > MAX_DELAY) return;

  delayTime = v;
  saveDelaySetting();
}