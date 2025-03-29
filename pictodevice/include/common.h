#include <ArduinoJson.h>
//#include <pins_arduino.h>
#include <colors.h>

#define PD_VERSION_MAJOR 1
#define PD_VERSION_MINOR 2
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
const char* wifi_mngr_networkname = "pictodevice";
const char* wifi_mngr_password = "p@ssw0rd";

// 192.168.178.148 FIXME:  servername for HTTP server needs to change
String serverName = "http://192.168.178.148:8001";

#define FORMAT_SPIFFS_IF_FAILED true

// store values from JSON config file
const char* config_comment; // nullptr
const char* config_version; // "1.0.1"
const char* config_name; // "Peter"
const char* config_device_ip; // "128.8.2.123"
const char* config_date_created; // "24-03-2025"
const char* config_date_valid; // "23-03-2025"
int8_t config_activities_size = 0;
//int config_activities[0];
//String config_activities_name_nl[];   //   "name_nl": "douchen"
//int config_activities_order[];        //   "order": "002",
//String config_activities_picto[];     //   "picto": "shower.png",
int config_activities_size_max = 19;   // hardcoded maximum of activities still able to show mini menu

// box for picto 100 x 100 pixels 
int16_t picto_box_width = 100;
int16_t picto_box_height = 100;

// 135 x 240 pixels wordt aangestuurd door de ST7789V2.
// NOTE: LCD_HEIGHT and LCD_WIDTH are switched around
// in this case m5stcik: LCD_HEIGHT=240 and LCD_WIDTH=135
// because the screen is rotated!
#define MY_WIDTH  TFT_HEIGHT
#define MY_HEIGHT TFT_WIDTH

//time variables
String h, m, s;
int day, month;

//settime variables
bool setTimeDate = false;
int setData[8];  //setHour,setMin,setSec,setDate,setMonth,setYear; SET REGION , SET BEEPER;
String setDataLbl[8]={"HOUR","MIN","SEC","DATE","MON","YEAR","REGION","SOUND"};
int setMin[8]={0,0,0,1,1,24,0,0};
int setMax[8]={24,60,60,32,13,36,2,2};
int setPosX[8]={10,50,91,10,50,91,8,8};
int setPosY[8]={54,54,54,124,124,124,172,192};
int chosen=0;
//brightness and battery
int brightnes[6]={16,32,48,64,96,180};
int b=2;
int vol;
int volE;

//buzzer
int buzzer = 1;

//sleep variables
int sleepTime = 10;
int ts,tts = 0;
bool slp = false;

#define BUTTON_PRESSED LOW 
#define BUTTON_RELEASED HIGH

uint16_t ontime, offtime;
uint8_t i,num_codes;
uint8_t region;

#define MAX_IMAGE_WIDTH 100 // Adjust for your images
int16_t xpos = 12;
int16_t ypos = 12;
int current_activity_index = 0;

String lang = "nl"; // "en" for english, "nl" for dutch
