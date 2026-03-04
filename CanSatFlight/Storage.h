#pragma once
#include <Arduino.h>

void loadDelaySetting();
void saveDelaySetting();

void loadMissionState(int &state, unsigned long &count);
void saveMissionState(int state, unsigned long count);

void initFlightLog();
void appendFlightLog(const String &line);