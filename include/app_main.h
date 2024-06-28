#ifndef __APP_MAIN_H__
#define __APP_MAIN_H__

#include <Arduino.h>
#include <DebugLog.h>
#include <Preferences.h>
#include "SPI.h"
#include "DW1000.h"
#include "DW1000Ranging.h"


#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4

#define DW1000_PIN_RST 27 // reset pin
#define DW1000_PIN_IRQ 34 // irq pin
#define DW1000_PIN_SS 4   // spi select pin

// leftmost two bytes below will become the "short address"
// #define ANCHOR_ADDR "84:01:22:EA:82:60:3B:9C"
// #define ANCHOR_ADELAY 16502

// #define ANCHOR_ADDR "84:02:22:EA:82:60:3B:9C"
// #define ANCHOR_ADELAY 16500

// #define ANCHOR_ADDR "84:03:22:EA:82:60:3B:9C"
// #define ANCHOR_ADELAY 16495

#define ANCHOR_ADDR "84:04:22:EA:82:60:3B:9C"
#define ANCHOR_ADELAY 16502


#define ANCHOR_OPERATION_MODE DW1000.MODE_LONGDATA_RANGE_ACCURACY

#define ANCHOR_LOG_LEVEL DebugLogLevel::LVL_INFO

// extern Preferences pref;

extern void app_main();

#endif // __APP_MAIN_H__