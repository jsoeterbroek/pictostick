#include <Arduino.h>
#include <M5StickCPlus2.h>
#include <common.h>
#include <WiFiManager.h>
#include <SPI.h>
#include <time.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <FS.h>
#include <SPIFFS.h>
using namespace fs;
#include <devicemode.h>
#include "NotoSansBold15.h"
#include "bigFont.h"
#include "secFont.h"
#include "smallFont.h"
#include <PNGdec.h>
#include <PNG_SPIFFS_Support.h>
#include <PSpref.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <StreamString.h>

#include <AsyncJson.h>
#include <AsyncMessagePack.h>
#include <_locale.h>
#include "init.h"
#include "uimain.h"
#include "uisetup.h"
#include "uiconfig.h"
#include "ui_helpers.h"
#include "status.h"

// Global variable definitions
AppStatus app_status;

boolean GET_CONFIG_DATA_HTTP = false;
boolean GET_CONFIG_DATA_SPIFF = true;
const char *ntpServer = "europe.pool.ntp.org";
String timezone = "CET-1CEST,M3.5.0,M10.5.0/3";
const char *wifi_mngr_networkname = "pictostick";
const char *wifi_mngr_password = "password";
String serverName = "http://192.168.178.148:8001";
const char *cfilename = "/data.json";
const char *config_comment;
const char *config_version;
const char *config_name;
const char *config_device_ip;
const char *config_date_created;
const char *config_date_valid;
int8_t config_activities_size = 0;
int config_activities_size_max = 19;
int16_t picto_box_width = 100;
int16_t picto_box_height = 100;
String h, m, s;
int day, month;
int8_t hour = 0;
int8_t minute = 0;
int8_t second = 0;
int vol;
int volE;
int sleepTime = 60;
int ts, tts = 0;
bool slp = false;
int16_t xpos = 12;
int16_t ypos = 12;
int current_activity_index = 0;
int ps_current_activity_index = 0;
String lang = "en";
bool marked_done = false;
int cursor = 0;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

// WiFi Manager
WiFiManager wm;

// AsyncWebserver
static AsyncWebServer server(80);

PNG png;

JsonDocument cdoc;

struct tm timeinfo;

// Forward declarations for functions used in init.cpp
void getConfigDataSPIFF();

// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  sprite.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void setTimezone(String timezone) {
  Serial.printf("  Setting Timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(),
         1);  //  Now adjust the TZ.  Clock settings are adjusted to show the
              //  new local time
  tzset();
}

void initTime(String timezone) {
  Serial.println(" ");
  Serial.print("connecting to time server ");
  Serial.println(" ");
  // Init and get the time
  configTime(0, 0, ntpServer);
  delay(500);
  setTimezone(timezone);
  delay(100);
  if (!getLocalTime(&timeinfo)) {
    Serial.println("ERROR: failed to obtain time");
    delay(10000);
  } else {
    Serial.println("OK: obtained time");
    Serial.println(&timeinfo, " %A, %B %d %Y %H:%M:%S");
    app_status.ntp_ok = true;
  }
}

void configModeCallback(WiFiManager *myWiFiManager) {
  StickCP2.Display.drawString("******************", 4, 28);
  StickCP2.Display.drawString(TXT_WM_WEBPORTAL_STARTED, 4, 40);
  StickCP2.Display.drawString(WiFi.softAPIP().toString(), 4, 52);
  StickCP2.Display.drawString(TXT_WM_CONNECT_SSID, 4, 64);
  StickCP2.Display.drawString(myWiFiManager->getConfigPortalSSID(), 4, 76);
  StickCP2.Display.drawString(TXT_WM_CONFIG_DEVICE, 4, 88);
  StickCP2.Display.drawString("******************", 4, 100);
  app_status.wifi_mgr_config_mode_ok = true;
}

void deleteConfigFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

void writeConfigFile(fs::FS &fs, const char *path, JsonObject _json) {
  app_status.set_config_data_spiff_ok = false;

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  Serial.printf("");                                   // FIXME, remove later
  Serial.printf("Writing config file: %s\r\n", path);  // FIXME, remove later

  // Open file for writing
  File myfile = fs.open(path, FILE_WRITE);
  if (!myfile) {
    Serial.println("ERROR: failed to open config file for writing");
  } else {
    // serializeJson(_json, Serial);
    if (serializeJson(_json, myfile) == 0) {
      Serial.print(F("Failed write to file "));
      Serial.println(F(path));
      app_status.set_config_data_spiff_ok = false;
    } else {
      Serial.print(F("Success write to file "));
      Serial.println(F(path));
      app_status.set_config_data_spiff_ok = true;
    }
  }
  // Close the file
  myfile.close();
}

