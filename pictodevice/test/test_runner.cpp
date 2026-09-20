/*
 * ABOUTME: Test runner for all unit tests
 * ABOUTME: Upload to device and monitor serial output for test results
 */

#include <Arduino.h>
#include <AUnit.h>

// Forward declarations from test files
void registerTests_PSpref();
void registerTests_Bounds();

void setup() {
  Serial.begin(115200);
  delay(2000);  // Give time for serial monitor to connect
  
  Serial.println("\n");
  Serial.println("========================================");
  Serial.println("  PictoStick Unit Test Runner v1.0");
  Serial.println("========================================");
  Serial.println();
  
  // Initialize hardware
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Run all registered tests
  aunit::TestRunner::run();
}

void loop() {
  // Tests run once in setup(), loop is idle
  delay(1000);
}
