#include <ArduinoJson.h>
#include <colors.h>

#define PD_VERSION_MAJOR 1
#define PD_VERSION_MINOR 4
#define PD_VERSION_PATCH 1

static inline int pd_version_major(void) {
  return PD_VERSION_MAJOR;
}
static inline int pd_version_minor(void) {
  return PD_VERSION_MINOR;
}
static inline int pd_version_patch(void) {
  return PD_VERSION_PATCH;
}

extern boolean GET_CONFIG_DATA_HTTP;
extern boolean GET_CONFIG_DATA_SPIFF;

extern const char *ntpServer;
#define NTP_TIMEZONE "UTC+1"
extern String timezone;

// WiFi
extern const char *wifi_mngr_networkname;
extern const char *wifi_mngr_password;

// FIXME: servername for HTTP server needs to change
extern String serverName;

#define FORMAT_SPIFFS_IF_FAILED true

// configuration file in json format stored in SPIFFS fs
extern const char *cfilename;

// store values from JSON config file
extern const char *config_comment;       // nullptr
extern const char *config_version;       // "1.0.1"
extern const char *config_name;          // "Peter"
extern const char *config_device_ip;     // "128.8.2.123"
extern const char *config_date_created;  // "24-03-2025"
extern const char *config_date_valid;    // "23-03-2025"
extern int8_t config_activities_size;
extern int config_activities_size_max;  // hardcoded maximum of activities still able to show mini menu

// box for picto 100 x 100 pixels
extern int16_t picto_box_width;
extern int16_t picto_box_height;

// 135 x 240 pixels wordt aangestuurd door de ST7789V2.
// NOTE: LCD_HEIGHT and LCD_WIDTH are switched around
// in this case m5stcik: LCD_HEIGHT=240 and LCD_WIDTH=135
// because the screen is rotated!
#define MY_WIDTH  TFT_HEIGHT
#define MY_HEIGHT TFT_WIDTH

//time variables
extern String h, m, s;
extern int day, month;
extern int8_t hour;
extern int8_t minute;
extern int8_t second;

//battery
extern int vol;
extern int volE;

//sleep variables
extern int sleepTime;
extern int ts, tts;
extern bool slp;

#define BUTTON_PRESSED  LOW
#define BUTTON_RELEASED HIGH

#define MAX_IMAGE_WIDTH 100  // Adjust for your images
extern int16_t xpos;
extern int16_t ypos;
extern int current_activity_index;
extern int ps_current_activity_index;

extern String lang;  // "en" for english, "nl" for dutch
extern bool marked_done;

extern int cursor;