void readConfigFile(fs::FS &fs, const char *path) {
  app_status.get_config_data_spiff_ok = false;

  static uint8_t buf[512];
  size_t len = 0;
  Serial.printf("Reading config file: %s\r\n", path);
  File cfile = fs.open(path);
  if (!cfile || cfile.isDirectory()) {
    Serial.println("ERROR: failed to open config file for reading");
  } else {
    DeserializationError error = deserializeJson(cdoc, cfile);
    if (!error) {
      Serial.println("deserializeJson OK");
      app_status.get_config_data_spiff_ok = true;
    } else {
      Serial.print("ERROR: deserializeJson returned ");
      Serial.println(error.c_str());
    }
  }
  cfile.close();
}

void writeConfigDataSPIFF(JsonObject _json) {
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  writeConfigFile(SPIFFS, cfilename, _json);
}

void getConfigDataSPIFF() {
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  readConfigFile(SPIFFS, cfilename);
}

void beepOrNot() {
  if (get_pspref_buzzer()) {
    StickCP2.Speaker.tone(6000, 100);
  }
}

// processor placeholder
String processor(const String &var) {
  String foo = "bar";
  return foo;
}

bool initWiFi() {
  WiFi.mode(WIFI_STA);

  Serial.print("Connecting to WiFi network... ");
  Serial.println(wm.getWiFiSSID());
  WiFi.begin(wm.getWiFiSSID(), wm.getWiFiPass());

  unsigned long currentMillis = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - currentMillis) <= 5000) {
    Serial.print(".");
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected to the WiFi network");
    Serial.println("restarting in device mode 1");
    set_devicemode(1);
    ESP.restart();
  } else {
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    String localip = WiFi.localIP().toString();
    StickCP2.Display.drawString(localip, 120, 16);
  }

  return true;
}

void init_ESPAsync_Ws() {
  app_status.config_data_ok = false;
  app_status.set_config_data_spiff_ok = false;

  // mount SPIFFS
  Serial.println("Mounting SPIFFS...");
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  if (!SPIFFS.exists("/index.html")) {
    Serial.println("ERROR: index.html not found in SPIFFS");
    Serial.println("Please upload the web files to SPIFFS");
    return;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
  });

  // Route to get list of available pictos
  server.on("/get-pictos", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    JsonDocument doc;
    JsonArray pictos = doc.to<JsonArray>();

    File root = SPIFFS.open("/picto");
    if (!root || !root.isDirectory()) {
      response->print("[]");
      request->send(response);
      return;
    }

    File file = root.openNextFile();
    while (file) {
      if (!file.isDirectory()) {
        String filename = file.name();
        if (filename.endsWith(".png")) {
          // Remove path prefix if present
          int lastSlash = filename.lastIndexOf('/');
          if (lastSlash >= 0) {
            filename = filename.substring(lastSlash + 1);
          }
          // Remove .png extension for the name
          String pictoName = filename.substring(0, filename.length() - 4);

          JsonObject picto = pictos.add<JsonObject>();
          picto["filename"] = filename;
          picto["name"] = pictoName;
        }
      }
      file = root.openNextFile();
    }
    root.close();

    serializeJson(doc, *response);
    request->send(response);
  });

  // Route to handle JSON configuration save
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/save-config", [](AsyncWebServerRequest *request, JsonVariant &json) {
    Serial.println("Received JSON configuration");

    // Write the JSON to data.json file
    File file = SPIFFS.open("/data.json", FILE_WRITE);
    if (!file) {
      Serial.println("Failed to open data.json for writing");
      request->send(500, "text/plain", "Failed to save configuration 1");
      return;
    } else {
      Serial.println("Success opened data.json for writing");
    }

    // Write JSON to file
    //serializeJson(json, file);
    if (serializeJson(json, file) == 0) {
      Serial.print(F("Failed write to file "));
      request->send(500, "text/plain", "Failed to save configuration 2");
      app_status.set_config_data_spiff_ok = false;
    } else {
      Serial.println("Configuration saved to data.json");
      request->send(200, "text/plain", "Configuration saved successfully");
      // Update the global config document
      app_status.config_data_ok = true;
      app_status.set_config_data_spiff_ok = true;
      //cdoc.clear();
      //cdoc = json;
    }
    file.close();
  });
  server.addHandler(handler);

  server.serveStatic("/", SPIFFS, "/");

  // Route to get current configuration
  server.on("/get-config", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");

    File file = SPIFFS.open("/data.json", FILE_READ);
    if (file) {
      String content = file.readString();
      file.close();
      response->print(content);
    } else {
      response->print("{}");
    }

    request->send(response);
  });

  server.begin();
}

