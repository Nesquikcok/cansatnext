#include "Storage.h"
#include "CanSatNeXT.h"
#include "Config.h"

void loadDelaySetting() {
  if (fileExists(settingFile)) {
    int v = readFile(settingFile).toInt();
    if (v >= MIN_DELAY && v <= MAX_DELAY) {
      delayTime = v;
    }
  }
}

void saveDelaySetting() {
  writeFile(settingFile, String(delayTime));
}

void loadMissionState(int &state, unsigned long &count) {
  if (!fileExists(stateFile)) return;

  String s = readFile(stateFile);
  s.trim();
  int c = s.indexOf(',');
  if (c > 0) {
    state = s.substring(0, c).toInt();
    count = s.substring(c + 1).toInt();
  }
}

void saveMissionState(int state, unsigned long count) {
  writeFile(stateFile, String(state) + "," + String(count));
}

void initFlightLog() {
  if (!fileExists(flightFile)) {
    writeFile(
      flightFile,
      "Sample,Ax,Ay,Az,Gx,Gy,Gz,Pressure,Temperature,LDR\n"
    );
  }
}

void appendFlightLog(const String &line) {
  appendFile(flightFile, line);
}