#include <Arduino.h>
#include <LilyGoLib.h>
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include "ui.h"
#include "global_flags.h"
#include "devicemode.h"
#include "PSpref.h"
#include "common.h"
#include <PNGdec.h>
#include <PNG_SPIFFS_Support.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncMessagePack.h>

JsonDocument cdoc;
PNG png;

#define GFXFF 1
#define FF18  &FreeSans12pt7b
// Custom are fonts added to library "TFT_eSPI\Fonts\Custom" folder
// a #include must also be added to the "User_Custom_Fonts.h" file
// in the "TFT_eSPI\User_Setups" folder. See example entries.
#define CF_OL24 &Orbitron_Light_24
#define CF_OL32 &Orbitron_Light_32
#define CF_RT24 &Roboto_Thin_24
#define CF_S24  &Satisfy_24
#define CF_Y32  &Yellowtail_32

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
  File myfile = fs.open(path, "w");
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
  File cfile = fs.open(path, "r");
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
  delay(100);    // give some time to the file system to process the read
  delay(10000);  // FIXME: remove later, just for testing
  cfile.close();
}

void getConfigData() {
  readConfigFile(SPIFFS, cfilename);
}

// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  watch.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void drawPicto(String _strname) {
  _strname = "/picto/" + _strname;
  int16_t rc = png.open(_strname.c_str(), pngOpen, pngClose, pngRead, pngSeek, pngDraw);
  if (rc == PNG_SUCCESS) {
    watch.startWrite();
    rc = png.decode(NULL, 0);
    png.close();
    watch.endWrite();
  }
}

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
  watch.setTextDatum(ML_DATUM);
  watch.setFreeFont(CF_OL24);
  watch.setTextColor(RGB565_GRAY_LIGHT);
  watch.drawString("PictoWatch", 32, 80, GFXFF);
  watch.drawString("Version 1.0.0", 32, 110, GFXFF);
  watch.drawString("Prototype", 32, 140, GFXFF);
  delay(10000);
}

void drawError(const char *message) {
  watch.fillScreen(RGB565_BLACK_OUTER_SPACE);
  //watch.setTextSize(2);
  watch.setTextColor(RGB565_RED);
  watch.setCursor(10, 10);
  watch.println(F("ERROR:"));
  watch.setTextColor(RGB565_WHITE);
  watch.setCursor(10, 40);
  watch.println(message);
}

void drawMain() {

  watch.fillScreen(BG_COLOR);

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
  //Serial.print(config_name);
  //Serial.println("***************");

  watch.setTextDatum(ML_DATUM);
  watch.setFreeFont(CF_OL24);

  // top day part
  watch.fillRect(0, 0, 80, 30, TOP_RECT_BG_COLOR_1);
  watch.setTextColor(TOP_RECT_TEXT_COLOR_1, TOP_RECT_BG_COLOR_1);
  watch.drawString("Mon", 4, 12);

  // top time part
  watch.fillRect(80, 0, 80, 30, TOP_RECT_BG_COLOR_1);
  watch.setTextColor(TOP_RECT_TEXT_COLOR_1, TOP_RECT_BG_COLOR_1);
  watch.drawString("13:00", 84, 12);

  // top batt part
  watch.fillRect(160, 0, 80, 30, TOP_RECT_BG_COLOR_1);
  watch.setTextColor(TOP_RECT_TEXT_COLOR_1, TOP_RECT_BG_COLOR_1);
  watch.drawString("  batt", 164, 12);

  // picto part
  watch.fillSmoothRoundRect(48, 48, picto_box_width, picto_box_height, 5, FG_COLOR, BG_COLOR);

  // by default, if there is no current activity, the first one will be current
  for (int i = 0; i < config_activities_size; i++) {
    // current
    if (i == ps_current_activity_index) {
      // draw the picto
      Serial.print("Drawing current activity picto: ");  // FIXME: remove later
      Serial.print(_array_picto[i]);                     // FIXME: remove later
      drawPicto(_array_picto[i]);

      // now check if this activity is marked done in
      if (get_pspref_activity_done(ps_current_activity_index) == 1) {
        Serial.print(ps_current_activity_index);  // FIXME: remove later
        Serial.println(" is marked done");        // FIXME: remove later
                                                  // drawMarkedDone();
      }
    }
  }

  // TEST, uncomment below
  // config_activities_size = 5;

  int _circle_x;
  int _dist_between;
  int _size_circle;
  // Replace switch-case with array lookup and for loop for activity circle parameters
  const int max_params = 18;
  int circle_x_vals[max_params] = {120, 104, 82, 68, 56, 45, 37, 30, 25, 23, 20, 18, 17, 16, 15, 14, 13, 13};
  int dist_between_vals[max_params] = {46, 43, 40, 37, 34, 31, 28, 26, 24, 22, 20, 18, 17, 16, 15, 14, 13, 12};
  int size_circle_vals[max_params] = {6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3};

  if (config_activities_size >= 1 && config_activities_size <= max_params) {
    int idx = config_activities_size - 1;
    _circle_x = circle_x_vals[idx];
    _dist_between = dist_between_vals[idx];
    _size_circle = size_circle_vals[idx];
  } else {
    // Default values for out-of-range sizes
    _circle_x = 14;
    _dist_between = 16;
    _size_circle = 6;
  }

  if (config_activities_size < config_activities_size_max) {
    for (int i = 0; i < config_activities_size; i++) {
      // small indicator (rectangle) for the current activity
      if (i == ps_current_activity_index) {
        watch.fillRect(_circle_x - 8, 219, 16, 4, DAYPERIOD_CIRCLE_BG_COLOR);
      }
      if (get_pspref_activity_done(i) == 1) {
        watch.fillSmoothCircle(_circle_x, 212, _size_circle, COLOR_DONE, BG_COLOR);
      } else {
        watch.fillSmoothCircle(_circle_x, 212, _size_circle, COLOR_TODO, BG_COLOR);
      }
      _circle_x = _circle_x + _dist_between;
    }
  }
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
      // Initialize FS
      if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
        Serial.println("An error occurred while mounting FS");
        return;
      } else {
        Serial.println("FS mounted successfully");
        getConfigData();
      }
      // TODO: make check for md5sum checksum of config file
      if (STATUS_GET_CONFIG_DATA_FS_OK) {
        STATUS_CONFIG_DATA_OK = true;
        Serial.println("config successfully read from FS");
      } else {
        Serial.println("ERROR: error reading config from FS");
      }
      Serial.println("initialization complete");
      drawSplash();
      break;
  }
}

void loop() {

  set_devicemode(4);  // FIXME: set devicemode to 1, for now, remove later

  if (STATUS_CONFIG_DATA_OK) {
    drawMain();
  } else {
    // print error message to screen
    drawError("ERROR: config data not available");
  }
  delay(100000);  // wait for 100 second
}

static uint8_t conv2d(const char *p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9') {
    v = *p - '0';
  }
  return 10 * v + *++p - '0';
}
