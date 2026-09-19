## Activity Index Out of Bounds - Crash Potential

**Severity:** Critical (High)
**Priority:** 🐛 HIGH  
**Location:** `src/uisetup.cpp` - `drawDeviceMode3()` and related functions

### Problem Description

Activity index access is not properly bounded, causing potential crashes:

```cpp
// Lines ~90-110 in drawDeviceMode3()
void drawDeviceMode3() {
    // ... initialization ...
    
    // ❌ PROBLEM: cursor can be > MAX_ACTIVITIES - 1
    while (cursor < MAX_ACTIVITIES) {
        if (cursor == activityCursor) {
            // Highlight current activity
            activities[activInd].draw();  // ❌ If activInd == MAX_ACTIVITIES, crash!
        }
        cursor++;  // ❌ This can increment beyond MAX_ACTIVITIES
    }
    
    activityCursor = (activityCursor + 1) % (activitySize_t)(MAX_ACTIVITIES - 1);
    
    // ❌ PROBLEM: Using activityCursor without proper bounds
    if (activityCursor == activitySize_t(0)) {  // ❌ Invalid comparison
        // Highlight first activity
        activities[0].draw();  // ❌ If activityIndex < MAX_ACTIVITIES check, crash!
    }
    
    // ... more activity drawing ...
    // activities[activInd].draw();  // ❌ activInd not checked for bounds!
}
```

### Impact

1. **Segmentation fault** (crash) if:
   - `activityIndex` >= `MAX_ACTIVITIES`
   - `cursor` >= `MAX_ACTIVITIES`
   - `activInd` >= `MAX_ACTIVITIES`
   
2. **Unpredictable behavior:**
   - Crash at random times
   - Device appears to freeze, then recover
   - Hard to reproduce/debug

3. **Data corruption:**
   - Stack corruption
   - Memory corruption
   - Flash corruption

### Root Cause

Multiple places access `activities[]` array without proper bounds checking:

```cpp
#define MAX_ACTIVITIES 64
activity_t activities[MAX_ACTIVITIES];  // Valid indices: 0 to 63

// Access patterns:
activities[0];        // ✅ Safe
activities[63];       // ✅ Safe
activities[64];       // ❌ OUT OF BOUNDS!
activities[-1];       // ❌ OUT OF BOUNDS!
```

The code doesn't consistently check if indices are within `[0, MAX_ACTIVITIES)`.

### Problem Locations

#### 1. cursor Index Issue

```cpp
void drawDeviceMode3() {
    uint8_t cursor = 0;  // Cursor position
    
    // ❌ PROBLEM: cursor increments without proper boundary check
    while (cursor < MAX_ACTIVITIES) {
        // Draw activity
        activities[activInd].draw();  // If activInd >= MAX_ACTIVITIES → CRASH!
        
        // Increment cursor
        cursor++;
        
        // If cursor >= MAX_ACTIVITIES, next iteration will fail
    }
    
    // ❌ PROBLEM: Cursor can exceed MAX_ACTIVITIES
    cursor = 0;
}
```

#### 2. activityIndex / activInd Access

```cpp
// In drawDeviceMode4()
void drawDeviceMode4() {
    uint8_t activityIndex = get_current_device_mode();  // User input, not checked!
    
    if (activityIndex > 0 && activityIndex < (int)MAX_ACTIVITIES) {
        // ✅ Good: Some check here
        activities[activityIndex].draw();
    }
}

// ❌ PROBLEM: What if activityIndex == MAX_ACTIVITIES or < 0?
// ❌ PROBLEM: What if activityIndex is uninitialized?
```

#### 3. activityCursor Comparison

```cpp
// In drawDeviceMode3()
uint8_t activityCursor = activityIndex;  // User input, not checked!

// ❌ PROBLEM: Comparison uses literal 0, not bounds-checked
if (activityCursor == 0) {  // Should be: activityCursor == 0 && activityCursor < MAX_ACTIVITIES
    activities[activityCursor].draw();  // ✅ Safe if activityCursor is already in range
}
```

#### 4. activitySize_t vs uint8_t Comparison

