#include "uimain.h"
#include <M5StickCPlus2.h>
#include <TFT_eSPI.h>
#include "common.h"
#include "PSpref.h"
#include <PNGdec.h>
#include "NotoSansBold15.h"
#include "smallFont.h"
#include "ui_helpers.h"
#include "fs_helpers.h"

// Forward declarations from main.cpp
extern TFT_eSprite sprite;
extern TFT_eSPI tft;
extern JsonDocument cdoc;
extern int16_t xpos;
extern int16_t ypos;
extern int ps_current_activity_index;
extern String lang;
extern int devicemode;
extern bool draw_device_mode_config;
extern struct tm timeinfo;

// Static variables for caching config data
static String lastLoadedDay = "";
static bool configLoadedForCurrentDay = false;

String getCurrentDayName() {
  char dayName[10];
  strftime(dayName, sizeof(dayName), "%A", &timeinfo);
  String dayNameStr = String(dayName);
  dayNameStr.toLowerCase();
  if (dayNameStr == "monday") return "monday";
  if (dayNameStr == "tuesday") return "tuesday";
  if (dayNameStr == "wednesday") return "wednesday";
  if (dayNameStr == "thursday") return "thursday";
  if (dayNameStr == "friday") return "friday";
  if (dayNameStr == "saturday") return "saturday";
  if (dayNameStr == "sunday") return "sunday";
  return "monday";
}

