// ABOUTME: Configuration constants for PictoStick device
// ABOUTME: Separates compile-time configuration from runtime state

#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>

// =====================================================
// Screen Configuration
// =====================================================
#define MY_WIDTH      135
#define MY_HEIGHT     240
#define MY_REFRESH_RATE 60

// =====================================================
// GPIO Configuration
// =====================================================
#define BTN_A_PIN     0
#define BTN_B_PIN     1

// =====================================================
// Storage Configuration
// =====================================================
#define SPIFFS_PATH           "/spiffs"
#define CONFIG_FILE           "/config.json"
#define ACTIVITIES_PATH       "/activities"
#define PICTO_PATH            "/picto"
#define SPIFFS_SIZE           256000
#define FORMAT_SPIFFS_IF_FAILED true

// =====================================================
// Default Settings
// =====================================================
#define DEFAULT_LANGUAGE      "en"
#define DEFAULT_TIMEOUT       25
#define DEFAULT_BRIGHTNESS    64
#define DEFAULT_VOLUME        8
#define DEFAULT_VOLUME_EQUALIZER 0

// =====================================================
// Maximum Limits
// =====================================================
#define MAX_ACTIVITIES        5
#define MAX_DAYS              7
#define MAX_IMAGE_WIDTH       100

// =====================================================
// NTP Configuration
// =====================================================
#define NTP_TIMEZONE          "UTC+1"
#define DEFAULT_TIMEZONE      "CET-1CEST,M3.5.0,M10.5.0/3"

// =====================================================
// Version Information
// =====================================================
#define PD_VERSION_MAJOR      1
#define PD_VERSION_MINOR      6
#define PD_VERSION_PATCH      1

// =====================================================
// Button States
// =====================================================
#define BUTTON_PRESSED        LOW
#define BUTTON_RELEASED       HIGH

#endif // CONFIG_H_
