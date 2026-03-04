#include "StateManager.h"
#include "Storage.h"

static MissionState missionState = WAITING;
static unsigned long sampleCount = 0;

void loadState() {
  loadMissionState((int&)missionState, sampleCount);
}

void saveState() {
  saveMissionState(missionState, sampleCount);
}

MissionState getMissionState() {
  return missionState;
}

void setMissionState(MissionState s) {
  missionState = s;
  saveState();
}

unsigned long getSampleCount() {
  return sampleCount;
}

void incrementSampleCount() {
  sampleCount++;
  saveState();
}