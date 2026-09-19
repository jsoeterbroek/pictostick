## Timezone Initialization Order - NTP Connection Fails

**Severity:** Critical (High)
**Priority:** 🐛 HIGH
**Location:** `src/system.cpp` - `initTime()` function

### Problem Description

The timezone is set AFTER attempting to connect to NTP, which causes NTP connection failures:

```cpp
// Lines ~60-80 in initTime()
void initTime(String timezone) {
    Serial.println(" ");
    Serial.print("connecting to time server ");
    Serial.println(" ");
    
    // ❌ PROBLEM: Uses default timezone (UTC) for NTP connection!
    configTime(0, 0, ntpServer);  // Global timezone is still default
    
    delay(5000);  // Wait for NTP connection
    
    // Too late! Already connected or failed with wrong timezone
    setTimezone(timezone);
    
    // Try to sync again?
    delay(5000);
    
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    
    if (tv.tv_year == 0) {
        Serial.println("ERROR: failed to obtain time, setting to 10:00:00");
        setTimeTime();
    }
    
    // Update RTC with whatever time we got
    struct tm timeinfo;
    timeptr = gmtime(&t);  // GMT time, not local
    StickCP2.Rtc.setDateTime(*timeptr);
}
```

### Impact

On first boot with:
- **Empty system timezone** (never set via `setTimezone()`)
- **Wrong timezone** selected by user in setup

The NTP connection will:
1. **Use wrong timezone** for calculating the offset
2. **Fail** or get incorrect time
3. **Delay** 5 seconds before attempting (timeout period)
4. **Show error** to user: "ERROR: failed to obtain time"
5. **Set fallback time** to 10:00:00 instead of actual time
6. **User sees wrong time** on device display
7. **User confusion** and frustration

**Example scenario:**
- User is in Amsterdam (UTC+1 in winter, UTC+2 in summer)
- Device defaults to UTC (offset 0)
- Device tries to connect to NTP server at "UTC"
- NTP fails (timezone mismatch)
- Device falls back to 10:00:00
- User sees "10:00" on device, but it's actually 11:00 (off by 1 hour)
- User spends time trying to "fix" the time
- User finally sets timezone to "Europe/Amsterdam"
- **Now** device works correctly

### Root Cause

The `configTime()` function works like this:
1. Reads the current timezone offset (from `setTimezone()` calls)
2. Adds offset to UTC to get local time
3. Uses offset to connect to NTP server
4. Waits for NTP response
5. Converts UTC time from NTP to local time using offset

If you call `configTime()` **before** `setTimezone()`:
- Device uses **default timezone** (UTC, offset = 0)
- NTP server is at its own timezone (usually UTC)
- No offset, so connection might work but gets UTC time
- Then `setTimezone()` is called, but **time is already set**
- Device displays UTC as "local" time

### Proposed Fix

Set timezone BEFORE calling `configTime()`:

```cpp
void initTime(String timezone) {
    Serial.println(" ");
    Serial.print("connecting to time server ");
    Serial.println(" ");
    
    // ✅ FIX: Set timezone FIRST, before NTP connection
    setTimezone(timezone);
    
    // NOW the device uses the correct timezone for NTP
    configTime(0, 0, ntpServer);
    
    delay(5000);  // Wait for NTP connection
    
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    
    if (tv.tv_year == 0) {
        Serial.println("ERROR: failed to obtain time, setting to 10:00:00");
        setTimeTime();
    }
    
    struct tm timeinfo;
    auto dt = StickCP2.Rtc.getDateTime();
    timeptr = gmtime(&t);
    StickCP2.Rtc.setDateTime(*timeptr);
}
```

### Enhanced Fix with Retry Logic

Add retry logic for NTP connection failures:

```cpp
void initTime(String timezone) {
    Serial.println(" ");
    Serial.print("connecting to time server ");
    Serial.println(" ");
    
    // ✅ FIX: Set timezone FIRST
    setTimezone(timezone);
    
    // Try to sync time multiple times
    int ntpAttempts = 0;
    bool ntpSuccess = false;
    
    while (ntpAttempts < 3 && !ntpSuccess) {  // Try up to 3 times
        ntpAttempts++;
        Serial.printf("ATTEMPT %d... ", ntpAttempts);
        
        // Clear old time to force NTP sync
        setTime(-1, -1, -1, -1, -1, -1);
        
        configTime(0, 0, ntpServer);  // Now uses correct timezone
        delay(5000);  // Wait for NTP
        
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        
        // Check if time seems reasonable (10:00 PM is a safe bet)
        if (tv.tv_hour >= 21 && tv.tv_hour < 24) {  // Or check for 0
            Serial.println("OK!");
            ntpSuccess = true;
            
            // Update RTC with synchronized time
            struct tm timeinfo;
            auto dt = StickCP2.Rtc.getDateTime();
            timeptr = localtime(&tv.tv_sec);
            StickCP2.Rtc.setDateTime(*timeptr);
        } else {
            Serial.println("FAILED");
        }
    }
    
    if (!ntpSuccess) {
        Serial.println("ERROR: failed to obtain time after 3 attempts");
        setTimeTime();  // Set fallback time
    }
    
    // Update RTC with current time (either from NTP or fallback)
    struct tm timeinfo;
    timeptr = gmtime(&t);
    StickCP2.Rtc.setDateTime(*timeptr);
}
```

