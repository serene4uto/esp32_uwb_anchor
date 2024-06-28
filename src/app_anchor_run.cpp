#include "app_anchor_run.h"

#define NUMBER_OF_DISTANCES 1 // how many distances to average over, more = more stable but slower

static void newRange();
static void newDevice(DW1000Device *device);
static void inactiveDevice(DW1000Device *device);

static void newRange()
{
  Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print(", ");

  float dist = 0.0;
  for (int i = 0; i < NUMBER_OF_DISTANCES; i++) {
    dist += DW1000Ranging.getDistantDevice()->getRange();
  }
  dist = dist/NUMBER_OF_DISTANCES;
  Serial.println(dist);
}

static void newDevice(DW1000Device *device)
{
  Serial.print("Device added: ");
  Serial.println(device->getShortAddress(), HEX);
}

static void inactiveDevice(DW1000Device *device)
{
  Serial.print("Delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}

void app_anchor_run(void) {
  // set antenna delay for anchors only. Tag is default (16384)
  // pref.begin("anchor", true);
  // uint16_t current_Adelay = pref.getUShort("Adelay", 16600);

  uint16_t current_Adelay = ANCHOR_ADELAY;
  DW1000.setAntennaDelay(current_Adelay);

  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  //start the module as an anchor, do not assign random short address
  DW1000Ranging.startAsAnchor((char *)ANCHOR_ADDR, ANCHOR_OPERATION_MODE, false);

  while(1)
  {
    DW1000Ranging.loop();
  }
}