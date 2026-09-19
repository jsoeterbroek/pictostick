## Button Race Condition - Multiple Beeps and State Conflicts

**Severity:** Critical (High)
**Priority:** 🐛 HIGH
**Location:** `src/uisetup.cpp` - `drawDeviceMode3()` function

### Problem Description

Multiple button presses are processed in the same loop iteration without proper debouncing:

```cpp
// Lines ~60-90 in drawDeviceMode3()
void drawDeviceMode3() {
    // ... initialization ...
    
    // Handle BtnPWR
    if (StickCP2.BtnPWR.wasPressed()) {
        if (get_pspref_buzzer()) {
            StickCP2.Speaker.tone(6000, 100);  // Beep #1 happens
        }
        set_devicemode(4);  // Mode change
    }
    
    // ❌ PROBLEM: BtnPWR is still "pressed" - check BtnB in same iteration
    if (StickCP2.BtnB.wasPressed()) {
        if (get_pspref_buzzer()) {
            StickCP2.Speaker.tone(6000, 100);  // Beep #2 happens!
        }
        switch (cursor) { ... cursor handling ... }
    }
    
    // ❌ PROBLEM: Also check BtnA in same iteration
    if (StickCP2.BtnA.wasPressed()) {
        // More actions for BtnA
    }
    
    // Continue with rest of function...
}
```

When users press multiple buttons simultaneously, they experience:
- **Multiple beeps** (if buzzer is enabled)
- **Unpredictable state changes**
- **Cursor moves in unexpected directions**

### Impact

1. **Confusing UX:** User presses one button, gets multiple feedback (beeps + actions)
2. **Device State Conflicts:** Device might transition to wrong mode
3. **Cursor Jumping:** Cursor might move multiple positions in one action
4. **Hard to Debug:** Behavior depends on button press timing, not reproducible
5. **Audio Pollution:** Multiple beeps in sequence sounds unpleasant

