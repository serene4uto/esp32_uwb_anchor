#include "app_main.h"

// Preferences pref;

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  while(!Serial); // Wait for Serial to be ready

  LOG_SET_LEVEL(ANCHOR_LOG_LEVEL); 

  // Initialize SPI
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, DW_CS);

  // Initialize DW1000
  DW1000Ranging.initCommunication(DW1000_PIN_RST, DW1000_PIN_SS, DW1000_PIN_IRQ);
}

void loop() {
  app_main();
}