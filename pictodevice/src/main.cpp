#include <Arduino.h>
#include <M5StickCPlus2.h>
#include <common.h>
#include <WiFiManager.h>
#include <SPI.h>
#include <time.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <ESP32Time.h>
#include <TFT_eSPI.h>
#include <FS.h>
#include <SPIFFS.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite errSprite = TFT_eSprite(&tft);

struct tm timeinfo;
ESP32Time rtc(0);
#define EEPROM_SIZE 4

//brightness and battery
int brightnes[6]={16,32,48,64,96,180};
int b=2;
int vol;
int volE;

#define BUTTON_PRESSED LOW 
#define BUTTON_RELEASED HIGH

//scroling message on bottom right side
//String Wmsg = "";

JsonDocument cdoc;
//float pData[4];

void setTime() {
    Serial.println(" ");
    Serial.print("connecting to time server ");
    tft.print("connecting to time server ");
    Serial.println(ntpServer);
    tft.println(ntpServer);
    Serial.println(" ");
    // Init and get the time
    configTime(0, 0, ntpServer);
    delay(500);
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
    tzset();
    delay(100);
    if(!getLocalTime(&timeinfo)){
        tft.println("ERROR: failed to obtain time");
        Serial.println("ERROR: failed to obtain time");
        delay(10000);
    } else {
        Serial.println("OK: obtained time");
        tft.println("OK: obtained time");
        Serial.println(&timeinfo, " %A, %B %d %Y %H:%M:%S");
        tft.println(&timeinfo, " %A, %B %d %Y %H:%M:%S");
        STATUS_TIME_OK = true;
    }
}

void configModeCallback (WiFiManager *myWiFiManager) {

    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    Serial.println(myWiFiManager->getConfigPortalSSID());

    tft.println(" ");
    tft.println(" ******************");
    tft.println(" Entered config mode:");
    tft.print(" Webportal started at:");
    tft.println(WiFi.softAPIP());
    tft.print(" please connect to WiFi SSID: ");
    tft.println(myWiFiManager->getConfigPortalSSID());
    tft.println(" to configure WiFI for this device");
    tft.println(" ******************");

    STATUS_WIFI_MGR_CONFIG_MODE_OK = true;
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);
    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }
    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readConfigFile(fs::FS &fs, const char * path){

    STATUS_GET_CONFIG_DATA_SPIFF_OK = false;

    static uint8_t buf[512];
    size_t len = 0;
    Serial.printf("Reading config file: %s\r\n", path);
    File cfile = fs.open(path);
    if(!cfile || cfile.isDirectory()){
        Serial.println("ERROR: failed to open config file for reading");
    } else {
        DeserializationError error = deserializeJson(cdoc, cfile);
        if (!error) {
            Serial.println("deserializeJson OK");
            STATUS_GET_CONFIG_DATA_SPIFF_OK = true;
        } else {
            Serial.print("ERROR: deserializeJson returned ");
            Serial.println(error.c_str());
        }
    }
    cfile.close();
}

void getConfigDataSPIFF () {

    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    //listDir(SPIFFS, "/", 0);
    readConfigFile(SPIFFS, "/data.json");
}

void getConfigDataHTTP () {

    STATUS_GET_CONFIG_DATA_HTTP_OK = false;
    HTTPClient http;
    String serverPath = serverName;
    String payloadStr = "";
      
    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payloadStr = http.getString();
        DeserializationError error = deserializeJson(cdoc, payloadStr);

        if (!error) {
            STATUS_CONFIG_DATA_OK = true;
            STATUS_GET_CONFIG_DATA_HTTP_OK = true;
        } else {
            Serial.print("ERROR JSON-a: ");
            Serial.println(error.c_str());
            STATUS_GET_CONFIG_DATA_HTTP_OK = false;
        }
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        STATUS_GET_CONFIG_DATA_HTTP_OK = false;
    }
    // Free resources
    http.end();
}

void draw_splash() {

    tft.fillScreen(TFT_WHITE);
    tft.println("draw_splash screen");
    tft.println("FIXME: logo here");
}

