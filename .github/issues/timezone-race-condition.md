## Timezone Selection Race Condition - Conflicting Updates

**Severity:** High
**Priority:** ⚠️ MEDIUM-HIGH
**Location:** `uisetup.cpp` - Multiple functions handling timezone selection

### Problem Description

Multiple code paths can update the timezone simultaneously, causing race conditions:

```cpp
// uisetup.cpp - device setup mode (Mode 3)
void drawDeviceMode3() {
    // ... button handling ...
    
    // Handle BtnA (time setup)
    if (StickCP2.BtnA.wasPressed()) {
        if (cursor == TIMEZONE) {
            // ❌ PROBLEM: User can press buttons too fast
            // Before this timezone value is processed
        }
        
        // ❌ PROBLEM: Calls drawDeviceMode3() which handles buttons again
        drawDeviceMode3();  // Recursive call - potential race
    }
}

// In recursive call to drawDeviceMode3():
    // Handle BtnA again (same button that just pressed)
    if (StickCP2.BtnA.wasPressed()) {  // Still pressed!
        if (cursor == TIMEZONE) {
            // ❌ PROBLEM: Same timezone selected again
            // OR if cursor moved, different timezone
            String tz = getTimezoneSelect();
            setTimezone(tz);  // Race condition begins
        }
    }
}
```

```cpp
// src/system.cpp - system initialization
void initTime(String timezone) {
    Serial.println(" ");
    Serial.print("connecting to time server ");
    Serial.println(" ");
    
    // ❌ PROBLEM: setTimezone() called again from user input
    // During the 5 second NTP wait
    setTimezone(timezone);
    
    delay(5000);  // UI can be changed during this time!
    
    // Then initTime continues
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    
    // But timezone might have changed during delay!
    // Now using WRONG timezone for NTP connection
    
    configTime(0, 0, ntpServer);
    
    // ...
}
```

### Impact

