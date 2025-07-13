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
#include "system.h"
#include "fs_helpers.h"

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
AsyncWebServer server(80);

PNG png;

JsonDocument cdoc;

struct tm timeinfo;

// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  sprite.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void beepOrNot() {
  if (get_pspref_buzzer()) {
    StickCP2.Speaker.tone(6000, 100);
  }
}

String processor(const String &var) {
  String foo = "bar";
  return foo;
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
