// ABOUTME: Common runtime state variables for PictoStick
// ABOUTME: Only runtime state - configuration constants moved to config.h

#ifndef COMMON_H_
#define COMMON_H_

#include <ArduinoJson.h>
#include <colors.h>
#include <themes.h>
#include "config.h"

// =====================================================
// Version Information (runtime access)
// =====================================================
static inline int pd_version_major(void) {
  return PD_VERSION_MAJOR;
}
static inline int pd_version_minor(void) {
  return PD_VERSION_MINOR;
}
static inline int pd_version_patch(void) {
  return PD_VERSION_PATCH;
}

// =====================================================
// Configuration Data (loaded from config file)
// =====================================================
extern boolean GET_CONFIG_DATA_HTTP;
extern boolean GET_CONFIG_DATA_SPIFF;

extern const char *ntpServer;
extern String timezone;

// WiFi credentials
extern const char *wifi_mngr_networkname;
extern const char *wifi_mngr_password;

// Server configuration
extern String serverName;

// Configuration file path
extern const char *cfilename;

// Configuration values (loaded from JSON)
extern String config_comment;
extern String config_version;
extern String config_name;
extern String config_device_ip;
extern String config_date_created;
extern String config_date_valid;
extern int8_t config_activities_size;
extern int config_activities_size_max;

// =====================================================
// Display Configuration
// =====================================================
// Picto display box dimensions
extern int16_t picto_box_width;
extern int16_t picto_box_height;

// =====================================================
// Time Variables (runtime)
// =====================================================
extern String h, m, s;
extern int day, month;
extern int8_t hour;
extern int8_t minute;
extern int8_t second;

// =====================================================
// Battery Variables (runtime)
// =====================================================
extern int vol;
extern int volE;

// =====================================================
// Sleep Variables (runtime)
// =====================================================
extern int sleepTime;
extern int ts, tts;
extern bool slp;

// =====================================================
// UI Position Variables (runtime)
// =====================================================
extern int16_t xpos;
extern int16_t ypos;

// =====================================================
// Activity State (runtime)
// =====================================================
extern int current_activity_index;
extern int ps_current_activity_index;

// =====================================================
// Language and UI State (runtime)
// =====================================================
extern String lang;
extern bool marked_done;
extern int cursor;

#endif // COMMON_H_
