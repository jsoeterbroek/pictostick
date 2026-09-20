# Unit Tests for PictoStick

## Overview
This directory contains unit tests for critical PictoStick components.
Tests use the AUnit framework (https://github.com/bxparks/AUnit) which runs on ESP32 hardware.

## Running Tests

```bash
# Run all tests
pio test --test-dir test/unit -e m5stickcplus2

# Run specific test file
pio test --test-dir test/unit/test_PSpref.cpp -e m5stickcplus2

# Run with verbose output
pio test --verbose
```

## Test Structure

- `test_PSpref.cpp` - Preferences/storage functions
- `test_fs_helpers.cpp` - File system operations
- `test_bounds.cpp` - Array bounds checking
- `test_json_validation.cpp` - JSON config validation

## Writing New Tests

1. Create new test file in `test/unit/`
2. Include AUnit and component being tested
3. Use `test()` macro for each test case
4. Use `assert*()` macros for assertions
5. Add to platformio.ini `[env:test]` section

## Example Test

```cpp
#include <AUnit.h>
#include "PSpref.h"

test(get_pspref_brightness_returns_valid_value) {
  int brightness = get_pspref_brightness();
  assertMoreOrEqual(brightness, 0);
  assertLessOrEqual(brightness, 100);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize preferences
  init_preferences();
}

void loop() {
  aunit::TestRunner::run();
}
```