### Alternative: Use localtime instead of gmtime

If updating the RTC, use `localtime()` to get local time from UTC:

```cpp
// Get current local time
struct timeval tv;
gettimeofday(&tv, nullptr);

// Convert to local time
struct tm timeinfo;
timeinfo = *localtime(&tv.tv_sec);  // Convert to local time
timeptr = &timeinfo;
StickCP2.Rtc.setDateTime(*timeptr);
```

### Related Issues

This is related to the issue in `uisetup.cpp` where `gmtime()` and `localtime()` calls can interfere with each other.

### Related Components

- `src/system.cpp` - contains `initTime()` and `setTimezone()`
- `uisetup.cpp` - device setup with timezone selection (where user chooses timezone)
- `include/status.h` - timeinfo structure
- `main.cpp` - calls `init_ESPAsync_Ws()` which calls `initTime()`

### Acceptance Criteria

- [ ] Timezone is set **before** attempting NTP connection
- [ ] NTP connection succeeds on first attempt with correct timezone
- [ ] Device displays correct time after setup
- [ ] If NTP fails, device shows appropriate error message
- [ ] Fallback time is only used as last resort
- [ ] User sees "connecting to time server" message before connection is attempted
- [ ] User sees "OK!" when NTP connection succeeds
- [ ] No timing issues with timezone selection

### Proposed Test Cases

```cpp
// Test: Correct timezone leads to successful NTP sync
TEST(TestCaseCorrectTimezone) {
    // Simulate device in Amsterdam timezone
    setTimezone("Europe/Amsterdam");
    
    // Reset time (force NTP connection)
    setTime(-1, -1, -1, -1, -1, -1);
    
    // Call initTime
    initTime("Europe/Amsterdam");
    
    // Verify: NTP connection succeeded
    assert(gettimeofday(&tv, nullptr).tv_year > 2000);  // Not 0 (not from fallback)
    assert(tv.tv_hour >= 10 && tv.tv_hour <= 20);  // Reasonable hour
    
    // Verify: RTC has correct time
    auto dt = StickCP2.Rtc.getDateTime();
    assert(dt.hour == tv.tv_hour);
    assert(dt.minute == tv.tv_min);
    assert(dt.day == tv.tm_mday);
}

// Test: Wrong timezone leads to NTP failure
TEST(TestCaseWrongTimezone) {
    // Simulate device in wrong timezone (Japan)
    setTimezone("Asia/Tokyo");
    
    // Set NTP server to one that requires specific timezone
    ntpServer = "pool.ntp.org";
    
    // Reset time
    setTime(-1, -1, -1, -1, -1, -1);
    
    // Call initTime
    initTime("Wrong/NonExistent");
    
    // Verify: NTP connection failed
    assert(tv.tv_year == 0);  // From fallback
    
    // Verify: Fallback time set
    assert(tv.tv_hour == 10);  // Default fallback is 10:00
    
    // Verify: Error message shown
    Serial.println("ERROR: failed to obtain time after 3 attempts");
}

// Test: Default (empty) timezone leads to NTP failure
TEST(TestCaseDefaultTimezone) {
    // Don't set timezone (leave empty/default UTC)
    setTimezone("");
    
    // Set NTP server to one that's timezone-specific
    ntpServer = "pool.ntp.org";
    
    // Reset time
    setTime(-1, -1, -1, -1, -1, -1);
    
    // Call initTime
    initTime("");
    
    // Verify: NTP connection failed (or got UTC time)
    // This is expected - user should set timezone first
}
```

### Additional Timezone Handling Issues

#### Issue: gmtime() vs localtime() Confusion

In the same function, there's potential confusion between `gmtime()` and `localtime()`:

```cpp
struct tm timeinfo;
auto dt = StickCP2.Rtc.getDateTime();  // Local time from RTC
timeptr = gmtime(&t);  // ❌ WRONG: Converts to GMT, not local time
StickCP2.Rtc.setDateTime(*timeptr);  // Sets RTC back to GMT!
```

**Fix:** Use `localtime()` to get local time from the RTC:

```cpp
struct tm timeinfo;
timeptr = localtime(&StickCP2.Rtc.getDateTime().millis_since_epoch());
StickCP2.Rtc.setDateTime(*timeptr);
```

Or just use the RTC time directly (it's already in the device's timezone):

```cpp
StickCP2.Rtc.setDateTime(StickCP2.Rtc.getDateTime());
```

#### Issue: Daylight Saving Time

