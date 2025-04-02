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
#include <devicemode.h>
#include "Noto.h"
#include "smallFont.h"
#include <PNGdec.h>
#include <PNG_SPIFFS_Support.h>
#include <PSpref.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

// WiFi Manager
WiFiManager wm;

PNG png;

struct tm timeinfo;
ESP32Time rtc(0);

JsonDocument cdoc;

void setTime() {
    Serial.println(" ");
    Serial.print("connecting to time server ");
    Serial.println(" ");
    // Init and get the time
    configTime(0, 0, ntpServer);
    delay(500);
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
    tzset();
    delay(100);
    if(!getLocalTime(&timeinfo)){
        Serial.println("ERROR: failed to obtain time");
        delay(10000);
    } else {
        Serial.println("OK: obtained time");
        Serial.println(&timeinfo, " %A, %B %d %Y %H:%M:%S");
        STATUS_NTP_OK = true;
    }
}

void configModeCallback(WiFiManager *myWiFiManager) {

    Serial.println(" ********************");
    Serial.println(" Entered config mode:");
    Serial.print(" Webportal started at:");
    Serial.println(WiFi.softAPIP());
    Serial.print(" please connect to WiFi SSID: ");
    Serial.println(myWiFiManager->getConfigPortalSSID());
    Serial.println(" to configure WiFI for this device");
    Serial.println(" ********************");

    STATUS_WIFI_MGR_CONFIG_MODE_OK = true;
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
    http.end(); // Free resources
}

void beepBeep() {
  static unsigned long previousMillis = 0;  // Store the previous time
  unsigned long currentMillis = millis();   // Get the current time

  static int state = 0;  // Store the current state
  static int beepDuration = 250;  // Store the beep duration
  if (currentMillis - previousMillis >= beepDuration) {  // If the beep duration has passed
    previousMillis = currentMillis;  // Update the previous time

    state++; if (state >= 10) state = 0;

    if (state == 1 || state == 3)
        if(buzzer) {
            StickCP2.Speaker.tone(6000, 100);
        }
    }
}


void drawSplash() {

    sprite.createSprite(MY_WIDTH, MY_HEIGHT);
    sprite.fillSprite(TFT_WHITE);
    sprite.loadFont(Noto);
    sprite.setTextColor(TFT_DARKGRAY,TFT_WHITE);
    String software = " Pictostick ";
    //software += String("v") + pd_version_major() + "." + pd_version_minor() + "." + pd_version_patch();
    software += String("v") + pd_version_major() + "." + pd_version_minor() + " PROTOTYPE";
    String maker = " Joost Soeterbroek";
    String maker_email = " <joost.soeterbroek@gmail.com>";
    String code = " github.com/jsoeterbroek/pictostick";

    sprite.drawString(software,4,24);
    sprite.unloadFont();
    sprite.loadFont(smallFont);
    sprite.drawString(maker,4,52);
    sprite.drawString(maker_email,4,72);
    sprite.drawString(code,4,92);
    sprite.unloadFont();
    StickCP2.Display.pushImage(0, 0, MY_WIDTH, MY_HEIGHT, (uint16_t*)sprite.getPointer());
}

// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
    uint16_t lineBuffer[MAX_IMAGE_WIDTH];
    png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
    sprite.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void drawMarkedDone() {
    sprite.loadFont(Noto);
    sprite.setTextColor(TFT_BLACK, RIGHT_RECT_BG_COLOR_1);
    sprite.drawString("X", 20, 100);
    sprite.unloadFont();
}

void drawPicto(String _strname) {
    _strname = "/" + _strname;
    int16_t rc = png.open(_strname.c_str(), pngOpen, pngClose, pngRead, pngSeek, pngDraw);
    if (rc == PNG_SUCCESS) {
        sprite.startWrite();
        rc = png.decode(NULL, 0);
        png.close();
        sprite.endWrite();
    }
}