void drawMain() {
  StickCP2.Display.powerSaveOff();
  StickCP2.Display.setBrightness(get_pspref_brightness());
  sprite.createSprite(MY_WIDTH, MY_HEIGHT);
  sprite.fillSprite(currentTheme.bgColor);
  sprite.setTextSize(2);

  String currentDay = getCurrentDayName();

  // Only load config if the day has changed or it hasn't been loaded yet for the current day
  if (currentDay != lastLoadedDay || !configLoadedForCurrentDay) {
    String filename = "/data_" + currentDay + ".json";
    getConfigDataSPIFF(filename.c_str());
    lastLoadedDay = currentDay;
    configLoadedForCurrentDay = true;
  }

  // Validate that config was loaded successfully before accessing
  if (cdoc.isNull() || !cdoc.containsKey("activities")) {
    Serial.println("ERROR: Config data not loaded or missing 'activities' key");
    // Draw error message instead of crashing
    sprite.setTextColor(TFT_RED, currentTheme.bgColor);
    sprite.drawString("Config Error", 10, 50);
    StickCP2.Display.pushImage(0, 0, MY_WIDTH, MY_HEIGHT, (uint16_t *)sprite.getPointer());
    return;  // Exit early to prevent crash
  }

  JsonArray activities = cdoc["activities"];

  // extract values from config JSON object
  config_activities_size = activities.size();
  
  // Validate activities size to prevent out-of-bounds access
  if (config_activities_size <= 0 || config_activities_size > config_activities_size_max) {
    Serial.printf("ERROR: Invalid activities size: %d (max: %d)\n", config_activities_size, config_activities_size_max);
    config_activities_size = 0;  // Prevent any array access
  }
  
  config_name = cdoc["name"].as<String>();

  int _i = 0;
  String _array_order[config_activities_size];
  String _array_picto[config_activities_size];
  String _array_desc[config_activities_size];
  int _array_activity_marked_done[config_activities_size];
  for (JsonObject activity : activities) {
    if (_i >= config_activities_size) break;  // Safety check
    _array_order[_i] = String(activity["order"].as<int>());
    _array_picto[_i] = String(activity["picto"].as<String>());
    _array_desc[_i] = String(activity["description"].as<String>());
    _i = _i + 1;
  }

  ps_current_activity_index = get_pspref_current_activity_index();
  
  // Validate activity index bounds
  if (ps_current_activity_index < 0 || ps_current_activity_index >= config_activities_size) {
    Serial.printf("WARNING: Activity index %d out of bounds, resetting to 0\n", ps_current_activity_index);
    ps_current_activity_index = 0;
    set_pspref_current_activity_index(0);
  }

  sprite.setTextColor(TFT_WHITE, currentTheme.rightRectBgColor1);

  sprite.fillSmoothRoundRect(10, 10, picto_box_width, picto_box_height, 5, currentTheme.fgColor, currentTheme.bgColor);

  drawTime();
  drawUserName();
  drawBatt();

  for (int i = 0; i < config_activities_size; i++) {
    if (i == ps_current_activity_index) {
      drawPicto(_array_picto[i]);
      if (get_pspref_activity_done(currentDay, ps_current_activity_index)) {
        drawName(_array_desc[i], 1);
      } else {
        drawName(_array_desc[i], 0);
      }
      if (get_pspref_activity_done(currentDay, ps_current_activity_index)) {
        // drawMarkedDone(); // This function draws a large X, might not be what you want here
      }
    }
  }

  int _circle_x;
  int _dist_between;
  int _size_circle;
  switch (config_activities_size) {
    case 1:
      _circle_x = 120;
      _dist_between = 46;
      _size_circle = 6;
      break;
    case 2:
      _circle_x = 104;
      _dist_between = 43;
      _size_circle = 6;
      break;
    case 3:
      _circle_x = 82;
      _dist_between = 40;
      _size_circle = 6;
      break;
    case 4:
      _circle_x = 68;
      _dist_between = 37;
      _size_circle = 6;
      break;
    case 5:
      _circle_x = 56;
      _dist_between = 34;
      _size_circle = 6;
      break;
    case 6:
      _circle_x = 45;
      _dist_between = 31;
      _size_circle = 6;
      break;
    case 7:
      _circle_x = 37;
      _dist_between = 28;
      _size_circle = 5;
      break;
    case 8:
      _circle_x = 30;
      _dist_between = 26;
      _size_circle = 5;
      break;
    case 9:
      _circle_x = 25;
      _dist_between = 24;
      _size_circle = 5;
      break;
    case 10:
      _circle_x = 23;
      _dist_between = 22;
      _size_circle = 5;
      break;
    case 11:
      _circle_x = 20;
      _dist_between = 20;
      _size_circle = 5;
      break;
    case 12:
      _circle_x = 18;
      _dist_between = 18;
      _size_circle = 4;
      break;
    case 13:
      _circle_x = 17;
      _dist_between = 17;
      _size_circle = 4;
      break;
    case 14:
      _circle_x = 16;
      _dist_between = 16;
      _size_circle = 4;
      break;
    case 15:
      _circle_x = 15;
      _dist_between = 15;
      _size_circle = 4;
      break;
    case 16:
      _circle_x = 14;
      _dist_between = 14;
      _size_circle = 4;
      break;
    case 17:
      _circle_x = 13;
      _dist_between = 13;
      _size_circle = 3;
      break;
    case 18:
      _circle_x = 13;
      _dist_between = 12;
      _size_circle = 3;
      break;
    default:
      _circle_x = 14;
      _dist_between = 16;
      _size_circle = 6;
      break;
  }

  sprite.fillRect(0, 128, 240, 5, currentTheme.bgColor);

  if (config_activities_size < config_activities_size_max) {
    for (int i = 0; i < config_activities_size; i++) {
      if (i == ps_current_activity_index) {
        sprite.fillRect(_circle_x - 8, 129, 16, 4, currentTheme.dayPeriodCircleBgColor);
      }
      if (get_pspref_activity_done(currentDay, i) == 1) {
        sprite.fillSmoothCircle(_circle_x, 122, _size_circle, currentTheme.colorDone, currentTheme.bgColor);
      } else {
        sprite.fillSmoothCircle(_circle_x, 122, _size_circle, currentTheme.colorTodo, currentTheme.bgColor);
      }
      _circle_x = _circle_x + _dist_between;
    }
  }

  StickCP2.Display.pushImage(0, 0, MY_WIDTH, MY_HEIGHT, (uint16_t *)sprite.getPointer());

  // Static variables to track long press state
  static unsigned long btnA_press_start_time = 0;
  static bool btnA_long_press_triggered = false;
  static unsigned long btnB_press_start_time = 0;
  static bool btnB_long_press_triggered = false;

  // button action
  if (devicemode == 4) {

    // === PWR Button (Backwards) ===
    if (StickCP2.BtnPWR.wasPressed()) {
      sleepTime = get_pspref_timeout();
      ps_current_activity_index = get_pspref_current_activity_index();
      if (ps_current_activity_index >= 1) {
        set_pspref_current_activity_index(ps_current_activity_index - 1);
      } else {
        if (get_pspref_buzzer()) {
          StickCP2.Speaker.tone(6000, 100);
        }
      }
    }

    // === BtnB (Forwards / Config) ===
    if (StickCP2.BtnB.isPressed()) {
      // Button is currently held
      if (btnB_press_start_time == 0) {
        btnB_press_start_time = millis();  // Record press start
      }
      
      // Check if held for 5 seconds
      if (!btnB_long_press_triggered && (millis() - btnB_press_start_time >= 5000)) {
        if (get_pspref_buzzer()) {
          StickCP2.Speaker.tone(4000, 20);
          delay(1000);
        }
        draw_device_mode_config = true;
        btnB_long_press_triggered = true;
      }
    } else {
      // Button was released
      unsigned long press_duration = millis() - btnB_press_start_time;
      
      // If released before 5 seconds and not already triggered, it's a short press
      if (!btnB_long_press_triggered && press_duration < 5000 && press_duration > 100) {
        // Short press - increment activity index
        sleepTime = get_pspref_timeout();
        ps_current_activity_index = get_pspref_current_activity_index();
        if (ps_current_activity_index < config_activities_size - 1) {
          set_pspref_current_activity_index(ps_current_activity_index + 1);
        } else {
          if (get_pspref_buzzer()) {
            StickCP2.Speaker.tone(6000, 100);
          }
        }
      }
      
      // Reset state
      btnB_press_start_time = 0;
      btnB_long_press_triggered = false;
    }

    // === BtnA (Reset All / Toggle) ===
    if (StickCP2.BtnA.isPressed()) {
      // Button is currently held
      if (btnA_press_start_time == 0) {
        btnA_press_start_time = millis();  // Record press start
      }
      
      // Check if held for 5 seconds
      if (!btnA_long_press_triggered && (millis() - btnA_press_start_time >= 5000)) {
        set_pspref_all_activity_undone();
        if (get_pspref_buzzer()) {
          StickCP2.Speaker.tone(6000, 100);
        }
        btnA_long_press_triggered = true;
      }
    } else {
      // Button was released
      unsigned long press_duration = millis() - btnA_press_start_time;
      
      // If released before 5 seconds and not already triggered, it's a short press
      if (!btnA_long_press_triggered && press_duration < 5000 && press_duration > 100) {
        // Short press - toggle current activity
        sleepTime = get_pspref_timeout();
        if (get_pspref_activity_done(currentDay, ps_current_activity_index)) {
          set_pspref_activity_done(currentDay, ps_current_activity_index, false);
        } else {
          set_pspref_activity_done(currentDay, ps_current_activity_index, true);
        }
        if (get_pspref_buzzer()) {
          StickCP2.Speaker.tone(6000, 100);
        }
      }
      
      // Reset state
      btnA_press_start_time = 0;
      btnA_long_press_triggered = false;
    }
  }
}
