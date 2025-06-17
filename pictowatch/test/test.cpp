#include <Arduino.h>
#include "unity.h"
#include "app_batt_voltage.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_app_batt_voltage_load(void) {
    // Create a dummy container (mock or minimal lv_obj_t if possible)
    // For now, just check that the function can be called without crashing
    app_batt_voltage_load(NULL);
    // Add more assertions if you have a testable LVGL environment
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_app_batt_voltage_load);
  return UNITY_END();
}

void setup() {
  // Wait ~2 seconds before the Unity test runner
  // establishes connection with a board Serial interface
  delay(2000);

  runUnityTests();
}
void loop() {}