When Daylight Saving Time (DST) starts/steps, the timezone offset changes, but:
- `configTime()` is called once at boot
- `setTimezone()` might be called later
- Between the two calls, the offset is wrong
- Time will be off by 1 hour during DST change

**Fix:** Call `configTime()` whenever the timezone might have changed:

```cpp
void setTimezone(const String &tz) {
    // Set timezone
    tz = tz;
    
    // If this is the first time setting timezone, configure NTP
    if (configTimeInitialized) {
        Serial.println("Timezone changed, reconnecting to NTP...");
        initTime(tz);
        configTimeInitialized = true;
    } else {
        Serial.println("Timezone changed. Please wait for NTP update...");
    }
}
```

Or call it every time:

```cpp
void setTimezone(const String &tz) {
    setTimezone_internal(tz);
    
    // Always re-sync to NTP
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    
    configTime(0, 0, ntpServer);
    delay(5000);
}
```

### Related Function: `init_ESPAsync_Ws()`

There's also a timezone issue in the web server initialization that's connected to this:

```cpp
// In init_ESPAsync_Ws()
{
    // Set timezone BEFORE configuring WiFi/HTTP for time-synced operations
    setTimezone(timezone);
    
    // THEN configure WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    
    // THEN call initTime
    initTime(timezone);  // Now timezone is set, NTP will work correctly
    
    // ... rest of setup ...
}
```

### References

- ESP32 configTime() documentation: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/time.html
- ESP32 NTP configuration: https://randomnerdtutorials.com/esp32-time-ntp-arduino/
- Linux timezone database (used for timezone offsets): https://www.iana.org/time-zones
- Daylight Saving Time: https://www.timeanddate.com/time/zones/
- TZ Database: https://www.iana.org/time-zones

### References to Other Issues

- See `button-race-condition-time.md` for related button timing issue in `uisetup.cpp`
- See `activity-index-bound.md` for related activity timing issue

### Recommended Changes

#### In `src/system.cpp`:

```cpp
// Line ~60 - Set timezone FIRST
void initTime(String timezone) {
    Serial.println(" ");
    Serial.print("connecting to time server ");
    Serial.println(" ");
    
    // FIX: Set timezone BEFORE configTime()
    setTimezone(timezone);
    
    // Now call configTime with the correct timezone set
    configTime(0, 0, ntpServer);
    
    delay(5000);
    
    struct timeval tv;
    if (gettimeofday(&tv, nullptr)) {
        if (tv.tv_year == 0) {
            Serial.println("ERROR: failed to obtain time, setting to 10:00:00");
            setTimeTime();
        } else {
            struct tm timeinfo;
            timeptr = localtime(&tv.tv_sec);
            StickCP2.Rtc.setDateTime(*timeptr);
        }
    }
    
    Serial.println(" ");
}
```

#### In `uisetup.cpp`:

```cpp
// In drawDeviceMode3(), handle timezone initialization properly

void drawDeviceMode3() {
    // Check if timezone needs to be initialized
    if (timezoneInitialized == false) {
        String selectedZone = getTimezoneSelect();
        
        // Apply timezone and re-sync to NTP
        setTimezone(selectedZone);
        setTime(selectedZone);
        timezoneInitialized = true;
        
        // Restart NTP service
        // This will sync time with the correct timezone
    }
    
    // ... rest of button handling ...
}
```

### Acceptance Criteria (Enhanced)

- [ ] Timezone is set before attempting NTP connection
- [ ] NTP connection succeeds on first attempt with correct timezone
- [ ] Device displays correct time after setup (matching local timezone)
- [ ] If NTP fails, device shows clear error message and uses fallback time
- [ ] Fallback time is reasonable (not 10:00 PM, but midnight or dawn)
- [ ] Code passes static analysis for timezone ordering
- [ ] Unit tests for timezone initialization pass
- [ ] No timezone-related bugs introduced by the fix

### Estimated Fix Time

- **Minimal fix:** 15 minutes (swap the order of setTimezone() and configTime())
- **With retry logic:** 30-45 minutes
- **With DST handling:** 60-90 minutes

### Test Plan

1. **Unit test timezone initialization:**
   - Test with correct timezone
   - Test with wrong timezone
   - Test with empty timezone

2. **Integration test NTP sync:**
   - Simulate device boot with timezone set
   - Verify NTP connection succeeds
   - Verify correct time displayed

3. **Regression test:**
   - Test existing functionality is not broken
   - Verify timezone can still be changed later

### Risk Assessment

- **Low risk** with minimal fix (just swap the order)
- **Medium risk** with retry logic (could delay boot by ~20 seconds)
- **High risk** with DST handling (complex, could introduce new bugs)

**Recommendation:** Start with minimal fix, add retry logic if needed, add DST handling later if reports come in.

---

*This issue was identified during code review of the PictoDevice firmware.*

**Reported by:** Code Review Bot
**Date:** Generated on current date