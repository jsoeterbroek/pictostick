
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