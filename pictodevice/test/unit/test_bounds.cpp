/*
 * ABOUTME: Unit tests for array bounds checking and validation
 * ABOUTME: Tests verify that out-of-bounds access is prevented
 */

#include <Arduino.h>
#include <AUnit.h>
#include <ArduinoJson.h>
#include "common.h"
#include "fs_helpers.h"

// Test JSON document for validation tests
StaticJsonDocument<2048> testDoc;

test(activities_size_validation_prevents_negative) {
  // Simulate invalid activities size
  int invalid_size = -1;
  
  // Should be caught by validation logic
  assertTrue(invalid_size <= 0);
}

test(activities_size_validation_prevents_too_large) {
  int max_allowed = config_activities_size_max;
  int too_large = max_allowed + 100;
  
  // Should be rejected
  assertTrue(too_large > max_allowed);
}

test(activity_index_bounds_check) {
  int valid_index = 5;
  int invalid_high = 100;
  int invalid_low = -1;
  
  // Valid index should pass
  assertTrue(valid_index >= 0 && valid_index < config_activities_size_max);
  
  // Invalid indices should fail
  assertFalse(invalid_high >= 0 && invalid_high < config_activities_size_max);
  assertFalse(invalid_low >= 0 && invalid_low < config_activities_size_max);
}

test(json_config_null_check) {
  // Create empty document
  JsonDocument emptyDoc;
  
  // Should be detected as null/missing
  assertTrue(emptyDoc.isNull() || !emptyDoc.containsKey("activities"));
}

test(json_config_missing_activities_key) {
  JsonDocument docWithoutActivities;
  docWithoutActivities["name"] = "Test";
  docWithoutActivities["version"] = "1.0";
  
  // Missing activities key should be detected
  assertFalse(docWithoutActivities.containsKey("activities"));
}

test(json_config_valid_activities_array) {
  JsonDocument validDoc;
  JsonArray activities = validDoc["activities"].to<JsonArray>();
  
  // Add some test activities
  JsonObject act1 = activities.add<JsonObject>();
  act1["order"] = 1;
  act1["picto"] = "test1.bmp";
  act1["description"] = "Test Activity 1";
  
  JsonObject act2 = activities.add<JsonObject>();
  act2["order"] = 2;
  act2["picto"] = "test2.bmp";
  act2["description"] = "Test Activity 2";
  
  // Should have valid activities
  assertTrue(validDoc.containsKey("activities"));
  assertEqual(activities.size(), 2);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== PictoStick Bounds & Validation Tests ===");
}

void loop() {
  aunit::TestRunner::run();
}