1. **Confusing UX:** User selects timezone, then gets confused by "Timezone changed" message
2. **Incorrect Time:** Device syncs to wrong timezone (user's selection got overwritten)
3. **Extended Boot Time:** If timezone keeps changing, NTP retries take longer
4. **System Instability:** NTP connection might fail due to changing timezone
5. **Hard to Debug:** Behavior depends on button press timing

**Example scenario:**
1. User is on timezone selection screen (cursor at "Europe/Amsterdam")
2. User presses BtnA to select timezone
3. Device calls `setTimezone("Europe/Amsterdam")`
4. Device starts connecting to NTP
5. During the 5 second delay, user changes their mind
6. User presses BtnB (back button)
7. Device changes timezone to "Europe/Berlin"
8. NTP connection starts with Berlin timezone
9. NTP connection fails (wrong timezone)
10. Device falls back to 10:00:00
11. User frustrated: "Why did my timezone change!"

### Root Cause

The issue is similar to the button race condition, but for timezone settings:

```cpp
// User can change timezone at multiple points:
// Point 1: In drawDeviceMode3(), when BtnA is pressed
// Point 2: In initTime(), before NTP connection
// Point 3: Via touchscreen input during setup

// Each point sets a different timezone value
// Race condition if:
// - User changes timezone faster than system processes
// - Multiple code paths set timezone simultaneously
// - No locking mechanism prevents concurrent updates
```

### Proposed Fix

#### Option A: Protect setTimezone() with Flag (Recommended)

Add a flag to indicate if timezone is currently being configured:

```cpp
// Add to global state in status.h or similar
bool timezoneConfiguring = false;  // Flag to prevent concurrent updates

// Modify setTimezone() to accept flag
bool setTimezone(String timezone) {
    // ✅ FIX: Don't allow timezone changes if already configuring
    if (timezoneConfiguring) {
        Serial.println("TIMEZONE ALREADY CONFIGURING, IGNORE");
        return false;
    }
    
    // Set flag
    timezoneConfiguring = true;
    
    // Validate timezone
    if (isValidTimezone(timezone)) {
        // ... existing timezone setting code ...
        
        return true;
    } else {
        Serial.println("Invalid timezone");
        return false;
    }
}

// After NTP sync completes, clear flag
void initTime(String timezone) {
    String timezoneSetting = timezone;
    timezoneConfiguring = true;  // Start configuring
    
    setTimezone(timezoneSetting);
    
    delay(5000);  // Wait for NTP
    // During this time, timezoneConfiguring stays true
    // Any attempts to change timezone are ignored
    
    // After NTP completes (or fails)
    if (tv.tv_year == 0) {
        // NTP failed
        // Clear flag and use fallback
        timezoneConfiguring = false;
    } else {
        // NTP succeeded
        // ✅ Clear flag
        timezoneConfiguring = false;
    }
    
    // Update RTC and continue
}
```

#### Option B: Lock/Unlock Mechanism

Similar to Option A, but more explicit:

```cpp
// Add to global state
TimezoneLock lock;  // Mutex or bool flag

class TimezoneLock {
    bool isLocked = false;
    
    bool lock() {
        if (isLocked) {
            Serial.println("TIMEZONE LOCKED, IGNORE");
            return false;
        }
        
        isLocked = true;
        return true;
    }
    
    void unlock() {
        isLocked = false;
    }
};

// Usage in setTimezone()
void drawDeviceMode3() {
    if (StickCP2.BtnA.wasPressed()) {
        TimezoneLock lock;  // Try to lock
        if (lock.lock()) {
            // Lock acquired, can update timezone
            String tz = getTimezoneSelect();
            setTimezone(tz);
        }
        lock.unlock();  // Unlock
    }
}

// Usage in initTime()
void initTime(String timezone) {
    TimezoneLock lock;
    lock.lock();  // Lock during configuration
    
    setTimezone(timezone);
    delay(5000);
    
    // Clear lock after NTP completes
    lock.unlock();
}
```

#### Option C: Separate Configuration and Active Timezone

Keep "user selected timezone" separate from "active timezone":

```cpp
// Add to status.h
struct TimezoneState {
    String selected;  // What user has selected
    String active;    // What is currently active
    bool configured;  // Has timezone been configured (NTP succeeded)?
};

// Initialize state
TimezoneState timezoneState = {
    .selected = getDefaultTimezone(),
    .active = getDefaultTimezone(),
    .configured = false
};

// Modify setTimezone()
void setTimezone(String timezone) {
    // ✅ Only allow changing to selected timezone
    if (timezone != timezoneState.selected) {
        Serial.println("TIMEZONE NOT MATCHED TO SELECTION");
        return;
    }
    
    // ✅ Only configure after NTP succeeds
    if (!timezoneState.configured) {
        Serial.println("TIMEZONE NEEDS TO BE CONFIGURED VIA NTP");
        return;
    }
    
    // Set new active timezone
    timezoneState.active = timezone;
}

// Modify drawDeviceMode3()
void drawDeviceMode3() {
    if (StickCP2.BtnA.wasPressed()) {
        // ✅ Only allow changing selection
        String tz = getTimezoneSelect();
        timezoneState.selected = tz;
        Serial.printf("Selected timezone: %s", tz.c_str());
    }
}

// Modify initTime()
void initTime(String timezone) {
    // Set state
    timezoneState.selected = timezone;
    timezoneState.active = timezone;
    timezoneState.configured = true;  // Now configured!
    
    // Configure timezone (NTP connection)
    configTime(0, 0, ntpServer);
    
    // Now timezoneState.active will be used for NTP
    // ...
}
```

### Recommended Fix Approach

I recommend **Option A (flag-based protection)** for the following reasons:

1. **Minimal code changes** - Only adds one boolean flag
2. **Simple to implement** - Straightforward logic
3. **Easy to understand** - Self-explanatory flag name
4. **Prevents all race conditions** - Clear when protection is active
5. **Preserves UX** - User sees clear message when change is rejected
6. **Easy to test** - Can verify flag state at different points

### Implementation Details

```cpp
// Add to status.h (or create timezone_flags.h)
#ifndef TIMEZONE_FLAGS_H
#define TIMEZONE_FLAGS_H

// Flag to prevent timezone race conditions
extern bool timezoneConfiguring;

// Initialize flag
static inline void timezoneSetConfiguring(bool status) {
    timezoneConfiguring = status;
}

// Check flag
static inline bool timezoneIsConfiguring() {
    return timezoneConfiguring;
}

#endif
```

```cpp
// src/system.cpp - Modified initTime()
void initTime(String timezone) {
    Serial.println(" ");
    Serial.print("connecting to time server ");
    Serial.println(" ");
    
    // ✅ Set flag BEFORE starting configuration
    timezoneConfiguring = true;
    
    // ✅ Set timezone
    setTimezone(timezone);
    
    // NTP connection
    delay(5000);
    
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    
    if (tv.tv_year == 0) {
        Serial.println("ERROR: failed to obtain time, setting to 10:00:00");
        setTimeTime();
        // ✅ Clear flag on failure
        timezoneConfiguring = false;
    } else {
        // ✅ Clear flag on success
        timezoneConfiguring = false;
    }
    
    struct tm timeinfo;
    auto dt = StickCP2.Rtc.getDateTime();
    timeptr = gmtime(&t);
    StickCP2.Rtc.setDateTime(*timeptr);
}
```

```cpp
// uisetup.cpp - Modified drawDeviceMode3()
void drawDeviceMode3() {
    // ... existing button handling ...
    
    // Handle BtnA (timezone selection)
    if (StickCP2.BtnA.wasPressed()) {
        // ✅ Check flag BEFORE processing
        if (timezoneConfiguring) {
            Serial.println("TIMEZONE ALREADY CONFIGURING - PLEASE WAIT");
            // Don't process timezone selection
        } else {
            String tz = getTimezoneSelect();
            setTimezone(tz);
        }
    }
    
    // ... rest of button handling ...
}
```

```cpp
// uisetup.cpp - Modified drawDeviceMode5() (timezone display)
void drawDeviceMode5() {
    // ...
    
    // Handle BtnA (back button)
    if (StickCP2.BtnA.wasPressed()) {
        if (timezoneConfiguring) {
            Serial.println("TIMEZONE STILL CONFIGURING");
        } else {
            // Show timezone selection screen
            drawDeviceMode3();
        }
    }
}
```

### Enhanced: Prevent Multiple Configurations

You can also prevent the user from changing timezone multiple times during configuration:

```cpp
// Add to global state
int timezoneConfigAttempts = 0;
const int MAX_TIMEZONE_CONFIG_ATTEMPTS = 3;  // Allow 3 attempts

// Modify setTimezone()
bool setTimezone(String timezone) {
    // ✅ Don't allow more than MAX_ATTEMPTS
    if (timezoneConfigAttempts >= MAX_TIMEZONE_CONFIG_ATTEMPTS) {
        Serial.println("TIMEZONE CONFIGURATION LIMITED");
        return false;
    }
    
    // ✅ Only configure once
    if (timezoneConfiguring) {
        Serial.println("TIMEZONE ALREADY CONFIGURING - PLEASE WAIT");
        return false;
    }
    
    // Set configuring flag
    timezoneConfiguring = true;
    timezoneConfigAttempts++;
    
    // Validate and set timezone
    if (isValidTimezone(timezone)) {
        setTimezone_internal(timezone);
        return true;
    }
    
    return false;
}

// After configuration completes (NTP success or failure)
if (tv.tv_year != 0) {
    timezoneConfiguring = false;
    timezoneConfigAttempts = 0;  // Reset for next configuration
}
```

### Related Components

- `uisetup.cpp` - device setup with timezone selection
- `src/system.cpp` - `initTime()` and `setTimezone()` functions
- `uisetup.cpp` - timezone configuration screens
- `uisetup.cpp` - `drawDeviceMode3()` - timezone selection
- `uisetup.cpp` - `drawDeviceMode5()` - timezone display

### Related Issues

- See `timezone-initialization-order.md` - the NTP connection order issue
- See `button-race-condition.md` - similar race condition pattern with buttons

### Acceptance Criteria

- [ ] Timezone can only be configured once
- [ ] User cannot change timezone during NTP configuration
- [ ] Clear feedback if timezone change is rejected
- [ ] NTP connection uses the correct timezone (user's selection)
- [ ] After successful NTP, timezone configuration is complete
- [ ] If NTP fails, user can try again (within limits)
- [ ] No race conditions detected via static analysis
- [ ] Code passes timing tests for simultaneous button presses

### Test Cases to Add

```cpp
// Test: Timezone cannot be changed during configuration
TEST(TestCaseTimezoneLockDuringConfig) {
    bool timezoneConfiguring = true;
    
    // Try to change timezone
    setTimezone("Europe/Berlin");
    
    // Verify: timezone was NOT changed
    assert(timezoneConfiguring == true);
    assert(currentTimezone() == "Europe/Amsterdam");  // Previous timezone
}

// Test: Timezone can be set before configuration
TEST(TestCaseTimezoneSetBeforeConfig) {
    bool timezoneConfiguring = false;
    
    // Set timezone
    setTimezone("Europe/Berlin");
    
    // Verify: timezone WAS set
    assert(timezoneConfiguring == true);
    assert(currentTimezone() == "Europe/Berlin");
}

// Test: Multiple timezone change attempts
TEST(TestCaseMultipleTimezoneAttempts) {
    bool timezoneConfiguring = false;
    int configAttempts = 0;
    
    // First attempt
    setTimezone("Europe/Berlin");
    assert(configAttempts == 1);
    
    // Second attempt
    setTimezone("Europe/Amsterdam");
    assert(configAttempts == 2);
    
    // Third attempt
    setTimezone("Europe/Paris");
    assert(configAttempts == 3);
    
    // Fourth attempt - should be rejected
    setTimezone("Europe/London");
    assert(configAttempts == 3);
    assert(currentTimezone() == "Europe/Paris");  // Still the 3rd timezone
}

// Test: NTP timeout doesn't change timezone
TEST(TestCaseNTPTimeoutNoTimeZoneChange) {
    bool timezoneConfiguring = false;
    String selectedTimezone = "Europe/Amsterdam";
    
    // Set timezone and start config
    setTimezone(selectedTimezone);
    timezoneConfiguring = true;
    configTime(0, 0, "pool.ntp.org");
    
    // Simulate NTP failure
    delay(10000);  // Wait longer than 5 second timeout
    
    // Clear config flag (simulating failure)
    timezoneConfiguring = false;
    
    // Verify: Timezone was NOT changed due to timeout
    assert(currentTimezone() == "Europe/Amsterdam");
}
```

### Alternative: Lock-Based Approach (If Flag is Not Sufficient)

```cpp
#include <semphr.h>
SemaphoreHandle_t timezoneLock;

// Initialize lock
void setup() {
    // ... WiFi initialization ...
    
    // Create mutex
    timezoneLock = xSemaphoreCreateMutex();
}

// Usage
bool setTimezone(String timezone) {
    // Take lock
    if (xSemaphoreTake(timezoneLock, 100 / portTICK_PERIOD_MS) == pdFALSE) {
        Serial.println("TIMEZONE LOCK BUSY, RETRY LATER");
        return false;
    }
    
    try {
        // Check if already configuring
        if (timezoneConfiguring) {
            Serial.println("TIMEZONE ALREADY CONFIGURING");
            xSemaphoreGive(timezoneLock);
            return false;
        }
        
        timezoneConfiguring = true;
        
        // ... set timezone ...
    } finally {
        // Always release lock
        xSemaphoreGive(timezoneLock);
    }
}
```

This is more robust but adds complexity and requires ESP-IDF (not Arduino framework).

### Related Functions That Need Fixing

1. **`drawDeviceMode3()`** - timezone selection screen (BtnA press)
2. **`drawDeviceMode5()`** - timezone display screen (BtnA press to go back)
3. **`initTime()`** - timezone configuration (initialization)
4. **`drawDeviceMode7()`** - any other timezone handling

All need to check `timezoneConfiguring` flag before attempting timezone changes.

### References

- ESP32 Semaphore: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/esp32/api_system.html#xsemaphorecreatemutex
- Arduino Semaphore: https://www.pjrc.com/teensy/td_hardware.html#timer
- Mutex vs Binary Semaphore: https://www.freescaler.com/samples/CPP/Docs/04-Semaphore.html
- Race Conditions: https://en.wikipedia.org/wiki/Race_condition

### References to Other Issues

- See `timezone-initialization-order.md` - the same timezone but different issue (initialization order)
- See `button-race-condition.md` - same pattern but for button presses

### Priority Reason

This is a **high priority** because:
- Confuses users during setup
- Can prevent timezone from being configured
- Can cause NTP connection failures
- Hard to diagnose without the fix

### Estimated Fix Time

- **Option A (Flag-based):** 30-45 minutes
- **Option B (Lock-based):** 45-60 minutes
- **Option C (Separate state):** 60-90 minutes

**Recommendation:** Option A (simplest to implement and test)

---

*This issue was identified during code review of the PictoDevice firmware.*

**Reported by:** Code Review Bot
**Date:** Generated on current date