#include <ArduinoJson.h>

//#define TFT_SCREEN_M5STACK
#define TFT_SCREEN_WAVESHARE_ESP32_S3

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

// https://github.com/newdigate/rgb565_colors
#define RGB565_BLUE_AZURE 0x041F
#define RGB565_BLUE_LIGHT 0xAEBC
#define RGB565_BLACK_DIM 0x6B4D
#define RGB565_BLACK_OUTER_SPACE 0x4249 
#define RGB565_GRAY_BATTLESHIP 0x8430
#define RGB565_GRAY_MEDIUM 0xBDF7
#define RGB565_GRAY_LIGHT 0xD69A
#define RGB565_GRAY_WEB 0x8410
#define RGB565_GRAY_STONE 0x9470
#define RGB565_YELLOW 0xFFE0
#define RGB565_RED 0xF800
#define RGB565_RED_MAXIMUM 0xD104
#define RGB565_RED_BRICK 0xCA0A
#define RGB565_ORANGE 0xFBE0
#define RGB565_ORANGE_CADMIUM 0xEC25
#define RGB565_GREEN_GO 0x054C
#define RGB565_GREEN_FERN 0x75CF
#define RGB565_WHITE 0xFFFF
#define RGB565_TEAL 0x0410
#define RGB565_CORAL 0xFBEA

// colors
#define TEXT_COLOR           TFT_WHITE
#define FG_COLOR             TFT_WHITE
#define BG_COLOR             TFT_BLACK
#define LINE_COLOR           TFT_DARKGREY
#define RECT_BG_COLOR        TFT_BLACK
#define WARN_NOK             TFT_ORANGE
#define WARN_OK              TFT_GREEN
#define DAYPERIOD1_BG_COLOR  RGB565_CORAL
#define DAYPERIOD2_BG_COLOR  RGB565_TEAL
#define DAYPERIOD3_BG_COLOR  RGB565_RED_BRICK
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