void drawBatt() {
    // battery
    // FIXME: animate battery charging (icon) to indicate charging
    vol = StickCP2.Power.getBatteryVoltage();
    volE=map(vol,3000,4180,0,5);
    String batteryPercentString;
    int batteryPercent = map(vol, 3000, 4200, 0, 100);  // Convert voltage to percentage
    if (batteryPercent > 100) batteryPercent = 100;  // Cap at 100%
    if (batteryPercent < 0) batteryPercent = 0;      // Floor at 0%

    sprite.fillRect(116, 0, 120, 20, RIGHT_RECT_BG_COLOR_2);
    sprite.setTextColor(RIGHT_RECT_TEXT_COLOR_2, RIGHT_RECT_BG_COLOR_2);
    sprite.loadFont(Noto);
    sprite.setCursor(160, 3);  // Adjust cursor for the top right
    sprite.printf("%d%%", batteryPercent);
    sprite.drawRect(200, 3, 28, 14, RGB565_GRAY_STONE); // body
    sprite.fillRect(229, 7, 3, 6, RGB565_GRAY_STONE); // tip
    for(int i=0;i<volE;i++) {
        sprite.fillRect(222-(i*5), 5, 3, 10, RGB565_GRAY_STONE);
    }
    sprite.unloadFont();
}

void drawUserName() {
    // user name
    sprite.loadFont(Noto);
    sprite.fillRect(116, 20, 120, 20, RIGHT_RECT_BG_COLOR_1);
    sprite.setTextColor(RIGHT_RECT_TEXT_COLOR_1, RIGHT_RECT_BG_COLOR_1);
    sprite.drawString(config_name, 118, 24);
    sprite.unloadFont();
}

