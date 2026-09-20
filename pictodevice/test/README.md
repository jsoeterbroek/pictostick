# Unit Testing Guide for PictoStick

## Overview
PictoStick uses **AUnit** (https://github.com/bxparks/AUnit) for unit testing on ESP32 hardware.
Tests run directly on the M5StickC Plus2 device and output results via Serial.

## Quick Start

### Run All Tests
```bash
# Compile and upload test runner to device
cd pictodevice
../penv/bin/pio run -e m5stickcplus2 -t upload

# Open serial monitor to see results
../penv/bin/pio device monitor
```

### Test Individual Components
Each test file in `test/unit/` is standalone:
- `test_PSpref.cpp` - Preferences module tests
- `test_bounds.cpp` - Bounds checking tests
- `test_json_validation.cpp` - JSON config validation tests

To run a specific test:
1. Copy the test file to `src/` temporarily
2. Build and upload
3. Monitor serial output
4. Remove test file from `src/` when done

## Writing New Tests

### Basic Test Structure
```cpp
#include <Arduino.h>
#include <AUnit.h>
#include "your_module.h"

test(your_test_name) {
  // Arrange
  int expected = 42;
  
  // Act
  int result = your_function();
  
  // Assert
  assertEqual(expected, result);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Running tests...");
}

void loop() {
  aunit::TestRunner::run();
}
```

### Assertion Macros
- `assertEqual(expected, actual)` - Equality check
- `assertMoreOrEqual(actual, min)` - Minimum value check
- `assertLessOrEqual(actual, max)` - Maximum value check
- `assertTrue(condition)` - Boolean true check
- `assertFalse(condition)` - Boolean false check
- `assertNotNull(pointer)` - Null pointer check

## Interpreting Results

Tests output to Serial at 115200 baud:
```
=== PictoStick PSpref Unit Tests ===
brightness_returns_value_in_range: PASS
timeout_returns_positive_value: PASS
current_activity_index_is_valid: PASS

Test summary: 3 passed, 0 failed
```

## Best Practices

1. **Keep tests simple** - One assertion per concept
2. **Test edge cases** - Boundaries, empty values, max values
3. **Use descriptive names** - `test_brightness_in_valid_range` not `test1`
4. **Clean up after tests** - Don't leave device in modified state
5. **Run tests frequently** - After each significant change

## Limitations

- Tests run on hardware (no simulator)
- Manual upload required
- No automated CI/CD yet
- Limited by available flash/RAM

## Future Improvements

- [ ] Add CI/CD pipeline with GitHub Actions
- [ ] Create hardware-in-the-loop tests
- [ ] Add integration tests for full workflows
- [ ] Mock external dependencies (WiFi, SPIFFS)
- [ ] Add code coverage reporting

## Troubleshooting

**Tests don't compile:**
- Ensure AUnit library is installed: `pio lib install bxparks/AUnit`
- Check include paths are correct

**Tests crash on device:**
- Check for null pointers
- Verify array bounds
- Monitor serial output for error messages

**Serial output garbled:**
- Ensure baud rate matches (115200)
- Try different USB cable
- Reset device and reconnect
