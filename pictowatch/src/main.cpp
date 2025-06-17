#include <Arduino.h>
#include <LilyGoLib.h>
#include <WiFi.h>
#include "global_flags.h"
#include "devicemode.h"
#include "PSpref.h"
#include "common.h"

#define ENABLE_PLAYER

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = 120, osy = 120, omx = 120, omy = 120, ohx = 120, ohy = 120; // Saved H, M, S x & y coords
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0;                    // for next 1 second timeout

bool initial = 1;

void setup() {
  dmPrefs.begin(NS, RO_MODE);  // Open our namespace (or create it                                                                                       
                               //  if it doesn't exist) in RO mode.
  
  bool dmInit = dmPrefs.isKey("devicemode");  // Test for the existence
                                              // of the "already initialized" key.

  watch.begin();
  watch.setRotation(0);

  watch.fillScreen(RGB565_GRAY_LIGHT);
}

void loop() {
}