```cpp
// ❌ PROBLEM: Mixing signed and unsigned types in comparison
uint8_t activitySize_t = sizeof(activities) / sizeof(activity_t);  // Should be 64
uint8_t MAX_ACTIVITIES = 64;

if (activityIndex < activitySize_t) {
    // ✅ This works, but confusing
    // Comparison between uint8_t and uint8_t
}
```

Should be:

```cpp
// ✅ Clearer, use consistent type
uint8_t MAX_ACTIVITIES = 64;

if (activityIndex < MAX_ACTIVITIES) {  // uint8_t < uint8_t
    // ...
}
```

#### 5. User Input Not Validated

```cpp
// In uisetup.cpp, timezone selection screen
void drawDeviceMode3() {
    // User selects an activity
    activityCursor = input;  // ❌ No validation that input is in range
    
    // Later code uses activityCursor
    if (activityCursor == 0) {
        activities[activityCursor].draw();  // If input was invalid, undefined behavior
    }
}
```

### Proposed Fix

#### Option A: Centralize Bounds Constants

Define clear constants and use them consistently:

```cpp
// In uisetup.cpp or common header
#include "status.h"
#include "activity.h"

// Define constants with clear names
const uint8_t MAX_ACTIVITIES = 64;
const uint8_t MAX_ACTIVITIES_CURSOR = MAX_ACTIVITIES;  // For cursor
const uint8_t MAX_ACTIVITIES_INDEX = MAX_ACTIVITIES;  // For index

// Helper functions for bounds checking
static inline bool isValidActivityIndex(uint8_t index) {
    return (index < MAX_ACTIVITIES_INDEX);
}

static inline uint8_t clampActivityIndex(uint8_t index) {
    if (index >= MAX_ACTIVITIES_INDEX) {
        return MAX_ACTIVITIES_INDEX - 1;  // Clamp to max valid index
    }
    return index;
}

// Usage throughout the code:
uint8_t activityCursor = input;  // From user input
uint8_t safeCursor = clampActivityIndex(activityCursor);

// When accessing array:
if (isValidActivityIndex(activityCursor)) {
    activities[activityCursor].draw();
}
```

#### Option B: Add In-Place Bounds Checking

Add validation at each access point:

```cpp
// In drawDeviceMode3()

void drawDeviceMode3() {
    // ... existing code ...
    
    // FIX: Bounds check cursor
    if (cursor >= MAX_ACTIVITIES) {
        cursor = 0;  // Reset to start
        Serial.printf("Cursor reset: was %u, now 0", cursor);
    }
    
    while (cursor < MAX_ACTIVITIES) {
        if (cursor == activityCursor) {
            // ✅ Check before accessing
            if (isValidActivityIndex(activInd)) {
                activities[activInd].draw();
            } else {
                Serial.printf("Skipping invalid activity index: %u", activInd);
            }
        }
        
        // FIX: Check before incrementing
        if (cursor == MAX_ACTIVITIES - 1) {
            // Last activity, don't increment
            Serial.println("Reached end of activities, staying here");
            break;  // Exit loop
        }
        
        cursor++;
    }
    
    // FIX: Bounds check activityCursor BEFORE comparison
    activityCursor = clampActivityIndex(activityCursor);
    
    if (activityCursor == 0) {  // Now safe to compare
        activities[activityCursor].draw();
    }
    
    // FIX: Check before accessing
    if (isValidActivityIndex(activityCursor)) {
        activities[activityCursor].drawNext();
    } else {
        Serial.println("Invalid activityCursor, using default");
        activities[0].drawNext();
    }
}
```

#### Option C: Input Validation at Source

Validate user input when it's received:

```cpp
// In uisetup.cpp, where user input is processed

void drawDeviceMode3() {
    // ...
    
    // When user enters a number for activity selection:
    String userInput = getTextInput();
    uint8_t activityCursor = userInput.toInt();
    
    // ✅ Validate input IMMEDIATELY
    if (activityCursor >= MAX_ACTIVITIES) {
        activityCursor = 0;  // Default to first activity
        Serial.printf("Invalid activity index: %u, using first", userInput.toInt());
    }
    
    if (activityCursor < 0) {  // Though toInt() won't return negative for uint8_t
        activityCursor = 0;
    }
    
    // Continue with validated index
    // No more bounds checking needed if we trust validation
}
```

### Recommended Fix Approach

I recommend a **hybrid approach**:

