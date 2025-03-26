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
        STATUS_NTP_OK = true;
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
    sprite.fillSmoothRoundRect(-38, 6, middle_box_width, middle_box_height, 5, FG_COLOR, BG_COLOR);
    // middle now
    sprite.fillSmoothRoundRect(MY_WIDTH / 2 - 50, 6 , middle_box_width, middle_box_height, 5, FG_COLOR, BG_COLOR);
    // middle next
    sprite.fillSmoothRoundRect(178, 6, middle_box_width, middle_box_height, 5, FG_COLOR, BG_COLOR);

    // bottom
    // each activity gets its own circle 
    // we should maximize the number of activities to screen length (max 14?)

    // TEST, uncomment below
    // maximum activities_size = 18;
    //activities_size = 18;

    int _circle_x; int _dist_between; int _size_circle;
    int activities_size_max = 19;
    switch (activities_size) {
    case 1:
        _circle_x = 120; _dist_between = 46; _size_circle = 6; break;
    case 2:
        _circle_x = 104; _dist_between = 43; _size_circle = 6; break;
    case 3:
        _circle_x = 82; _dist_between = 40; _size_circle = 6; break;
    case 4:
        _circle_x = 68; _dist_between = 37; _size_circle = 6; break;
    case 5:
        _circle_x = 56; _dist_between = 34; _size_circle = 6; break;
    case 6:
        _circle_x = 45; _dist_between = 31; _size_circle = 6; break;
    case 7:
        _circle_x = 37; _dist_between = 28; _size_circle = 5; break;
    case 8:
        _circle_x = 30; _dist_between = 26; _size_circle = 5; break;
    case 9:
        _circle_x = 25; _dist_between = 24; _size_circle = 5; break;
    case 10:
        _circle_x = 23; _dist_between = 22; _size_circle = 5; break;
    case 11:
        _circle_x = 20; _dist_between = 20; _size_circle = 5; break;
    case 12:
        _circle_x = 18; _dist_between = 18; _size_circle = 4; break;
    case 13:
        _circle_x = 17; _dist_between = 17; _size_circle = 4; break;
    case 14:
        _circle_x = 16; _dist_between = 16; _size_circle = 4; break;
    case 15:
        _circle_x = 15; _dist_between = 15; _size_circle = 4; break;
    case 16:
        _circle_x = 14; _dist_between = 14; _size_circle = 4; break;
    case 17:
        _circle_x = 13; _dist_between = 13; _size_circle = 3; break;
    case 18:
        _circle_x = 13; _dist_between = 12; _size_circle = 3; break;
    default:
        _circle_x = 14; _dist_between = 16; _size_circle = 6; break;
    }

    if (activities_size < activities_size_max) {
        for(int i = 0; i < activities_size; i++){
            sprite.fillSmoothCircle(_circle_x, 122, _size_circle, DAYPERIOD_CIRCLE_BG_COLOR, BG_COLOR);
            _circle_x = _circle_x + _dist_between;
        }
    }

    //sprite.setTextDatum(0);

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

    // set NTP time to rtc clock
    struct tm timeInfo;
    if (STATUS_NTP_OK) {
        tft.println("set rtc clock from NTP");
        while (!getLocalTime(&timeInfo, 1000)) {
            Serial.print('.');
        };
        time_t t = time(nullptr) + 1;  // Advance one second.
        while (t > time(nullptr))
            ;  /// Synchronization in seconds
        StickCP2.Rtc.setDateTime(gmtime(&t));
    } else {
        tft.println("ERROR: rtc clock not set");
    }

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

    // for (JsonObject activity : cdoc["activities"].as<JsonArray>()) {

    //     const char* activity_order = activity["order"]; // "001", "002", "003", "004", "005", "006", "007", ...
    //     const char* activity_picto = activity["picto"]; // "alarm.png", "shower.png", "self_care.png", ...
    //     const char* activity_name_en = activity["name_en"]; // "waking", "shower", "brushing teeth", ...
    //     const char* activity_name_nl = activity["name_nl"]; // "opstaan", "douchen", "tanden poetsen", ...
      
    // }

    const char* date_created = cdoc["date_created"]; // "24-03-2025"
    const char* date_valid = cdoc["date_valid"]; // "23-03-2025"
    activities_size = cdoc["activities"].size();

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

    static constexpr const char* const wd[7] = {"Zondag", "Maandag", "Dinsdag", "Woensdag", "Donderdag", "Vrijdag", "Zaterdag"};

    auto dt = StickCP2.Rtc.getDateTime();
    //Serial.printf("RTC   UTC    :%04d/%02d/%02d (%s)  %02d:%02d:%02d\r\n",
    //    dt.date.year, dt.date.month, dt.date.date,
    //    wd[dt.date.weekDay], dt.time.hours, dt.time.minutes,
    //    dt.time.seconds);

    /// ESP32 internal timer
    auto t = time(nullptr);
    auto tm = localtime(&t);  // for local timezone.
    Serial.printf("ESP32 %s  :%04d/%02d/%02d (%s)  %02d:%02d:%02d\r\n",
        NTP_TIMEZONE, tm->tm_year + 1900, tm->tm_mon + 1,
        tm->tm_mday, wd[tm->tm_wday], tm->tm_hour, tm->tm_min,
        tm->tm_sec);

    //drawTime(); // FIXME time screen
    //drawBattery(); // FIXME battery screen
    //drawWifi(); // FIXME wifi screen
    draw();
    delay(1000);
}