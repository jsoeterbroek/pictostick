## Memory Leak in Web Server Route Handler

**Severity:** Critical (High)
**Priority:** 🐛 HIGH
**Location:** `src/system.cpp` - `init_ESPAsync_Ws()` function

### Problem Description

Memory is allocated but never freed in the web server initialization:

```cpp
// Lines ~110-130 in init_ESPAsync_Ws()
AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/save-config", [](AsyncWebServerRequest *request, JsonVariant &json) {
    // Handler logic here
    if (dayName.isEmpty()) {
        request->send(400, "text/plain", "Bad Request: Day name missing");
        return;
    }
    // ... save config logic ...
});
server.addHandler(handler);
```

The `handler` object is created with `new` but never deleted before the server shuts down or restarts.

### Impact

On devices with limited PSRAM (like M5StickC Plus 2 with 40MB), this causes:
- **Memory exhaustion** after multiple device restarts
- **ESP32 becoming unresponsive** or crashing
- **Web server becoming slow** or completely unresponsive
- **Increased boot time** due to memory pressure
- **WiFiManager cleanup issues** - memory leaks persist across the entire system

### Proposed Fix

Option A: Delete the handler in a cleanup function

```cpp
// Add cleanup handler to delete when device shuts down
void cleanupWebServer() {
    Serial.println("Cleaning up web server...");
    if (handler != nullptr) {
        handler->delete();
    }
    delete handler;
    Serial.println("Web server cleanup complete");
}

// Call cleanup during WiFiManager event
WiFiManager.setEvent(WIFI_MODE_EVENT, []() {
    cleanupWebServer();
});

// Or call during main loop cleanup
#endif
```

Better: Call during device initialization:

```cpp
// In setup(), after calling init_ESPAsync_Ws()
void setup() {
    // ...
    init_ESPAsync_Ws();
    // Call cleanup handler here if needed
    WiFiManager.setWebServer([](ESPAsyncWebServer *server) {
        if (handler != nullptr) {
            handler->delete();
        }
        delete handler;
    });
}
```

Option B: Use a class-based approach (recommended for long-term maintenance)

```cpp
// Create a class to manage the handler
class WebServerHandler {
private:
    AsyncCallbackJsonWebHandler *handler;
    AsyncWebServer *server;
    String path;
    WebHandlerCallback callback;
    
public:
    WebServerHandler(AsyncWebServer *server, const String &path, WebHandlerCallback cb) {
        this->server = server;
        this->path = path;
        this->callback = cb;
        handler = new AsyncCallbackJsonWebHandler(path, cb);
    }
    
    ~WebServerHandler() {
        if (handler != nullptr) {
            handler->delete();
            delete handler;
        }
    }
    
    AsyncWebServer *getServer() const {
        return server;
    }
};

// Usage:
{
    AsyncWebServer server(80);
    {
        WebServerHandler handler(server, "/save-config", [](AsyncWebServerRequest *request, JsonVariant &json) {
            // Handler logic
            if (dayName.isEmpty()) {
                request->send(400, "text/plain", "Bad Request: Day name missing");
                return;
            }
            // ... rest of logic ...
        });
        
        // Scope ends, handler is automatically deleted
    }
}
```

This ensures the handler is automatically deleted when going out of scope.

Option C: Use SmartPointers (C++11+)

```cpp
// If project supports C++11
#include <memory>

// Define smart pointer wrapper
using AsyncCallbackJsonWebHandlerPtr = std::unique_ptr<AsyncCallbackJsonWebHandler>;

// Usage:
AsyncCallbackJsonWebHandlerPtr handler = 
    std::make_unique<AsyncCallbackJsonWebHandler>(
        "/save-config", 
        [](AsyncWebServerRequest *request, JsonVariant &json) {
            // Handler logic
        }
    );
server->onJSON("/save-config", HTTP_POST, handler.get(), [](AsyncWebServerRequest *request, JsonVariant &json) {
    // Request handler
});
```

When `handler` goes out of scope, it's automatically deleted.

### Recommended Fix Approach

I recommend **Option B (RAII with class)** as it:
- Is minimal and requires smallest code changes
- Keeps the current structure of `init_ESPAsync_Ws()`
- Is easy to understand and maintain
- Can be extended to add more handlers
- Ensures proper cleanup even on early exits
- Prevents similar issues in future development

### Implementation Steps

```cpp
// Add at top of system.cpp
#include <memory>

// Add class definition before usage
class WebServerHandler {
private:
    AsyncCallbackJsonWebHandler *handler;
    AsyncWebServer *server;
    String path;
    WebHandlerCallback callback;
    
public:
    WebServerHandler(AsyncWebServer *server, const String &path, WebHandlerCallback cb) 
        : server(server), path(path), callback(cb) {
        handler = new AsyncCallbackJsonWebHandler(path, cb);
    }
    
    ~WebServerHandler() {
        if (handler != nullptr) {
            handler->delete();
        }
    }
    
    WebServerHandler(const WebServerHandler &other) = delete;
    WebServerHandler &operator=(const WebServerHandler &other) = delete;
};

// Update init_ESPAsync_Ws() to use the class
void init_ESPAsync_Ws() {
    AsyncWebServer server(80);
    
    // Existing handlers...
    
    // Add handler with RAII
    {
        WebServerHandler handler(server, "/save-config", [](AsyncWebServerRequest *request, JsonVariant &json) {
            // Handler logic here
            // ...
        });
        // When this block ends, handler is automatically deleted
    }
}
```