void draw_bg() {

    tft.fillScreen(BG_COLOR);
    sprite.createSprite(MY_WIDTH, MY_HEIGHT);
    sprite.fillSprite(TFT_TRANSPARENT);
    
    // main
    // middle prev
    sprite.fillSmoothRoundRect(-46, 6, middle_box_width, middle_box_height, 2, FG_COLOR, BG_COLOR);
    // middle now
    sprite.fillSmoothRoundRect(MY_WIDTH / 2 - 50, 6 , middle_box_width, middle_box_height, 2, FG_COLOR, BG_COLOR);
    // middle next
    sprite.fillSmoothRoundRect(178, 6, middle_box_width, middle_box_height, 2, FG_COLOR, BG_COLOR);

    // bottom
    // DayPeriod 1 "morning"
    sprite.fillSmoothRoundRect(10, 118, 40, 10, 2, DAYPERIOD1_BG_COLOR, BG_COLOR);
    // DayPeriod 2 "afternoon"
    sprite.fillSmoothRoundRect(54, 118, 40, 10, 2, DAYPERIOD2_BG_COLOR, BG_COLOR);
    // DayPeriod 3 "evening"
    sprite.fillSmoothRoundRect(110, 118, 40, 10, 2, DAYPERIOD3_BG_COLOR, BG_COLOR);


    sprite.setTextDatum(0);

    sprite.pushSprite(0, 0, TFT_TRANSPARENT);
}

void draw() {

    //tft.println("draw");
    //if(STATUS_TIME_OK) {
    //    tft.print("currently the period is ");
    //    tft.println(dayPeriodNow);
    //}
}

