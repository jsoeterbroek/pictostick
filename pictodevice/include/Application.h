/*
 * ABOUTME: Central Application class for PictoStick
 * ABOUTME: Encapsulates device state, mode management, and main loop logic
 */

#pragma once

#include <Arduino.h>
#include <M5StickCPlus2.h>

// Forward declarations
class Display;
class PreferencesManager;

/**
 * Application modes representing different device states
 */
enum class AppMode {
    MODE_BOOT = 0,           // Initial boot
    MODE_FIRST_START = 1,    // First-time setup
    MODE_SETUP = 2,          // Configuration mode
    MODE_WIFI_CONFIG = 3,    // WiFi manager mode
    MODE_MAIN = 4,           // Normal operation (activity display)
    MODE_ACTIVITY0 = 5,      // Activity 0 (reserved)
    MODE_ACTIVITY1 = 6,      // Activity 1 (reserved)
    MODE_ACTIVITY2 = 7,      // Activity 2 (reserved)
    MODE_ACTIVITY3 = 8,      // Activity 3 (reserved)
    MODE_ACTIVITY4 = 9,      // Activity 4 (reserved)
    MODE_POWER = 10,         // Power saving mode
    MODE_DAY_SETUP = 11      // Day configuration mode
};

/**
 * Main Application class
 * 
 * Encapsulates:
 * - Device state management
 * - Mode transitions
 * - Main loop coordination
 * - Settings access
 * 
 * Usage:
 *   Application app;
 *   app.init();
 *   app.setup();
 *   app.loop();
 */
class Application {
public:
    // Constructor/Destructor
    Application();
    ~Application();
    
    /**
     * Initialize application components
     * Call once during startup
     */
    void init();
    
    /**
     * Setup hardware and initial state
     * Call once after init()
     */
    void setup();
    
    /**
     * Main application loop
     * Call repeatedly from Arduino loop()
     */
    void loop();
    
    /**
     * Get current application mode
     * @return Current AppMode value
     */
    AppMode getCurrentMode() const;
    
    /**
     * Request mode change
     * @param newMode Target mode
     */
    void requestModeChange(AppMode newMode);
    
    /**
     * Get human-readable mode name
     * @param mode Mode to convert
     * @return String name of mode
     */
    static const char* getModeName(AppMode mode);
    
    /**
     * Check if application is initialized
     * @return true if init() has been called
     */
    bool isInitialized() const;
    
    /**
     * Get uptime in milliseconds
     * @return Milliseconds since init()
     */
    unsigned long getUptime() const;
    
    /**
     * Trigger soft reset (reinitialize without power cycle)
     */
    void reset();

private:
    // State variables
    AppMode m_currentMode;
    AppMode m_requestedMode;
    bool m_initialized;
    bool m_modeChangePending;
    unsigned long m_initTime;
    unsigned long m_lastLoopTime;
    
    // Component references (initialized later)
    // Note: We use pointers to avoid circular dependencies
    Display* m_display;
    PreferencesManager* m_prefs;
    
    // Internal methods
    void processModeChange();
    void handleInputs();
    void updateDisplay();
    void checkPowerSaving();
    
    // Power saving
    unsigned long m_lastActivityTime;
    int m_sleepTimeout;
    bool m_isSleeping;
};

// Global application instance (optional, for backward compatibility)
extern Application* g_app;
