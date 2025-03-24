#include <ArduinoJson.h>

//#################### EDIT THIS  ###################
//time zone  
int zone = 2;
String town = "Zagreb";
String myAPI = "d0d0bf1bb46822e5dce67c95f4fd0800";
String units = "metric";  //  metric, imperial
//#################### end of edits ###################

//additional variables
int ani = 100;
float maxT;
float minT;
unsigned long timePased = 0;
int counter=0;

const char* config_version;
const char* config_date_created;
const char* config_date_valid;
JsonDocument json_config_period_morning_activities;
JsonDocument json_config_period_afternoon_activities;
JsonDocument json_config_period_evening_activities;
int morning_activities_size;
int afternoon_activities_size;
int evening_activities_size;
// typedef struct {
//     int order;
//     char picto[20];
//     char name_en[20];
//     char name_nl[20];
// } Activity;

// //typedef struct {
// //} ActivityList;

// typedef struct {
//     char periodname[10];
//     Activity activities[7];
// } ActivitiesByPeriod;

// ActivitiesByPeriod activitiesbyperiod[] {
//     {"morning", 
//         { {"order", 001}, {"picto", "picto1.png"}, {"name_en", "foo"}, {"name_nl", "bar1"} },
//         { {"order", 002}, {"picto", "picto3.png"}, {"name_en", "foo"}, {"name_nl", "bar3"} },
//         { {"order", 003}, {"picto", "picto7.png"}, {"name_en", "foo"}, {"name_nl", "bar5"} }
//     },
//     {"afternoon", 
//         { {"order", 001}, {"picto", "picto1.png"}, {"name_en", "foo"}, {"name_nl", "bar1"} },
//         { {"order", 002}, {"picto", "picto3.png"}, {"name_en", "foo"}, {"name_nl", "bar3"} },
//         { {"order", 003}, {"picto", "picto7.png"}, {"name_en", "foo"}, {"name_nl", "bar5"} }
//     },
//     {"evening", 
//         { {"order", 001}, {"picto", "picto1.png"}, {"name_en", "foo"}, {"name_nl", "bar1"} },
//         { {"order", 002}, {"picto", "picto3.png"}, {"name_en", "foo"}, {"name_nl", "bar3"} },
//         { {"order", 003}, {"picto", "picto7.png"}, {"name_en", "foo"}, {"name_nl", "bar5"} }
//     }
// };
// Cocktail cocktails[] = {
//    {"Americano", { {"campari", 1}, {"red vermouth", 2}, {"soda", 3} } },
//    {"Aviation", { {"gin", 1}, {"cherry liqueur", 2}, {"creme de violette", 3}, {"lemon juice", 4} } },
//    {"B52 Cocktail", { {"kahlua", 1}, {"baileys", 2}, {"trand marnier", 3} } }
//  };

// dinges
// [
//     {
//       "order": "001",
//       "picto": "0002_avondmaal.png",
//       "name_en": "dinner",
//       "name_nl": "avondmaal"
//     },
//     {
//       "order": "002",
//       "picto": "0011_wandeling_avond.png",
//       "name_en": "evening walk",
//       "name_nl": "avondwandeling"
//     },
//     {
//       "order": "003",
//       "picto": "0010_tv_kijken.png",
//       "name_en": "watch tv",
//       "name_nl": "tv kijken"
//     },
// ]

// status flags
boolean STATUS_WIFI_OK = false;
boolean STATUS_WIFI_MGR_OK = false;
boolean STATUS_WIFI_MGR_CONFIG_MODE_OK = false;
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

const char* ntpServer = "pool.ntp.org";

// WiFi
const char* wifi_mngr_networkname = "espconf";
const char* wifi_mngr_password = "password";

// 192.168.178.148
String serverName = "http://192.168.178.148:8001";

#define FORMAT_SPIFFS_IF_FAILED true