1. **Centralize bounds constants** (Option A)
2. **Add validation at input points** (Option C)
3. **Use inline checks for array access** (Option B)

This gives:
- Clear, maintainable code
- Safety at input points
- Safety at access points
- Easy to add new activities (just change MAX_ACTIVITIES)

### Implementation

```cpp
// Add to status.h (or create activity.h)
#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "status.h"

// Activity structure
typedef struct {
    String name;
    String description;
    String data;
    // ... other fields ...
} activity_t;

// Activity list
extern activity_t activities[MAX_ACTIVITIES];

// Bounds checking helpers
static inline bool isValidActivityIndex(uint8_t index) {
    return (index >= 0 && index < MAX_ACTIVITIES);
}

static inline bool isValidActivityCursor(uint8_t cursor) {
    return (cursor >= 0 && cursor < MAX_ACTIVITIES);
}

static inline uint8_t clampActivityIndex(uint8_t index) {
    if (index >= MAX_ACTIVITIES) {
        return MAX_ACTIVITIES - 1;
    }
    return index;
}

#endif // ACTIVITY_H
```

```cpp
// Usage in uisetup.cpp

void drawDeviceMode3() {
    uint8_t activityCursor = 0;
    uint8_t activInd = 0;
    
    // ✅ Validate activityCursor at initialization
    activityCursor = clampActivityIndex(activityCursor);
    
    // Draw all activities
    while (activityCursor < MAX_ACTIVITIES) {
        if (isValidActivityIndex(activInd)) {
            // ✅ Safe access
            activities[activInd].draw();
        } else {
            // Empty or invalid activity
            Serial.printf("Activity %u not initialized", activInd);
        }
        
        // Highlight current activity
        if (isValidActivityIndex(activityCursor)) {
            // Use activityCursor directly (already validated)
            // activities[activityCursor].draw();  // Already drawn above
        }
        
        activInd++;
        activityCursor++;
        
        // ✅ Check before incrementing
        if (activInd >= MAX_ACTIVITIES || activityCursor >= MAX_ACTIVITIES - 1) {
            // Reached end, stop
            break;
        }
    }
}
```

### Related Components

- `src/uisetup.cpp` - main code with multiple access points
- `include/status.h` - activity_t definition
- `uisetup.cpp` - `drawDeviceMode4()` also accesses activities[]
- `uisetup.cpp` - `drawDeviceMode5()` also accesses activities[]
- `uisetup.cpp` - `drawDeviceMode6()` also accesses activities[]

### Related Issues

- See `timezone-initialization-order.md` - different kind of bounds issue
- See `button-race-condition.md` - related to array access

### Acceptance Criteria

- [ ] All activity array accesses are bounds-checked
- [ ] No segmentation faults on invalid activity index
- [ ] No crashes with uninitialized activity indices
- [ ] User cannot select invalid activity (input validation)
- [ ] Code is clear and maintainable
- [ ] Static analysis passes for array bounds
- [ ] Unit tests for bounds checking pass

### Test Cases to Add

```cpp
// Test 1: Valid activity index
TEST(TestCaseValidActivityIndex) {
    uint8_t testIndex = 32;
    
    // Verify: Index is within bounds
    assert(isValidActivityIndex(testIndex) == true);
    assert(testIndex < MAX_ACTIVITIES);
}

// Test 2: Maximum valid activity index
TEST(TestCaseMaxValidActivityIndex) {
    uint8_t testIndex = MAX_ACTIVITIES - 1;  // 63
    
    // Verify: Index is within bounds
    assert(isValidActivityIndex(testIndex) == true);
    assert(testIndex < MAX_ACTIVITIES);
}

// Test 3: Out-of-bounds activity index
TEST(TestCaseInvalidActivityIndex) {
    uint8_t testIndex = MAX_ACTIVITIES;  // 64
    
    // Verify: Index is NOT within bounds
    assert(isValidActivityIndex(testIndex) == false);
    assert(testIndex >= MAX_ACTIVITIES);
}

// Test 4: Negative activity index (if possible)
TEST(TestCaseNegativeActivityIndex) {
    uint8_t testIndex = 0;  // Simulating negative (can't be negative for uint8_t)
    
    // Verify: 0 is valid
    assert(isValidActivityIndex(testIndex) == true);
    assert(testIndex >= 0);
}

// Test 5: Clamp function with valid index
TEST(TestCaseClampValidIndex) {
    uint8_t testIndex = 32;
    
    uint8_t clamped = clampActivityIndex(testIndex);
    
    // Verify: Valid index stays the same
    assert(clamped == testIndex);
}

// Test 6: Clamp function with out-of-bounds index
TEST(TestCaseClampInvalidIndex) {
    uint8_t testIndex = MAX_ACTIVITIES + 5;  // 69
    
    uint8_t clamped = clampActivityIndex(testIndex);
    
    // Verify: Invalid index is clamped to max
    assert(clamped == MAX_ACTIVITIES - 1);
    assert(clamped < MAX_ACTIVITIES);
}

// Test 7: Activities array bounds
TEST(TestCaseActivitiesArrayBounds) {
    // Test accessing activities array with valid indices
    for (uint8_t i = 0; i < MAX_ACTIVITIES; i++) {
        // Simulate access
        if (isValidActivityIndex(i)) {
            // Access is safe
        }
    }
    
    // Test with invalid indices
    uint8_t invalidIndex = MAX_ACTIVITIES;
    assert(isValidActivityIndex(invalidIndex) == false);
}
```

