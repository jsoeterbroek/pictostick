# PictoStick Todo List

Based on GitHub issues from https://github.com/jsoeterbroek/pictostick/issues

## Unchecked Items 🔲

_Note: Since I don't have direct access to your GitHub repository issues, I'll need you to provide the current open issues, or I can work with any specific todos you'd like me to focus on._

## Completed Items ✅

### esp wifimanager timezone (issue #18) ✅

**Proposed Fix:**

The most robust way to handle timezones, including automatic Daylight Saving Time (DST) adjustments, is by using POSIX timezone strings. We will use WiFiManager to capture this string from the user.

**Implementation Steps:**

1.  **Add Custom Parameter to WiFiManager:**
    - Create a `WiFiManagerParameter` to hold the POSIX timezone string.
    - **ID:** `timezone`
    - **Label:** "POSIX Timezone String. See [here](https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv) for a list."
    - **Default Value:** `CET-1CEST,M3.5.0,M10.5.0/3` (for Europe/Amsterdam).
    - This parameter will be a text input on the WiFi configuration portal.

2.  **Persist the Timezone String:**
    - After the user saves the WiFi settings, retrieve the timezone string from the custom parameter.
    - Save this string to the device's persistent storage using our `PSpref` helper class.

3.  **Configure System Time on Startup:**
    - During the boot sequence, read the saved POSIX timezone string from `PSpref`.
    - If it exists, use it to configure the global `TZ` environment variable.
    - Call `configTime()` to sync with an NTP server. The ESP32's time library will automatically use the `TZ` variable to calculate the correct local time, including DST offsets.

    ```cpp
    // Example snippet for time configuration
    const char* ntpServer = "pool.ntp.org";
    const char* posix_timezone = your_saved_timezone_string; // Load from PSpref
    
    setenv("TZ", posix_timezone, 1);
    tzset();
    configTime(0, 0, ntpServer);
    ```

This approach ensures time is always correct after the initial setup and properly handles DST transitions without needing complex lookup tables or external libraries.

_Items will be moved here as they are completed._

## Notes

- Each item should reference a GitHub issue number when available
- Plans will be posted as comments on the corresponding GitHub issues before implementation
- Implementation will be done in separate branches with pull requests
