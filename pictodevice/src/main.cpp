#include <Arduino.h>
#include <common.h>
#include <WiFiManager.h>
#include <SPI.h>
#include <time.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <ESP32Time.h>
//#include <lvgl.h>
//#include <ui.h>
#include "FS.h"
#include "SPIFFS.h"

struct tm timeinfo;
ESP32Time rtc(0);

//scroling message on bottom right side
//String Wmsg = "";

JsonDocument cdoc;
//float pData[4];

void setTime() {
    configTime(3600 * zone, 0, ntpServer);
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        rtc.setTimeStruct(timeinfo);
    }
}

void configModeCallback (WiFiManager *myWiFiManager) {
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    Serial.println(myWiFiManager->getConfigPortalSSID());
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

void setup() {

    // FIXME: crash
    //ui_ScreenSPLASH_screen_init();

    delay(5000);
    Serial.begin(115200);
    Serial.println("start initialisation..");

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
        Serial.println("Connected.");
    }
    setTime();

    // get config data
    // FIXME: logica invoegen om te checken of config data van fs moet komen of (nieuwere) data van HTTP (checksum vergelijking?) 
    if(GET_CONFIG_DATA_SPIFF) {
        getConfigDataSPIFF();
    } else {
        getConfigDataHTTP();
    }

//     // extract values from config JSON object

// const char* comment = doc["comment"]; // nullptr
// const char* version = doc["version"]; // "1.0.1"
// const char* name = doc["name"]; // "Peter"
// const char* device_ip = doc["device_ip"]; // "128.8.2.123"

// JsonObject morning = doc["morning"];
// const char* morning_periodname_en = morning["periodname_en"]; // "morning"
// const char* morning_periodname_nl = morning["periodname_nl"]; // "morgen"
// const char* morning_time_from = morning["time_from"]; // "07:00"
// const char* morning_time_to = morning["time_to"]; // "12:00"

// for (JsonObject morning_activity : morning["activities"].as<JsonArray>()) {

//   const char* morning_activity_order = morning_activity["order"]; // "001", "002", "003", "004"
//   const char* morning_activity_picto = morning_activity["picto"]; // "0000_slapen.png", ...
//   const char* morning_activity_name_en = morning_activity["name_en"]; // "waking", "shower", "brushing ...
//   const char* morning_activity_name_nl = morning_activity["name_nl"]; // "opstaan", "douchen", "tanden ...

// }

// JsonObject afternoon = doc["afternoon"];
// const char* afternoon_periodname_en = afternoon["periodname_en"]; // "afternoon"
// const char* afternoon_periodname_nl = afternoon["periodname_nl"]; // "middag"
// const char* afternoon_time_from = afternoon["time_from"]; // "12:00"
// const char* afternoon_time_to = afternoon["time_to"]; // "17:00"

// for (JsonObject afternoon_activity : afternoon["activities"].as<JsonArray>()) {

//   const char* afternoon_activity_order = afternoon_activity["order"]; // "001", "002", "003"
//   const char* afternoon_activity_picto = afternoon_activity["picto"]; // "0005_morderdagmaal.png", ...
//   const char* afternoon_activity_name_en = afternoon_activity["name_en"]; // "lunch", "shopping", "sawing"
//   const char* afternoon_activity_name_nl = afternoon_activity["name_nl"]; // "lunch", "boodschappen", ...

// }

// JsonObject evening = doc["evening"];
// const char* evening_periodname_en = evening["periodname_en"]; // "evening"
// const char* evening_periodname_nl = evening["periodname_nl"]; // "avond"
// const char* evening_time_from = evening["time_from"]; // "17:00"
// const char* evening_time_to = evening["time_to"]; // "22:00"

// for (JsonObject evening_activity : evening["activities"].as<JsonArray>()) {

//   const char* evening_activity_order = evening_activity["order"]; // "001", "002", "003", "004", "005"
//   const char* evening_activity_picto = evening_activity["picto"]; // "0002_avondmaal.png", ...
//   const char* evening_activity_name_en = evening_activity["name_en"]; // "dinner", "evening walk", "watch ...
//   const char* evening_activity_name_nl = evening_activity["name_nl"]; // "avondmaal", "avondwandeling", ...

// }

// const char* date_created = doc["date_created"]; // "24-03-2025"
// const char* date_valid = doc["date_valid"]; // "23-03-2025"

    config_version = cdoc["version"];
    json_config_period_morning_activities = cdoc["morning"]["activities"].as<JsonVariant>();
    json_config_period_afternoon_activities = cdoc["afternoon"]["activities"].as<JsonVariant>();
    json_config_period_evening_activities = cdoc["evening"]["activities"].as<JsonVariant>();
    morning_activities_size = json_config_period_morning_activities.size();
    afternoon_activities_size = json_config_period_afternoon_activities.size();
    evening_activities_size = json_config_period_evening_activities.size();

    config_date_created = cdoc["date_created"];
    config_date_valid = cdoc["date_valid"];


    if (STATUS_GET_CONFIG_DATA_SPIFF_OK) {
       STATUS_CONFIG_DATA_OK = true;
       Serial.println("config successfully read from fs");
    } else {
       Serial.println("ERROR: error reading config from fs");
    }
    delay(4000);
    Serial.println("initialisation complete");
    delay(4000);
}

void draw() {

    Serial.println("draw");
    if (STATUS_CONFIG_DATA_OK) {
        //Serial.println(config_date_created);
        //char buffer[800];
        //serializeJsonPretty(json_config_period_afternoon_activities, buffer);
        //Serial.println(buffer);
        Serial.print("morning_activities_size: ");
        Serial.println(morning_activities_size);
        Serial.print("afternoon_activities_size: ");
        Serial.println(afternoon_activities_size);
        Serial.print("evening_activities_size: ");
        Serial.println(evening_activities_size);
    }
    //ui_init();
/*     struct tm _time;
    char chTime[9];
    char chDate[20];
    char chDay[10];
    if (STATUS_TIME_OK) {
        if(getLocalTime(&_time)){
            strftime(chTime, 9, "%H : %M", &_time);
            strftime(chDate, 20, "%d %B %Y", &_time);
            strftime(chDay, 10, "%A", &_time);
        }
    }
    String stTime = chTime;
    String stDate = chDate;
    String stDay = chDay;
    draw_clock(8,18,304,120, stTime);
    draw_date(8,146,304,80, stDate, stDay); */
}

void loop() {
    draw();
    delay(5000);
}