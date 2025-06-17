#include <Arduino.h>
#include <LilyGoLib.h>
#include <WiFi.h>
#include <FS.h>
#include <LittleFS.h>
#include "ui.h"
#include "global_flags.h"
#include "devicemode.h"
#include "PSpref.h"
#include "common.h"
#include "NotoSansBold15.h"

#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncMessagePack.h>

JsonDocument cdoc;

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;  // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = 120, osy = 120, omx = 120, omy = 120, ohx = 120, ohy = 120;  // Saved H, M, S x & y coords
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0;                                                              // for next 1 second timeout
static uint8_t conv2d(const char *p);                                                 // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6);  // Get H, M, S from compile time

bool initial = 1;
#define FORMAT_LITTLEFS_IF_FAILED true

void writeConfigFile(fs::FS &fs, const char *path, JsonObject _json) {
  STATUS_SET_CONFIG_DATA_FS_OK = false;

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
      STATUS_SET_CONFIG_DATA_FS_OK = false;
    } else {
      Serial.print(F("Success write to file "));
      Serial.println(F(path));
      STATUS_SET_CONFIG_DATA_FS_OK = true;
    }
  }
  // Close the file
  myfile.close();
}

void readConfigFile(fs::FS &fs, const char *path) {
  STATUS_GET_CONFIG_DATA_FS_OK = false;

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
      STATUS_GET_CONFIG_DATA_FS_OK = true;
    } else {
      Serial.print("ERROR: deserializeJson returned ");
      Serial.println(error.c_str());
    }
  }
  cfile.close();
}

void getConfigData() {
  readConfigFile(LittleFS, cfilename);
}

//void drawUserName(const char *_config_name) {
//  // user name
//  watch.fillRect(116, 20, 120, 20, RIGHT_RECT_BG_COLOR_1);
//  watch.setTextColor(RIGHT_RECT_TEXT_COLOR_1, RIGHT_RECT_BG_COLOR_1);
//  watch.drawString(_config_name, 118, 24);
//}

void drawSplash() {
  String software = " PictoWatch ";
  // software += String("v") + pd_version_major() + "." +
  // pd_version_minor() +
  // "." + pd_version_patch();
  //   software += String("v") + pd_version_major() + "." + pd_version_minor() + " PROTOTYPE";
  String maker = " Joost Soeterbroek";
  String maker_email = " <joost.soeterbroek@gmail.com>";
  String code = " github.com/jsoeterbroek/pictostick";

  watch.fillScreen(RGB565_BLACK_OUTER_SPACE);
  watch.setTextSize(2);
  watch.setTextColor(RGB565_GRAY_LIGHT);
  watch.setCursor(35, 5);
  watch.println(F("PictoWatch"));
  watch.setCursor(35, 35);
  watch.println(F("Version 1.0.0"));
  watch.setCursor(35, 65);
  watch.println(F("Prototype"));
  delay(10000);
}

void drawMain() {

  // extract values from config JSON object
  config_activities_size = cdoc["activities"].size();
  config_name = cdoc["name"];  // "Peter"

  int _i = 0;
  String _array_order[config_activities_size];
  String _array_picto[config_activities_size];
  String _array_desc[config_activities_size];
  int _array_activity_marked_done[config_activities_size];
  for (JsonObject activity : cdoc["activities"].as<JsonArray>()) {
    _array_order[_i] = String(activity["order"].as<int>());
    _array_picto[_i] = String(activity["picto"].as<String>());
    _array_desc[_i] = String(activity["description"].as<String>());
    _i = _i + 1;
  }

  ps_current_activity_index = get_pspref_current_activity_index();

  //Serial.println("***************");
  // Serial.println(" ");
  // Serial.print("DEBUG: current activity index: ");
  // Serial.println(ps_current_activity_index);
  // Serial.println(" ");
  // Serial.print("DEBUG: this activity is: ");
  // if (get_pspref_activity_done(ps_current_activity_index) == 1) {
  //   Serial.println("done");
  // } else {
  //   Serial.println("todo");
  // }
  Serial.print(config_name);
  Serial.println("***************");

  watch.fillSmoothRoundRect(50, 30, picto_box_width, picto_box_height, 5, FG_COLOR, BG_COLOR);

  //drawUserName(config_name);
  watch.fillRect(0, 0, 120, 20, RIGHT_RECT_BG_COLOR_1);
  watch.setTextColor(RIGHT_RECT_TEXT_COLOR_1, RIGHT_RECT_BG_COLOR_1);
  //watch.drawString(config_name, 118, 24);
}

void setup() {
  Serial.begin(115200);
  Serial.println("start initialization..");

  dmPrefs.begin(NS, RO_MODE);  // Open our namespace (or create it
                               //  if it doesn't exist) in RO mode.

  bool dmInit = dmPrefs.isKey("devicemode");  // Test for the existence
                                              // of the "already initialized" key.

  if (dmInit == false) {
    dmPrefs.end();               // close the namespace in RO mode and...
    dmPrefs.begin(NS, RW_MODE);  //  reopen it in RW mode.

    // set devicemode to '1'
    dmPrefs.putInt("devicemode", 1);
    // The "factory defaults" are created and stored so...
    dmPrefs.end();               // Close the namespace in RW mode and...
    dmPrefs.begin(NS, RO_MODE);  //  reopen it in RO mode
  }

  Serial.print(" * Starting in mode:");
  devicemode = get_devicemode();
  Serial.println(devicemode);

  watch.begin();
  watch.setRotation(2);

  watch.fillScreen(RGB565_BLACK_OUTER_SPACE);
  drawSplash();
  watch.fillScreen(RGB565_BLACK_OUTER_SPACE);

  switch (devicemode) {
    case 0:
      Serial.println("mode 0");
      // Serial.println("rebooting..");
      // delay(10000);
      // ESP.restart();  // trigger reboot of device
      break;
    case 1:  // 1. network config mode
      Serial.println("mode 1");
      draw_device_mode_1 = true;
      break;
    case 2:  // 2. config mode
      Serial.println("mode 2");
      draw_device_mode_2 = true;
      break;
    case 3:  // 3. config mode
      Serial.println("mode 3");
      draw_device_mode_3 = true;
      break;
    case 4:  // 4. regular mode
      // get config data
      if (GET_CONFIG_DATA_SPIFF) {
        getConfigData();
      }
      // FIXME: make check for md5sum checksum of config file
      if (STATUS_GET_CONFIG_DATA_FS_OK) {
        STATUS_CONFIG_DATA_OK = true;
        Serial.println("config successfully read from fs");
      } else {
        Serial.println("ERROR: error reading config from fs");
      }
      Serial.println("initialization complete");
      drawSplash();
      break;
  }
}

void loop() {

  set_devicemode(4);  // FIXME: set devicemode to 1, for now, remove later

  drawMain();
}

static uint8_t conv2d(const char *p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9') {
    v = *p - '0';
  }
  return 10 * v + *++p - '0';
}