void setup() {

    // https://community.m5stack.com/topic/5943/m5stickc-plus2-and-tft_espi-problem/3
    pinMode(35,INPUT_PULLUP);
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    auto cfg = M5.config();
    StickCP2.begin(cfg);
    //StickCP2.Rtc.setDateTime( { { 2024, 2, 17 }, { 8, 20, 0 } } );
    StickCP2.Display.setBrightness(brightnes[b]);

    delay(5000);
    Serial.begin(115200);
    Serial.println("start initialisation..");

    //Backlight_Init();

    tft.init();
    tft.setRotation(3);
    tft.invertDisplay(1);
    tft.fillScreen(TFT_WHITE);
    tft.println("");
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.println("start initialisation..");

    delay(3000);
    String software = "Pictodevice ";
    software += String("V") + pd_version_major() + "." + pd_version_minor() + "." + pd_version_patch();
    tft.println(software);

    // WiFi Manager
    WiFiManager wm;

    // for testing
    //wm.resetSettings();

    wm.setConfigPortalTimeout(5000);
    wm.setAPCallback(configModeCallback);

    bool res;
    res = wm.autoConnect(wifi_mngr_networkname, wifi_mngr_password);

    if (!res) {
        Serial.println("Failed to connect and hit timeout");
        delay(3000);
        ESP.restart();
    } else {
        STATUS_WIFI_MGR_OK = true;
        tft.println("WiFi connected.");
        Serial.println("WiFi connected.");
    }

    setTime();

    // get config data
    // FIXME: logica invoegen om te checken of config data van fs moet komen of (nieuwere) data van HTTP (checksum vergelijking?) 
    if(GET_CONFIG_DATA_SPIFF) {
        getConfigDataSPIFF();
    } else {
        getConfigDataHTTP();
    }

    // // extract values from config JSON object

    const char* comment = cdoc["comment"]; // nullptr
    const char* version = cdoc["version"]; // "1.0.1"
    const char* name = cdoc["name"]; // "Peter"
    const char* device_ip = cdoc["device_ip"]; // "128.8.2.123"

    JsonObject morning = cdoc["morning"];
    const char* morning_periodname_en = morning["periodname_en"]; // "morning"
    const char* morning_periodname_nl = morning["periodname_nl"]; // "morgen"
    const char* morning_time_from = morning["time_from"]; // "07:00"
    const char* morning_time_to = morning["time_to"]; // "12:00"

    // for (JsonObject morning_activity : morning["activities"].as<JsonArray>()) {
    //   const char* morning_activity_order = morning_activity["order"]; // "001", "002", "003", "004"
    //   const char* morning_activity_picto = morning_activity["picto"]; // "0000_slapen.png", ...
    //   const char* morning_activity_name_en = morning_activity["name_en"]; // "waking", "shower", "brushing ...
    //   const char* morning_activity_name_nl = morning_activity["name_nl"]; // "opstaan", "douchen", "tanden ...
    // }

    JsonObject afternoon = cdoc["afternoon"];
    const char* afternoon_periodname_en = afternoon["periodname_en"]; // "afternoon"
    const char* afternoon_periodname_nl = afternoon["periodname_nl"]; // "middag"
    const char* afternoon_time_from = afternoon["time_from"]; // "12:00"
    const char* afternoon_time_to = afternoon["time_to"]; // "17:00"

    // for (JsonObject afternoon_activity : afternoon["activities"].as<JsonArray>()) {
    //   const char* afternoon_activity_order = afternoon_activity["order"]; // "001", "002", "003"
    //   const char* afternoon_activity_picto = afternoon_activity["picto"]; // "0005_morderdagmaal.png", ...
    //   const char* afternoon_activity_name_en = afternoon_activity["name_en"]; // "lunch", "shopping", "sawing"
    //   const char* afternoon_activity_name_nl = afternoon_activity["name_nl"]; // "lunch", "boodschappen", ...
    // }

    JsonObject evening = cdoc["evening"];
    const char* evening_periodname_en = evening["periodname_en"]; // "evening"
    const char* evening_periodname_nl = evening["periodname_nl"]; // "avond"
    const char* evening_time_from = evening["time_from"]; // "17:00"
    const char* evening_time_to = evening["time_to"]; // "22:00"

    // for (JsonObject evening_activity : evening["activities"].as<JsonArray>()) {
    //   const char* evening_activity_order = evening_activity["order"]; // "001", "002", "003", "004", "005"
    //   const char* evening_activity_picto = evening_activity["picto"]; // "0002_avondmaal.png", ...
    //   const char* evening_activity_name_en = evening_activity["name_en"]; // "dinner", "evening walk", "watch ...
    //   const char* evening_activity_name_nl = evening_activity["name_nl"]; // "avondmaal", "avondwandeling", ...
    // }

    const char* date_created = cdoc["date_created"]; // "24-03-2025"
    const char* date_valid = cdoc["date_valid"]; // "23-03-2025"


    if (STATUS_GET_CONFIG_DATA_SPIFF_OK) {
       STATUS_CONFIG_DATA_OK = true;
       tft.println("config successfully read from fs");
       Serial.println("config successfully read from fs");
    } else {
       tft.println("ERROR: error reading config from fs");
       Serial.println("ERROR: error reading config from fs");
    }
    delay(2000);
    tft.println("initialisation complete");
    Serial.println("initialisation complete");
    delay(2000);
    draw_splash();
    delay(2000);
    draw_bg();
}

void loop() {
    struct tm _time;
    char chrHour[3];
    char chrMinute[3];
    char chrTime[9];
    char chrDate[20];
    char chrDay[10];
    if (STATUS_TIME_OK) {
        if(getLocalTime(&_time)){
            strftime(chrHour, 3, "%H", &_time);
            strftime(chrMinute, 3, "%M", &_time);
            strftime(chrTime, 9, "%H:%M", &_time);
            strftime(chrDate, 20, "%d %B %Y", &_time);
            strftime(chrDay, 10, "%A", &_time);
        }
    }
    int intHour = atoi(chrHour);
    int intMinute = atoi(chrMinute);
    String strDay = chrDay;
    String strDate = chrDate;
    String strTime = chrTime;
  
    //Serial.print(strDay);
    //Serial.print(strDate);
    //Serial.println(strTime);

    // FIXME: the hours should be configurable and/or taken from config
    // uncomment below to test
    // TEST: intHour = 9;
    if (intHour > 4 && intHour < 12) {
        dayPeriodNow = "morning";
    } else if (intHour >= 12 && intHour < 17) {
        dayPeriodNow = "afternoon";
    } else if (intHour >= 17 && intHour <= 23) {
        dayPeriodNow = "evening";
    } else { 
        dayPeriodNow = "night";
    }

    draw();
    delay(5000);
}