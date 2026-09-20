# Unit Testing Guide for PictoStick

## Overview
PictoStick uses **AUnit** (https://github.com/bxparks/AUnit) for unit testing on ESP32 hardware.
Tests run directly on the M5StickC Plus2 device and output results via Serial.

## Quick Start

### Run a Test Suite
Tests are standalone programs that replace the main application temporarily:

```bash
cd pictodevice

# 1. Backup your current main.cpp (optional)
cp src/main.cpp src/main.cpp.backup

# 2. Copy test file to src/ (replaces main.cpp functionality)
cp test/unit/test_PSpref.cpp src/test_main.cpp

# 3. Temporarily rename main.cpp so it doesn't compile
mv src/main.cpp src/main.cpp.disabled

# 4. Compile and upload test
../penv/bin/pio run -e m5stickcplus2 -t upload

# 5. Open serial monitor to see results
../penv/bin/pio device monitor

# Expected output:
# ========================================
#   PictoStick PSpref Unit Tests
# ========================================
# brightness_returns_value_in_range: PASS
# timeout_returns_positive_value: PASS
# ...
# Test summary: 7 passed, 0 failed

# 6. Restore original main.cpp
rm src/test_main.cpp
mv src/main.cpp.disabled src/main.cpp
```

## Available Tests

| Test File | Module | Tests | Description |
|-----------|--------|-------|-------------|
| `test_PSpref.cpp` | Preferences | 7 | Validate preference read/write operations |
| `test_bounds.cpp` | Validation | 7 | Array bounds & JSON validation checks |

## Writing New Tests

See `test/unit/README.md` for detailed guidelines on writing new unit tests.
