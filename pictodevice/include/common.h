#include <ArduinoJson.h>
#include <colors.h>

//#define TFT_SCREEN_M5STACK
#define TFT_SCREEN_WAVESHARE_ESP32_S3

// waveshare ST7789 240x320
#ifdef TFT_SCREEN_WAVESHARE_ESP32_S3
    #include "waveshare.h"
#endif

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
boolean STATUS_CONFIG_DATA_OK = false;
boolean STATUS_BLE_OK = false;
boolean STATUS_GET_CONFIG_DATA_SPIFF_OK = false;
boolean STATUS_GET_CONFIG_DATA_HTTP_OK = false;

boolean GET_CONFIG_DATA_HTTP = false;
boolean GET_CONFIG_DATA_SPIFF = true;

unsigned short grays[13];

const char* ntpServer = "europe.pool.ntp.org";

// WiFi
const char* wifi_mngr_networkname = "espconf";
const char* wifi_mngr_password = "password";

// 192.168.178.148
String serverName = "http://192.168.178.148:8001";

#define FORMAT_SPIFFS_IF_FAILED true

// 
String dayPeriodNow = "unknown";