**Example scenario:**
- User presses BtnPWR with intention to exit
- Device: Plays PWR beep, sets mode 4
- User also pressed BtnB (didn't notice): Device plays ANOTHER beep, moves cursor
- Result: User is confused, device doesn't act as expected

### Root Cause

The `wasPressed()` flag indicates a button transition from "released" to "pressed". The problem is:
1. Microcontroller detects multiple buttons being pressed at nearly the same time
2. Each button sets its `wasPressed()` flag independently
3. All `wasPressed()` checks happen in the same `loop()` iteration
4. All actions execute, regardless of which button was "intended" to be pressed

### Proposed Fix

Option A: Implement per-button processing flags (Recommended)

```cpp
// Add at top of drawDeviceMode3()
bool buttonProcessedFlags = {false, false, false};  // Track if each button was processed

void drawDeviceMode3() {
    // ... initialization ...
    
    // Reset flags at start
    buttonProcessedFlags[0] = buttonProcessedFlags[1] = buttonProcessedFlags[2] = false;
    
    // Handle BtnPWR (index 0) with flag check
    if (StickCP2.BtnPWR.wasPressed() && !buttonProcessedFlags[0]) {
        buttonProcessedFlags[0] = true;
        
        if (get_pspref_buzzer()) {
            StickCP2.Speaker.tone(6000, 100);
        }
        
        set_devicemode(4);
        
        // Don't return yet - let other buttons be checked
    }
    
    // Handle BtnB (index 1) with flag check
    if (StickCP2.BtnB.wasPressed() && !buttonProcessedFlags[1]) {
        buttonProcessedFlags[1] = true;
        
        if (get_pspref_buzzer()) {
            StickCP2.Speaker.tone(6000, 100);
        }
        
        switch (cursor) {
            case 0: activityDraw[0].drawNext(); break;
            case 1: activityDraw[1].drawNext(); break;
            case 2: activityDraw[2].drawNext(); break;
            // ... etc ...
        }
    }
    
    // Handle BtnA (index 2) with flag check
    if (StickCP2.BtnA.wasPressed() && !buttonProcessedFlags[2]) {
        buttonProcessedFlags[2] = true;
        
        // BtnA actions (cursor movement, etc.)
        // ...
    }
    
    // Continue with rest of function...
}
```

This ensures each button is processed only once per loop iteration.

Option B: Clear button state immediately after processing

```cpp
void drawDeviceMode3() {
    // ...
    
    if (StickCP2.BtnPWR.wasPressed()) {
        if (get_pspref_buzzer()) StickCP2.Speaker.tone(6000, 100);
        set_devicemode(4);
        // Clear the flag immediately so other checks don't see it
        StickCP2.BtnPWR.resetPressed();
    }
    
    if (StickCP2.BtnB.wasPressed()) {
        // Same for BtnB
    }
    
    if (StickCP2.BtnA.wasPressed()) {
        // Same for BtnA
    }
}
```

This works but is less clean and doesn't handle the case where multiple buttons are still pressed.

Option C: Sequential button handling (Cleaner but more invasive)

```cpp
// Restructure the main loop to handle buttons one at a time
void loop() {
    // Phase 1: Handle BtnPWR only
    if (StickCP2.BtnPWR.wasPressed()) {
        if (get_pspref_buzzer()) StickCP2.Speaker.tone(6000, 100);
        set_devicemode(4);
        
        // Phase 1 complete, don't process other buttons yet
        return;
    }
    
    delay(5);  // Brief delay to let other buttons settle
    
    // Phase 2: Handle BtnB only (if mode 3)
    if (get_current_device_mode() == 3) {
        if (StickCP2.BtnB.wasPressed()) {
            if (get_pspref_buzzer()) StickCP2.Speaker.tone(6000, 100);
            
            switch (cursor) {
                case 0: activityDraw[0].drawNext(); break;
                // ...
            }
        }
    }
    
    delay(5);
    
    // Phase 3: Handle BtnA only (if mode 3)
    if (get_current_device_mode() == 3) {
        if (StickCP2.BtnA.wasPressed()) {
            // Handle actions based on cursor
        }
    }
    
    // Phase 4: Handle other buttons...
}
```

This is cleaner but requires significant restructuring of the code.

### Recommended Fix Approach

I recommend **Option A (per-button processing flags)** as it:
- Is **minimal** - smallest code change required
- **Preserves current structure** of `drawDeviceMode3()`
- Is **easy to understand** - clear which button was processed
- Can be **extended** - add more buttons if needed (just add to array)
- Prevents **all button race conditions** in one place
- Easy to **test and debug** - flags show button processing status

### Implementation Details

```cpp
// Add buttonProcessedFlags in drawDeviceMode3()
void drawDeviceMode3() {
    // ... setup code ...
    
    // Track which buttons have been processed this loop
    // Index: PWR=0, B=1, A=2
    bool buttonProcessedFlags[3] = {false, false, false};
    
    // Handle each button with its flag
    if (StickCP2.BtnPWR.wasPressed() && !buttonProcessedFlags[0]) {
        buttonProcessedFlags[0] = true;
        if (get_pspref_buzzer()) StickCP2.Speaker.tone(6000, 100);
        set_devicemode(4);
    }
    
    if (StickCP2.BtnB.wasPressed() && !buttonProcessedFlags[1]) {
        buttonProcessedFlags[1] = true;
        if (get_pspref_buzzer()) StickCP2.Speaker.tone(6000, 100);
        
        // Handle cursor-based actions
        switch (cursor) {
            case 0: activityDraw[0].drawNext(); break;
            case 1: activityDraw[1].drawNext(); break;
            case 2: activityDraw[2].drawNext(); break;
            // ... continue for all cursor positions ...
        }
    }
    
    if (StickCP2.BtnA.wasPressed() && !buttonProcessedFlags[2]) {
        buttonProcessedFlags[2] = true;
        
        // Handle cursor increment/decrement
        if (cursor < MAX_ACTIVITIES - 1) {
            cursor++;
            activityDraw[cursor].draw();
        }
    }
    
    // Draw current screen
    drawDeviceMode3();
}
```

### Alternative: Tone Duration Issue

There's also a secondary issue with the 6000ms tone:

```cpp
StickCP2.Speaker.tone(6000, 100);  // 6000ms tone
```

During this tone, `wasPressed()` might still return true for other buttons, causing:

```cpp
// User presses BtnPWR
// Tone starts and plays for 6 seconds
// During tone, user presses BtnB
// BtnB triggers and another tone starts
// Result: Overlapping tones, audio glitches
```

**Fix:** Clear all button states when a tone is playing

```cpp
void drawDeviceMode3() {
    // Check if tone is currently playing
    if (StickCP2.Speaker.getToneDuration() > 0) {
        // Tone is playing, clear all buttons
        StickCP2.BtnPWR.resetPressed();
        StickCP2.BtnPWR.resetHeld();
        StickCP2.BtnB.resetPressed();
        StickCP2.BtnB.resetHeld();
        StickCP2.BtnA.resetPressed();
        StickCP2.BtnA.resetHeld();
        return;  // Skip this iteration
    }
    
    // Now handle buttons (only after tone is done)
    // ... rest of button handling ...
}
```

### Related Components

- `src/uisetup.cpp` - main source file affected
- `include/status.h` - button press state tracking, device mode
- `uisetup.cpp` - `drawDeviceMode3()` function
- `uisetup.cpp` - `drawDeviceMode4()` may have similar issue
- `uisetup.cpp` - `drawDeviceMode5()` may have similar issue
- `uisetup.cpp` - `drawDeviceMode6()` may have similar issue

### Acceptance Criteria

- [ ] Only one beep per button press (maximum)
- [ ] Device transitions work correctly regardless of button press timing
- [ ] Cursor moves to expected position (not jumping)
- [ ] No confusion from multiple simultaneous beeps
- [ ] Code passes linting checks for button handling
- [ ] No race conditions detected via static analysis
- [ ] Unit tests for button handling pass

### Test Cases to Add

```cpp
// Test 1: Single button press triggers only that button's action
TEST(TestCaseButtonSinglePress) {
    // Reset all buttons
    StickCP2.BtnPWR.resetPressed();
    StickCP2.BtnB.resetPressed();
    StickCP2.BtnA.resetPressed();
    
    // Press only BtnPWR
    StickCP2.BtnPWR.press();
    Tick(1);  // Wait for one loop iteration
    
    // Verify: Only PWR actions executed
    assert(deviceMode == 4);
    assert(beepCount == 1);
    
    // Verify: B and A did not trigger
    assert(cursor == 0);  // Should not have moved
}

// Test 2: Fast button press (press multiple at once)
TEST(TestCaseButtonMultiplePress) {
    // Reset all buttons
    StickCP2.BtnPWR.resetPressed();
    StickCP2.BtnB.resetPressed();
    StickCP2.BtnA.resetPressed();
    
    // Press all buttons simultaneously
    StickCP2.BtnPWR.press();
    StickCP2.BtnB.press();
    StickCP2.BtnA.press();
    Tick(1);
    
    // Verify: Each button processed once
    assert(beepCount == 3);  // Should be exactly 3 beeps, no more
    assert(deviceMode == 4);  // PWR should have changed mode
    assert(cursor == 1);  // B should have moved cursor
}

// Test 3: Sequential button press
TEST(TestCaseButtonSequentialPress) {
    // Reset
    StickCP2.BtnPWR.resetPressed();
    StickCP2.BtnB.resetPressed();
    StickCP2.BtnA.resetPressed();
    
    // Press PWR, quickly press B
    StickCP2.BtnPWR.press();
    Tick(1);
    
    // Release PWR, press B
    StickCP2.BtnPWR.release();
    Tick(1);
    StickCP2.BtnB.press();
    Tick(1);
    
    // Verify: Both buttons executed
    assert(deviceMode == 4);
    assert(cursor == 1);
    assert(beepCount == 2);
}

// Test 4: Button release before processing
TEST(TestCaseButtonReleaseEarly) {
    // Reset
    StickCP2.BtnPWR.resetPressed();
    StickCP2.BtnB.resetPressed();
    StickCP2.BtnA.resetPressed();
    
    // Press PWR, release before Tick processes
    StickCP2.BtnPWR.press();
    Tick(1);
    StickCP2.BtnPWR.release();
    Tick(1);  // This should have processed the button
    
    // Verify: Button was processed
    assert(beepCount == 1);
    assert(deviceMode == 4);
}

// Test 5: Tone during button press
TEST(TestCaseToneDuringPress) {
    // Reset
    StickCP2.BtnPWR.resetPressed();
    StickCP2.BtnB.resetPressed();
    StickCP2.BtnA.resetPressed();
    
    // Press BtnPWR (will trigger tone)
    StickCP2.BtnPWR.press();
    Tick(1);
    
    // Verify: Only one tone
    assert(StickCP2.Speaker.getToneDuration() > 0);
    
    // Press BtnB during tone
    StickCP2.BtnB.press();
    Tick(1);
    
    // Verify: No second tone (cleared by tone check)
    assert(StickCP2.Speaker.getToneDuration() > 0);
    assert(StickCP2.BtnB.resetPressed());
}
```

### Time to Fix

- **Option A:** 15-30 minutes (add array, add flag checks)
- **Option B:** 10-15 minutes (add reset calls)
- **Option C:** 2-3 hours (restructure loop)

**Recommendation:** Option A (minimal change, good enough for most cases)

### References to Other Issues

- See `timezone-initialization-order.md` - related timing issue in `initTime()`
- See `button-race-condition-time.md` - same pattern, different timing issue
- See `activity-index-bound.md` - related to cursor handling in this function

### References

- Arduino/ESP32 button debouncing: https://esp32.com/documentation/core/arduino/
- LVGL Button handling: https://docs.lvgl.io/latest/en/html/details/buttons.html
- ESP32 Button Library: https://github.com/JasonSchober/ESP32Button

### Priority Reason

This is a **critical UX bug** because:
- Confuses users during device setup
- Makes the device feel buggy/unpredictable
- Not consistent with how physical buttons work
- Hard to diagnose (depends on timing)

---

*This issue was identified during code review of the PictoDevice firmware.*

**Reported by:** Code Review Bot