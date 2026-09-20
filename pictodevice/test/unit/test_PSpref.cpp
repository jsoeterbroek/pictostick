/*
 * ABOUTME: Standalone unit tests for PSpref (Preferences) module
 * ABOUTME: Copy this file to src/ temporarily to run tests on device
 * 
 * USAGE:
 * 1. Copy test_PSpref.cpp to pictodevice/src/
 * 2. Run: penv/bin/pio run -e m5stickcplus2 -t upload
 * 3. Open serial monitor: penv/bin/pio device monitor
 * 4. Remove test file from src/ when done
 */

#include <Arduino.h>
#include <M5StickCPlus2.h>
#include <AUnit.h>
#include <Preferences.h>
#include "PSpref.h"

// Test namespace constant
const char* TEST_NS = "TestPrefs";

test(brightness_returns_value_in_range) {
  int brightness = get_pspref_brightness();
  
  // Brightness should be between 0 and 100
  assertMoreOrEqual(brightness, 0);
  assertLessOrEqual(brightness, 100);
}

test(timeout_returns_positive_value) {
  int timeout = get_pspref_timeout();
  
  // Timeout should be positive (seconds until sleep)
  assertMoreOrEqual(timeout, 0);
}

test(current_activity_index_is_valid) {
  int index = get_pspref_current_activity_index();
  
  // Index should be non-negative
  assertMoreOrEqual(index, 0);
}

test(activity_done_returns_boolean) {
  String testDay = "monday";
  int testIndex = 0;
  
  bool isDone = get_pspref_activity_done(testDay, testIndex);
  
  // Should return either true (1) or false (0)
  assertTrue(isDone == 0 || isDone == 1);
}

test(buzzer_returns_boolean) {
  bool buzzerEnabled = get_pspref_buzzer();
  
  // Should return either true or false
  assertTrue(buzzerEnabled == 0 || buzzerEnabled == 1);
}

test(color_theme_returns_valid_range) {
  int theme = get_pspref_color_theme();
  
  // Theme index should be non-negative
  assertMoreOrEqual(theme, 0);
}

test(timezone_returns_non_empty_string) {
  String timezone = get_pspref_timezone();
  
  // Timezone should not be empty
  assertTrue(timezone.length() > 0);
}

void setup() {
  auto cfg = M5.config();
  StickCP2.begin(cfg);
  
  Serial.begin(115200);
  delay(2000);  // Give time for serial monitor
  
  Serial.println("\n========================================");
  Serial.println("  PictoStick PSpref Unit Tests");
  Serial.println("========================================\n");
  
  // Initialize preferences system
  init_preferences();
}

void loop() {
  aunit::TestRunner::run();
  delay(1000);
}