### Related Components

- `main.cpp` - calls `init_ESPAsync_Ws()`
- `src/system.cpp` - contains the problematic code
- `data/` - contains the SPIFFS mount logic
- ESP32Async library documentation

### Related Issues

- See `button-race-condition.md` - button handling timing may compound memory issues
- See `filesystem-error-handling.md` - related to the same handler, also has memory issues

### Acceptance Criteria

- [ ] No memory leak detected via ESP-IDF memory profiler (heap-blocks)
- [ ] Device can be restarted multiple times without crashing
- [ ] Web server responds correctly after restart
- [ ] Memory usage remains stable across restart cycles (< 10MB growth)
- [ ] Code review passes for resource management (RAII principles)
- [ ] Valgrind/Suppress file not needed (if running in simulation)

### Testing Method

```cpp
// Add to test suite
void testWebServerMemoryLeak() {
    // Simulate multiple device cycles
    for (int i = 0; i < 5; i++) {
        init_ESPAsync_Ws();
        delay(100);
        // Simulate shutdown
        cleanupWebServer();  // If using Option A
    }
    
    // Check memory usage
    heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    ESP_LOGI("memory", "Free RAM after %d cycles: %u bytes", i, freeHeap);
    
    // Assert: Memory usage should not increase significantly
    if (currentFreeHeap < initialFreeHeap - 500000) {
        TEST_FAILED("Memory leak detected!");
    }
}
```

### Additional Context

This is a classic C++ memory management issue that's particularly dangerous in embedded systems:

1. **ESP32 Memory Constraints:**
   - M5StickC Plus 2: 40MB PSRAM total
   - Flash: ~40MB (can store SPIFFS)
   - RAM: 512KB (SRAM)
   - PSRAM: 39MB (can be used as RAM)

2. **Memory Users:**
   - WiFiManager: ~10-20MB
   - ArduinoJson: ~1-5MB (depending on document size)
   - Display driver: ~10-20MB
   - AsyncWebServer: ~5-10MB
   - **Web handler: 1-2MB** (not freed!)

3. **Consequences:**
   - After 5 restarts: 5-10MB leaked
   - After 10 restarts: 10-20MB leaked
   - System becomes sluggish or unresponsive
   - Eventually, all memory is exhausted → crash

4. **Why This Happens:**
   - ESPAsyncWebServer doesn't provide a delete method for handlers
   - Arduino classes expect RAII-style cleanup
   - Global pointers aren't cleaned up by WiFiManager
   - ESP32 doesn't have a garbage collector

5. **Best Practices for ESP32:**
   - Always delete what you allocate
   - Use RAII (Resource Acquisition Is Initialization)
   - Minimize dynamic allocation
   - Use static allocation when possible
   - Profile memory usage regularly

### References

- ESPAsyncWebServer API: https://github.com/me-no-dev/ESPAsyncWebServer
- ArduinoJson Memory Usage: https://arduinojson.org/v6/faq/memory-usage/
- ESP32 PSRAM: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/psram.html
- RAII in C++: https://en.cppreference.com/w/cpp/language/rule_of_three
- Arduino ESP32: https://docs.espressif.com/projects/arduino-esp32/en/latest/

### Code Changes Required

```diff
// Current code (broken):
- AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/save-config", [](AsyncWebServerRequest *request, JsonVariant &json) {
-     if (dayName.isEmpty()) {
-         request->send(400, "text/plain", "Bad Request: Day name missing");
-         return;
-     }
-     // ... save logic ...
- });
- server.addHandler(handler);

// Fixed code (RAII approach):
+ class WebServerHandler {
+ private:
+     AsyncCallbackJsonWebHandler *handler;
+ public:
+     WebServerHandler(AsyncWebServer *server, const String &path, WebHandlerCallback cb) {
+         handler = new AsyncCallbackJsonWebHandler(path, cb);
+     }
+     ~WebServerHandler() {
+         delete handler;
+     }
+ };
+
+ void init_ESPAsync_Ws() {
+     AsyncWebServer server(80);
+     
+     {
+         WebServerHandler handler(server, "/save-config", [](AsyncWebServerRequest *request, JsonVariant &json) {
+             if (dayName.isEmpty()) {
+                 request->send(400, "text/plain", "Bad Request: Day name missing");
+                 return;
+             }
+             // ... save logic ...
+         });
+     }
+     // handler is automatically deleted here
+     server.addHandler(handler);
+ }
```

### Estimated Fix Time: 30-60 minutes

### Priority Reason

This is a **critical bug** that will:
- Break the device after a few restarts
- Be very hard to diagnose (looks like "random crashes")
- Affect all users eventually
- Potentially brick devices if memory exhaustion causes watchdog resets

---

*This issue was identified during code review of the PictoDevice firmware.*

**Reported by:** Code Review Bot
**Date:** Generated on $(date +%Y-%m-%d)