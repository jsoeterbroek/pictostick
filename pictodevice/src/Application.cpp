/*
 * ABOUTME: Application class implementation for PictoStick
 * ABOUTME: Manages device state, mode transitions, and main loop
 */

#include "Application.h"
#include <M5StickCPlus2.h>
#include "common.h"
#include "devicemode.h"
#include "PSpref.h"
#include "status.h"
#include "init.h"
#include "uimain.h"
#include "uisetup.h"
#include "uiconfig.h"

// Global application instance
Application* g_app = nullptr;

// Forward declarations from existing code
extern void drawMain();
extern void drawDeviceModeConfig(uint8_t);
extern void drawDeviceMode1();
extern void drawDeviceMode2();
extern void drawDeviceMode3();
extern bool draw_device_mode_config;
extern bool draw_device_mode_1;
extern bool draw_device_mode_2;
extern bool draw_device_mode_3;
extern uint8_t desired_devicemode;
extern int devicemode;
extern int sleepTime;
extern bool slp;
extern int ts, tts;

Application::Application() 
    : m_currentMode(AppMode::MODE_BOOT)
    , m_requestedMode(AppMode::MODE_BOOT)
    , m_initialized(false)
    , m_modeChangePending(false)
    , m_initTime(0)
    , m_lastLoopTime(0)
    , m_display(nullptr)
    , m_prefs(nullptr)
    , m_lastActivityTime(0)
    , m_sleepTimeout(60)
    , m_isSleeping(false)
{
}

Application::~Application() {
    // Cleanup if needed
    g_app = nullptr;
}

void Application::init() {
    if (m_initialized) {
        return;  // Already initialized
    }
    
    m_initTime = millis();
    m_lastLoopTime = millis();
    m_lastActivityTime = millis();
    
    // Set global instance pointer
    g_app = this;
    
    // Initialize preferences (existing function)
    init_preferences();
    
    // Get sleep timeout from preferences
    m_sleepTimeout = get_pspref_timeout();
    
    m_initialized = true;
}

void Application::setup() {
    if (!m_initialized) {
        return;  // Must call init() first
    }
    
    // Initialize device mode (existing function)
    init_devicemode();
    
    // Map device mode to application mode
    switch (devicemode) {
        case 1:
            m_currentMode = AppMode::MODE_WIFI_CONFIG;
            break;
        case 2:
            m_currentMode = AppMode::MODE_SETUP;
            break;
        case 3:
            m_currentMode = AppMode::MODE_DAY_SETUP;
            break;
        case 4:
        default:
            m_currentMode = AppMode::MODE_MAIN;
            break;
    }
}

void Application::loop() {
    if (!m_initialized) {
        return;
    }
    
    m_lastLoopTime = millis();
    
    // Check if device is in sleep mode
    if (slp) {
        m_isSleeping = true;
        // Wake up on next iteration (handled by existing code in main.cpp)
        StickCP2.Display.setBrightness(get_pspref_brightness());
        slp = false;
        m_sleepTimeout = get_pspref_timeout();
        m_lastActivityTime = millis();
        m_isSleeping = false;
    }
    
    // Process any requested mode changes
    processModeChange();
    
    // Handle button inputs (existing logic from uimain.cpp)
    handleInputs();
    
    // Update display based on current mode
    updateDisplay();
    
    // Check for power saving timeout
    checkPowerSaving();
}

AppMode Application::getCurrentMode() const {
    return m_currentMode;
}

void Application::requestModeChange(AppMode newMode) {
    if (m_currentMode != newMode) {
        m_requestedMode = newMode;
        m_modeChangePending = true;
    }
}

const char* Application::getModeName(AppMode mode) {
    switch (mode) {
        case AppMode::MODE_BOOT: return "BOOT";
        case AppMode::MODE_FIRST_START: return "FIRST_START";
        case AppMode::MODE_SETUP: return "SETUP";
        case AppMode::MODE_WIFI_CONFIG: return "WIFI_CONFIG";
        case AppMode::MODE_MAIN: return "MAIN";
        case AppMode::MODE_ACTIVITY0: return "ACTIVITY0";
        case AppMode::MODE_ACTIVITY1: return "ACTIVITY1";
        case AppMode::MODE_ACTIVITY2: return "ACTIVITY2";
        case AppMode::MODE_ACTIVITY3: return "ACTIVITY3";
        case AppMode::MODE_ACTIVITY4: return "ACTIVITY4";
        case AppMode::MODE_POWER: return "POWER_SAVE";
        case AppMode::MODE_DAY_SETUP: return "DAY_SETUP";
        default: return "UNKNOWN";
    }
}

bool Application::isInitialized() const {
    return m_initialized;
}

unsigned long Application::getUptime() const {
    return millis() - m_initTime;
}

void Application::reset() {
    ESP.restart();
}

void Application::processModeChange() {
    if (!m_modeChangePending) {
        return;
    }
    
    // Apply the mode change
    m_currentMode = m_requestedMode;
    m_modeChangePending = false;
    
    // Reset activity timer on mode change
    m_lastActivityTime = millis();
}

void Application::handleInputs() {
    // Only handle inputs in MAIN mode
    if (m_currentMode != AppMode::MODE_MAIN) {
        return;
    }
    
    // Delegate to existing button handling code
    // This maintains backward compatibility with uimain.cpp
    // The actual button logic remains in drawMain() for now
}

void Application::updateDisplay() {
    // Route to appropriate display function based on mode
    if (draw_device_mode_config) {
        drawDeviceModeConfig(desired_devicemode);
    } else if (draw_device_mode_1) {
        drawDeviceMode1();
    } else if (draw_device_mode_2) {
        drawDeviceMode2();
    } else if (draw_device_mode_3) {
        drawDeviceMode3();
    } else {
        // Default to main display
        drawMain();
    }
}

void Application::checkPowerSaving() {
    // Only check in MAIN mode
    if (m_currentMode != AppMode::MODE_MAIN) {
        return;
    }
    
    // Check if timeout has elapsed since last activity
    unsigned long elapsed = millis() - m_lastActivityTime;
    if (elapsed >= (unsigned long)m_sleepTimeout * 1000) {
        // Trigger sleep
        slp = true;
        m_isSleeping = true;
    }
}
