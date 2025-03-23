#include <Arduino.h>
#include <common.h>
#include <WiFiManager.h>
#include <SPI.h>
#include <time.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <ESP32Time.h>

struct tm timeinfo;
ESP32Time rtc(0);

//scroling message on bottom right side
//String Wmsg = "";

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

void getConfigDataSPIFF () {

    STATUS_GET_CONFIG_DATA_SPIFF_OK = false;
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
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payloadStr);

        if (!error) {
            STATUS_CONFIG_DATA_OK = true;
            pData[0] = doc["periods"]["morning"];
            pData[1] = doc["periods"]["afternoon"];
            pData[2] = doc["periods"]["evening"];
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

    Serial.println(" initialisation complete");
    delay(8000);
}

void draw() {

    Serial.println("draw");
    if (STATUS_CONFIG_DATA_OK) {
        Serial.println(pData[0]);
    }
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