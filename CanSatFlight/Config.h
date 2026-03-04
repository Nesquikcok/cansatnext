#pragma once
#include <Arduino.h>
// Частота измерений (мс)
#define MIN_DELAY 100     // 10 Гц
#define MAX_DELAY 10000   // 0.1 Гц
const String settingFile = "/delay_time";
const String stateFile   = "/state.txt";
const String flightFile  = "/flight_data.csv";

extern int delayTime;
extern int missionState;
extern unsigned long sampleCount;