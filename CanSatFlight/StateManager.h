#include <Arduino.h>
#pragma once

enum MissionState {
  WAITING = 0,
  FLIGHT  = 1,
  FINISHED = 2
};

void loadState();
void saveState();

MissionState getMissionState();
void setMissionState(MissionState s);

unsigned long getSampleCount();
void incrementSampleCount();