### Code Changes Required

#### Before (unsafe):

```cpp
// uisetup.cpp
void drawDeviceMode3() {
    uint8_t activityCursor = userInput.toInt();  // No validation
    
    // Direct access without checking
    if (activityCursor == 0) {
        activities[activityCursor].draw();  // CRASH if activityCursor >= MAX_ACTIVITIES
    }
    
    // Loop with potential overflow
    for (uint8_t i = 0; i <= MAX_ACTIVITIES; i++) {  // Should be < not <=
        activities[i].draw();  // CRASH on last iteration
    }
}
```

#### After (safe):

```cpp
// uisetup.cpp
#include "activity.h"  // New include

// Add helper functions
static inline bool isValidActivityIndex(uint8_t index) {
    return (index < MAX_ACTIVITIES);
}

static inline uint8_t clampActivityIndex(uint8_t index) {
    if (index >= MAX_ACTIVITIES) {
        return MAX_ACTIVITIES - 1;
    }
    return index;
}

void drawDeviceMode3() {
    uint8_t activityCursor = userInput.toInt();
    
    // ✅ Validate at input
    activityCursor = clampActivityIndex(activityCursor);
    
    // Draw current activity
    if (isValidActivityIndex(activityCursor)) {
        activities[activityCursor].draw();
    }
    
    // Loop with bounds
    for (uint8_t i = 0; i < MAX_ACTIVITIES; i++) {
        if (isValidActivityIndex(i)) {
            activities[i].draw();
        }
    }
}
```

### Static Analysis Checklist

When using tools like Clang Static Analyzer or Cppcheck:

```cpp
// Enable these checks:
- array-bounds
- signed-unsigned-mismatch
- resource-leaks
- memory
```

```bash
# Run with AddressSanitizer
make -DCFLAGS="-fsanitize=address"
# Then:
./pictodevice
# Output will show any bounds issues
```

### Review Checklist

For each place that accesses `activities[]`:

- [ ] Check index is >= 0
- [ ] Check index is < MAX_ACTIVITIES
- [ ] Use appropriate type (uint8_t, not int)
- [ ] Validate user input before use
- [ ] Handle invalid index gracefully

### References

- C++ Array Bounds Checking: https://en.cppreference.com/w/cpp/language/operator_comparison
- C++ Signed/Unsigned Integers: https://en.cppreference.com/w/cpp/language/numeric_literals
- ESP32 Memory Corruption: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/config-ldf.html
- M5StickC Plus 2 Documentation: https://www.m5stack.com/doc/3F87417J

### Risk Assessment

- **Critical risk** without fix: Crash, potential bricking
- **Low risk** with proper bounds checking
- **Medium effort** to fix (needs to be done in multiple places)

### Estimated Fix Time

- **Initial fix:** 1-2 hours (fix immediate issues)
- **Complete fix:** 2-4 hours (fix everywhere, add validation, add tests)
- **With static analysis:** 30-60 minutes (find issues automatically)

---

*This issue was identified during code review of the PictoDevice firmware.*

**Reported by:** Code Review Bot
**Date:** Generated on current date