void drawTime() {
    // time
    static constexpr const char* const wd_en[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    static constexpr const char* const wd_nl[7] = {"Zondag", "Maandag", "Dinsdag", "Woensdag", "Donderdag", "Vrijdag", "Zaterdag"};
    auto dt = StickCP2.Rtc.getDateTime();
    // ESP32 internal timer
    auto t = time(nullptr);
    auto tm = localtime(&t);  // for local timezone.
    char timebuffer[30];
    char daybuffer[30];
    if (lang.equals("en")) {
        snprintf(daybuffer, sizeof(daybuffer), "%s",
            wd_en[tm->tm_wday]);
    } else {
        snprintf(daybuffer, sizeof(daybuffer), "%s",
            wd_nl[tm->tm_wday]);
    }
    snprintf(timebuffer, sizeof(timebuffer), "%02d:%02d",
        tm->tm_hour,
        tm->tm_min);
        //tm->tm_sec);

    sprite.loadFont(Noto);
    sprite.fillRect(116, 40, 120, 20, RIGHT_RECT_BG_COLOR_2);
    sprite.setTextColor(RIGHT_RECT_TEXT_COLOR_2, RIGHT_RECT_BG_COLOR_2);
    sprite.drawString(daybuffer, 118, 44);
    sprite.drawString(timebuffer, 196, 44);
    sprite.unloadFont();
}

void drawName(String _strname, int _marked_done) {
    sprite.loadFont(Noto);
    if (_marked_done == 1) {
        sprite.fillRect(116, 70, 120, 40, COLOR_DONE);
        sprite.setTextColor(RIGHT_RECT_TEXT_COLOR_1, COLOR_DONE);
    } else {
        sprite.fillRect(116, 70, 120, 40, COLOR_TODO);
        sprite.setTextColor(RIGHT_RECT_TEXT_COLOR_1, COLOR_TODO);
    }
    sprite.drawString(_strname, 118, 84);
    sprite.unloadFont();
}

void drawMain() {

    StickCP2.Display.powerSaveOff();
    StickCP2.Display.setBrightness(brightness[b]);
    sprite.createSprite(MY_WIDTH, MY_HEIGHT);
    sprite.fillSprite(BG_COLOR);

    // extract values from config JSON object
    config_activities_size = cdoc["activities"].size();
    config_comment = cdoc["comment"]; // nullptr
    config_version = cdoc["version"]; // "1.0.1"
    config_name = cdoc["name"]; // "Peter"
    config_device_ip = cdoc["device_ip"]; // "128.8.2.123"
    config_date_created = cdoc["date_created"];
    config_date_valid = cdoc["date_valid"];

    int _i = 0;
    // TODO: probably use a fancy multidimensional array with structs for this, but just use 'lists' for now
    String _array_order[config_activities_size];
    String _array_picto[config_activities_size];
    String _array_name[config_activities_size];
    int _array_activity_marked_done[config_activities_size];
    for (JsonObject activity : cdoc["activities"].as<JsonArray>()) {
        _array_order[_i] = String(activity["order"]);
        _array_picto[_i] = String(activity["picto"]);
        if (lang.equals("en")) {
            _array_name[_i] = String(activity["name_en"]);
        } else {
            _array_name[_i] = String(activity["name_nl"]);
        }
        _i = _i + 1;
    }

    ps_current_activity_index = get_pspref_current_activity_index();

    Serial.println("***************");
    Serial.println(" ");
    Serial.print("DEBUG: current activity index: ");
    Serial.println(ps_current_activity_index);
    Serial.println(" ");
    Serial.print("DEBUG: this activity is: ");
    if (get_pspref_activity_done(ps_current_activity_index) == 1) {
        Serial.println("done");
    } else {
        Serial.println("todo");
    }
    Serial.println("***************");

    sprite.unloadFont();
    sprite.setTextColor(TFT_WHITE, RIGHT_RECT_BG_COLOR_1);

    sprite.fillSmoothRoundRect(10, 10, picto_box_width, picto_box_height, 5, FG_COLOR,BG_COLOR);

    drawTime();

    drawUserName();

    drawBatt();

    // by default, if there is no current activity, the first one will be current 
    for (int i = 0; i < config_activities_size; i++ ) {
    
        // current
        if (i == ps_current_activity_index) {
            
            // draw the picto
            drawPicto(_array_picto[i]);

            // draw the name of the activity
            if (get_pspref_activity_done(ps_current_activity_index) == 1) {
                drawName(_array_name[i], 1);
            } else {
                drawName(_array_name[i], 0);
            }

            // now check if this activity is marked done in  
            // _array_activity_marked_done[current_activity_index]
            if (get_pspref_activity_done(ps_current_activity_index) == 1) {
                Serial.print(ps_current_activity_index); // FIXME: remove later
                Serial.println(" is marked done"); // FIXME: remove later
                //drawMarkedDone();
            }
        }
    }

    // TEST, uncomment below
    // maximum activities_size = 18;
    // config_activities_size = 5;

    int _circle_x; int _dist_between; int _size_circle;
    switch (config_activities_size) {
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

    sprite.fillRect(0, 128, 240, 5, BG_COLOR);

    if (config_activities_size < config_activities_size_max) {
        for (int i = 0; i < config_activities_size; i++) {
            // small indicator (rectangle) for the current activity
            if (i == ps_current_activity_index) {
                sprite.fillRect(_circle_x - 8, 129, 16, 4, DAYPERIOD_CIRCLE_BG_COLOR);
            }
            if (get_pspref_activity_done(i) == 1) {
                sprite.fillSmoothCircle(_circle_x, 122, _size_circle, COLOR_DONE, BG_COLOR);
            } else {
                sprite.fillSmoothCircle(_circle_x, 122, _size_circle, COLOR_TODO, BG_COLOR);
            }
            _circle_x = _circle_x + _dist_between;
        }
    }

    sprite.unloadFont();
    StickCP2.Display.pushImage(0, 0, MY_WIDTH, MY_HEIGHT, (uint16_t*)sprite.getPointer());

    // button action
    if (StickCP2.BtnPWR.wasPressed()) {
        sleepTime=25;
        ps_current_activity_index = get_pspref_current_activity_index();
        if (ps_current_activity_index >= 1) {
            set_pspref_current_activity_index(ps_current_activity_index - 1);
        } else {
            if(buzzer) {
                StickCP2.Speaker.tone(6000, 100);
            }
        }
    }
    if (StickCP2.BtnB.wasPressed()) {
        sleepTime=25;
        ps_current_activity_index = get_pspref_current_activity_index();
        if (ps_current_activity_index < config_activities_size - 1) {
            set_pspref_current_activity_index(ps_current_activity_index + 1);
        } else {
            if(buzzer) {
                StickCP2.Speaker.tone(6000, 100);
            }
        }
    }
    if (StickCP2.BtnA.wasPressed()) {

        sleepTime=25;
        // mark activity done
        // if it was turned off, turn on
        // if it was turned on, turn off
        // since we have only one button available
        if (get_pspref_activity_done(ps_current_activity_index) == 1) {
            set_pspref_activity_undone(ps_current_activity_index);
        } else {
            set_pspref_activity_done(ps_current_activity_index);
        }

        //Serial.print("get_pspref_activity_done(ps_current_activity_index) is "); // FIXME: remove later
        // Serial.println(get_pspref_activity_done(ps_current_activity_index)); // FIXME: remove later
        //delay(8000); // FIXME: remove later

        //beepBeep();
    }
}

void setup() {

    pinMode(35,INPUT_PULLUP);
    auto cfg = M5.config();
    StickCP2.begin(cfg);
    StickCP2.Display.setRotation(3);
    StickCP2.Display.fillScreen(TFT_WHITE);

    sprite.createSprite(MY_WIDTH, MY_HEIGHT);

    StickCP2.Display.setBrightness(brightness[b]);

    Serial.begin(115200);
    Serial.println("start initialisation..");

    dmPrefs.begin(NS, RO_MODE);  // Open our namespace (or create it
                                 //  if it doesn't exist) in RO mode.

    bool dmInit = dmPrefs.isKey("devicemode"); // Test for the existence
                                               // of the "already initialized" key.

    if (dmInit == false) {
        // If tpInit is 'false', the key "nvsInit" does not yet exist therefore this
        //  must be our first-time run. We need to set up our Preferences namespace keys. So...
        dmPrefs.end();                     // close the namespace in RO mode and...
        dmPrefs.begin(NS, RW_MODE);        //  reopen it in RW mode.

        // set devicemode to '1'
        dmPrefs.putInt("devicemode", 1);
        // The "factory defaults" are created and stored so...
        dmPrefs.end();                  // Close the namespace in RW mode and...
        dmPrefs.begin(NS, RO_MODE);     //  reopen it in RO mode
    }

    uint8_t _mode = 0;
    Serial.print(" * Starting in mode:");
    _mode = get_devicemode();
    Serial.print(_mode);

    // FIXME: fix or remov3?? 
    switch(_mode) {
        case 0:
            Serial.println("mode 0");
            Serial.println("rebooting..");
            delay(10000);
            ESP.restart(); // trigger reboot of device
            break;
        case 1:            // 1. network config mode
        
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
                Serial.println("WiFi connected.");
            }
            
            // set NTP time
            setTime();

            // set NTP time to rtc clock
            struct tm timeInfo;
            if (STATUS_NTP_OK) {
                Serial.println("set rtc clock from NTP");
                while (!getLocalTime(&timeInfo, 1000)) {
                    Serial.print('.');
                };
                time_t t = time(nullptr) + 1;  // Advance one second.
                while (t > time(nullptr))
                    ;  /// Synchronization in seconds
                StickCP2.Rtc.setDateTime(gmtime(&t));
            } else {
                Serial.println("ERROR: rtc clock not set");
            }
            if (STATUS_NTP_OK) {
                // set devicemode 3
                set_devicemode(3);
                // set devicemode_1_flag = true to mark this mode succesfull completed
                set_devicemode_1_flag(true);
            }
            break;
        case 2:  // 2. config mode
            break;
        case 3:  // 3. regular mode

            // get config data
            // FIXME: add logic to compare config file on fs with possible new update from HTTP in device mode 2
            if(GET_CONFIG_DATA_SPIFF) {
                getConfigDataSPIFF();
            } else {
                getConfigDataHTTP();
            }

            // FIXME: make check for md5sum checksum of config file
            if (STATUS_GET_CONFIG_DATA_SPIFF_OK) {
                STATUS_CONFIG_DATA_OK = true;
                Serial.println("config successfully read from fs");
            } else {
                Serial.println("ERROR: error reading config from fs");
            }
            Serial.println("initialisation complete");
            drawSplash();
            delay(10000);
            break;
    }
}

void loop() {

    StickCP2.update();
    if(slp) {
        StickCP2.Display.setBrightness(brightness[b]);
        slp = false;
        sleepTime = 25;
    }
    drawMain();
    delay(100);
    auto dt = StickCP2.Rtc.getDateTime();
    if(dt.time.seconds<10) s="0"+String(dt.time.seconds); else s=String(dt.time.seconds);
    if(dt.time.minutes<10) m="0"+String(dt.time.minutes); else m=String(dt.time.minutes);
    ts=dt.time.seconds;
    if(tts!=ts) {sleepTime--; tts=ts;}

    Serial.print("DEBUG: sleepTime: "); // FIXME: remove later
    Serial.println(sleepTime); // FIXME: remove later

    if(sleepTime == 0) {
        slp = true;
        StickCP2.Display.setBrightness(0);
        delay(20);
        StickCP2.Power.lightSleep();
    }
}