void drawDeviceMode1() {
  Serial.println("DEBUG: drawDeviceMode1 active");  // FIXME, remove later
  // set_devicemode(3);
  StickCP2.Display.clear();
  StickCP2.Display.drawString(TXT_DM1, 4, 4);
  StickCP2.Display.drawString(TXT_WM_START, 4, 16);

  // WiFiManager
  wm.setDebugOutput(true);
  // wm.setShowInfoErase(false);
  wm.resetSettings();  // for testing only, remove later
  wm.setConfigPortalTimeout(5000);
  wm.setAPCallback(configModeCallback);

  if (!wm.startConfigPortal("PictoStick")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
  } else {
    app_status.wifi_mgr_ok = true;
    Serial.println("WiFi connected.");
  }

  // set NTP time
  initTime(timezone);

  // set NTP time to rtc clock
  if (app_status.ntp_ok) {
    Serial.println("set rtc clock from NTP");
    while (!getLocalTime(&timeinfo, 1000)) {
      Serial.print('.');
    };
    time_t t = time(nullptr) + 1;  // Advance one second.
    while (t > time(nullptr));     /// Synchronization in seconds
    StickCP2.Rtc.setDateTime(gmtime(&t));
  } else {
    Serial.println("ERROR: rtc clock not set");
  }
  if (app_status.ntp_ok) {
    // set devicemode 3
    set_devicemode(3);
  }
}

void drawDeviceMode2() {
  Serial.println("DEBUG: drawDeviceMode2 active");  // FIXME, remove later
  StickCP2.Display.clear();
  StickCP2.Display.drawString(TXT_DM2, 4, 4);
  StickCP2.Display.drawString(TXT_DM2_WIFI_START, 4, 16);

  if (initWiFi()) {
    init_ESPAsync_Ws();
  } else {
    // set devicemode 1
    StickCP2.Display.drawString(TXT_DM_SET_1, 4, 88);
    delay(2000);
    set_devicemode(1);
  }
  StickCP2.Display.drawString(TXT_DM2_WS_START, 4, 28);

  while (!app_status.set_config_data_spiff_ok) {
    Serial.print(".");
    delay(1000);
  }

  if (app_status.set_config_data_spiff_ok) {
    Serial.println("DEBUG: drawDeviceMode2 active; set config data spiff OK");  // FIXME, remove later
    StickCP2.Display.drawString(TXT_DM2_FILE_OK, 4, 64);
  } else {
    Serial.println("DEBUG: drawDeviceMode2 active; set config data spiff ERROR");  // FIXME, remove later
    StickCP2.Display.drawString(TXT_DM2_FILE_ERR, 4, 64);
  }

  // shutdown wifi
  StickCP2.Display.drawString(TXT_DM2_WIFI_DISC, 4, 76);
  delay(2000);
  WiFi.disconnect(true);

  if (app_status.set_config_data_spiff_ok) {
    // set devicemode 4
    StickCP2.Display.drawString(TXT_DM_SET_4, 4, 88);
    delay(2000);
    set_devicemode(4);
  } else {
    // set devicemode 2
    StickCP2.Display.drawString(TXT_DM_SET_2, 4, 88);
    delay(2000);
    set_devicemode(2);
  }

  // restart
  StickCP2.Display.drawString(TXT_DM_RESTART, 4, 100);
  delay(2000);
  ESP.restart();
}

void setup() {
  init_device();
  init_preferences();
  init_filesystem();
  init_devicemode();
}

void loop() {
  StickCP2.update();
  if (slp) {
    StickCP2.Display.setBrightness(get_pspref_brightness());
    slp = false;
    sleepTime = 25;
  }
  if (draw_device_mode_config) {
    drawDeviceModeConfig(desired_devicemode);
  } else if (draw_device_mode_1) {
    drawDeviceMode1();
  } else if (draw_device_mode_2) {
    drawDeviceMode2();
  } else if (draw_device_mode_3) {
    drawDeviceMode3();
  } else {
    drawMain();
    delay(100);
    auto dt = StickCP2.Rtc.getDateTime();
    if (dt.time.seconds < 10) {
      s = "0" + String(dt.time.seconds);
    } else {
      s = String(dt.time.seconds);
    }
    if (dt.time.minutes < 10) {
      m = "0" + String(dt.time.minutes);
    } else {
      m = String(dt.time.minutes);
    }
    ts = dt.time.seconds;
    if (tts != ts) {
      sleepTime--;
      tts = ts;
    }

    if (sleepTime == 0) {
      slp = true;
      StickCP2.Display.setBrightness(0);
      delay(20);
      StickCP2.Power.lightSleep();
    }
  }
}
