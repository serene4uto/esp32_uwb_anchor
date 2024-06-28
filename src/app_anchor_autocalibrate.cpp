#include "app_main.h"
#include "app_anchor_autocalibrate.h"


uint16_t this_anchor_Adelay = ANCHOR_CALIBRATION_STARTING_DELAY; //starting value
uint16_t Adelay_delta = ANCHOR_CALIBRATION_INIT_STEP_SIZE; //initial binary search step size
uint8_t current_trial = 0;
uint16_t trial_Adelays[ANCHOR_CALIBRATION_TRIALS] = {0};
float last_delta = 0.0;

static void newRange();
static void newDevice(DW1000Device *device);
static void inactiveDevice(DW1000Device *device);

static void newRange(){

  float dist = DW1000Ranging.getDistantDevice()->getRange();

  LOG_INFO(String(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX) + ',' + String(dist));

  if (Adelay_delta < ANCHOR_CALIBRATION_ADELAY_DELTA) {

    trial_Adelays[current_trial] = this_anchor_Adelay;
    current_trial++;

    
    if (current_trial >= ANCHOR_CALIBRATION_TRIALS) {
      LOG_INFO("Calibration complete");

      // average the trials
      uint32_t sum = 0;
      for (uint8_t i = 0; i < ANCHOR_CALIBRATION_TRIALS; i++) {
        sum += trial_Adelays[i];
      }
      this_anchor_Adelay = sum / ANCHOR_CALIBRATION_TRIALS;
      LOG_INFO("Average Calibrated Adelay = " + String(this_anchor_Adelay));

      // save the calibrated value
      // pref.begin("anchor", false);
      // pref.putUShort("Adelay", this_anchor_Adelay);
      // pref.end();

      LOG_INFO("Anchor Autocalibration complete");

      while(1);
    }

    this_anchor_Adelay = ANCHOR_CALIBRATION_STARTING_DELAY;
    Adelay_delta = ANCHOR_CALIBRATION_INIT_STEP_SIZE;
    last_delta = 0.0;
    DW1000.setAntennaDelay(this_anchor_Adelay);

    return;
  }

  float this_delta = dist - ANCHOR_CALIBRATION_TARGET_DISTANCE;  //error in measured distance

  if ( this_delta * last_delta < 0.0) Adelay_delta = Adelay_delta / 2; //sign changed, reduce step size
    last_delta = this_delta;
  
  if (this_delta > 0.0 ) this_anchor_Adelay += Adelay_delta; //new trial Adelay
  else this_anchor_Adelay -= Adelay_delta;

  LOG_INFO("Trial " + String(current_trial) + " Adelay = " + String(this_anchor_Adelay));

  DW1000.setAntennaDelay(this_anchor_Adelay);
}

static void newDevice(DW1000Device *device){
  Serial.print("Device added: ");
  Serial.println(device->getShortAddress(), HEX);
}

static void inactiveDevice(DW1000Device *device){
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}

void app_anchor_autocalibrate(void){
  Serial.println("Starting Anchor Autocalibration");

  // check if the anchor has been calibrated before
  // pref.begin("anchor", false);
  // if (!pref.isKey("Adelay")) {
  //   pref.putUShort("Adelay", this_anchor_Adelay);
  // }
  // pref.end();

  // set antenna delay for anchors only. Tag is default (16384)
  DW1000.setAntennaDelay(this_anchor_Adelay);
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  //start the module as anchor, don't assign random short address
  DW1000Ranging.startAsAnchor((char *)ANCHOR_ADDR, ANCHOR_OPERATION_MODE, false);
  

  while(1)
  {
    DW1000Ranging.loop();
  }
  
}




