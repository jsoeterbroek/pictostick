#include <Arduino.h>
#include <common.h>
#include <WiFiManager.h>
#include <SPI.h>
#include <time.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <ESP32Time.h>
#include <lvgl.h>
#include <ui.h>
#include "FS.h"
#include "SPIFFS.h"

struct tm timeinfo;
ESP32Time rtc(0);

//scroling message on bottom right side
//String Wmsg = "";

JsonDocument cdoc;
float pData[4];

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

void testFileIO(fs::FS &fs, const char * path){
    Serial.printf("Testing file I/O with %s\r\n", path);
    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++){
        if ((i & 0x001F) == 0x001F){
          Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();
    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        Serial.println("- failed to open file for reading");
    }
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
    Serial.printf("Reading file: %s\r\n", path);
    File cfile = fs.open(path);
    if(!cfile || cfile.isDirectory()){
        Serial.println("ERROR: failed to open config file for reading");
    } else {
        DeserializationError error = deserializeJson(cdoc, cfile);
        if (!error) {
            Serial.println("deserializeJson() OK");
            STATUS_GET_CONFIG_DATA_SPIFF_OK = true;
        } else {
            Serial.print("ERROR: deserializeJson() returned ");
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
    if(GET_CONFIG_DATA_SPIFF) {
        getConfigDataSPIFF();
    } else {
        getConfigDataHTTP();
    }
    pData[0] = cdoc["periods"]["morning"];
    pData[1] = cdoc["periods"]["afternoon"];
    pData[2] = cdoc["periods"]["evening"];

    if (STATUS_GET_CONFIG_DATA_SPIFF_OK) {
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
        Serial.println(pData[0]);
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
    Serial.println("loop");
    draw();
    delay(5000);
}