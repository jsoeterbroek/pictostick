#include <ArduinoJson.h>
//#include <pins_arduino.h>
#include <colors.h>

#define PD_VERSION_MAJOR 1
#define PD_VERSION_MINOR 1
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

int counter=0;

// status flags
boolean STATUS_WIFI_OK = false;
boolean STATUS_WIFI_MGR_OK = false;
boolean STATUS_WIFI_MGR_CONFIG_MODE_OK = false;
boolean STATUS_TIME_OK = false;
boolean STATUS_NTP_OK = false;
boolean STATUS_CONFIG_DATA_OK = false;
boolean STATUS_BLE_OK = false;
boolean STATUS_GET_CONFIG_DATA_SPIFF_OK = false;
boolean STATUS_GET_CONFIG_DATA_HTTP_OK = false;

boolean GET_CONFIG_DATA_HTTP = false;
boolean GET_CONFIG_DATA_SPIFF = true;

const char* ntpServer = "europe.pool.ntp.org";
#define NTP_TIMEZONE  "UTC+1"

// WiFi
const char* wifi_mngr_networkname = "pictostick";
const char* wifi_mngr_password = "password";

// 192.168.178.148
String serverName = "http://192.168.178.148:8001";

#define FORMAT_SPIFFS_IF_FAILED true

// json
const char* config_comment; // nullptr
const char* config_version; // "1.0.1"
const char* config_name; // "Peter"
const char* config_device_ip; // "128.8.2.123"
const char* config_date_created; // "24-03-2025"
const char* config_date_valid; // "23-03-2025"
int8_t config_activities_size = 0;

// 135 x 240 pixels wordt aangestuurd door de ST7789V2.
// NOTE: LCD_HEIGHT and LCD_WIDTH are switched around
// in this case m5stcik: LCD_HEIGHT=240 and LCD_WIDTH=135
// because the screen is rotated!
#define MY_WIDTH  TFT_HEIGHT
#define MY_HEIGHT TFT_WIDTH

int16_t middle_box_width = 100;
int16_t middle_box_height = 100;