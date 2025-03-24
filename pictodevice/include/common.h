#include <ArduinoJson.h>

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

// colors
#define TEXT_COLOR           TFT_WHITE
#define FG_COLOR             TFT_WHITE
#define BG_COLOR             TFT_BLACK
#define LINE_COLOR           TFT_DARKGREY
#define RECT_BG_COLOR        TFT_BLACK
#define WARN_NOK             TFT_ORANGE
#define WARN_OK              TFT_GREEN
unsigned short grays[13];


const char* ntpServer = "europe.pool.ntp.org";

// WiFi
const char* wifi_mngr_networkname = "espconf";
const char* wifi_mngr_password = "password";

// 192.168.178.148
String serverName = "http://192.168.178.148:8001";

#define FORMAT_SPIFFS_IF_FAILED true

// 
String dayPeriodNow;