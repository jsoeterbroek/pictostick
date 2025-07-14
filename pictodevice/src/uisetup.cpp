#include "uisetup.h"
#include <M5StickCPlus2.h>
#include <TFT_eSPI.h>
#include "common.h"
#include "PSpref.h"
#include "devicemode.h"
#include "smallFont.h"
#include "secFont.h"
#include "_locale.h"
#include "system.h"
#include "status.h"

// Forward declarations from main.cpp
extern TFT_eSprite sprite;
extern WiFiManager wm;
extern AppStatus app_status;
extern struct tm timeinfo;
extern String timezone;


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

  WiFiManagerParameter custom_timezone("timezone", "Timezone", get_pspref_timezone().c_str(), 100);
  wm.addParameter(&custom_timezone);


  if (!wm.startConfigPortal("PictoStick")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
  } else {
    app_status.wifi_mgr_ok = true;
    Serial.println("WiFi connected.");
    set_pspref_timezone(custom_timezone.getValue());
  }

  // set NTP time
  initTime(get_pspref_timezone());

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

void drawDeviceMode3() {
  // time
  struct tm timeinfo;
  auto dt = StickCP2.Rtc.getDateTime();
  char hourbuffer[6];
  char minutebuffer[6];
  char secondbuffer[6];
  snprintf(hourbuffer, sizeof(hourbuffer), "%02d", dt.time.hours);
  snprintf(minutebuffer, sizeof(minutebuffer), "%02d", dt.time.minutes);
  snprintf(secondbuffer, sizeof(secondbuffer), "%02d", dt.time.seconds);

  hour = atoi(hourbuffer);
  minute = atoi(minutebuffer);
  second = atoi(secondbuffer);

  StickCP2.Display.setBrightness(get_pspref_brightness());

  sprite.createSprite(MY_WIDTH, MY_HEIGHT);
  sprite.fillSprite(RGB565_GRAY_LIGHT);
  sprite.setTextDatum(4);

  sprite.loadFont(smallFont);
  sprite.setTextColor(TFT_ORANGE, RGB565_GRAY_LIGHT);
  sprite.drawString(TXT_DM3_SETUP, 28, 14);
  sprite.unloadFont();

  // element 1  -- hours
  sprite.loadFont(smallFont);
  sprite.setTextColor(RGB565_GRAY_BATTLESHIP, RGB565_GRAY_LIGHT);
  sprite.drawString(TXT_DM3_HOUR, 28, 36);
  sprite.unloadFont();
  sprite.loadFont(secFont);
  if (cursor == 1) {
    sprite.fillRect(10, 48, 36, 40, RGB565_GRAY_BATTLESHIP);
    sprite.setTextColor(TFT_ORANGE, RGB565_GRAY_BATTLESHIP);
  } else {
    sprite.fillRect(10, 48, 36, 40, TFT_WHITE);
    sprite.setTextColor(RGB565_GRAY_BATTLESHIP, TFT_WHITE);
  }
  sprite.drawString(hourbuffer, 27, 70);
  sprite.unloadFont();

  // element 2  -- minutes
  sprite.loadFont(smallFont);
  sprite.setTextColor(RGB565_GRAY_BATTLESHIP, RGB565_GRAY_LIGHT);
  sprite.drawString(TXT_DM3_MIN, 68, 36);
  sprite.unloadFont();
  sprite.loadFont(secFont);
  if (cursor == 2) {
    sprite.fillRect(51, 48, 36, 40, RGB565_GRAY_BATTLESHIP);
    sprite.setTextColor(TFT_ORANGE, RGB565_GRAY_BATTLESHIP);
  } else {
    sprite.fillRect(51, 48, 36, 40, TFT_WHITE);
    sprite.setTextColor(RGB565_GRAY_BATTLESHIP, TFT_WHITE);
  }
  sprite.drawString(minutebuffer, 68, 70);
  sprite.unloadFont();

  // element 3  -- seconds
  sprite.loadFont(smallFont);
  sprite.setTextColor(RGB565_GRAY_BATTLESHIP, RGB565_GRAY_LIGHT);
  sprite.drawString(TXT_DM3_SEC, 108, 36);
  sprite.unloadFont();
  sprite.loadFont(secFont);
  if (cursor == 3) {
    sprite.fillRect(92, 48, 36, 40, RGB565_GRAY_BATTLESHIP);
    sprite.setTextColor(TFT_ORANGE, RGB565_GRAY_BATTLESHIP);
  } else {
    sprite.fillRect(92, 48, 36, 40, TFT_WHITE);
    sprite.setTextColor(RGB565_GRAY_BATTLESHIP, TFT_WHITE);
  }
  sprite.drawString(secondbuffer, 108, 70);
  sprite.unloadFont();

  // element 4 -- brightness
  if (cursor == 4) {
    sprite.fillRect(10, 92, 120, 30, RGB565_GRAY_BATTLESHIP);
    sprite.setTextColor(TFT_ORANGE, RGB565_GRAY_BATTLESHIP);
  } else {
    sprite.fillRect(10, 92, 120, 30, TFT_WHITE);
    sprite.setTextColor(RGB565_GRAY_BATTLESHIP, TFT_WHITE);
  }
  sprite.setTextDatum(0);
  sprite.loadFont(smallFont);
  sprite.drawString(TXT_DM3_BRIGHTNESS, 15, 100);
  sprite.drawNumber(get_pspref_brightness(), 100, 100);
  sprite.unloadFont();

  // element 5 -- timeout
  if (cursor == 5) {
    sprite.fillRect(136, 58, 96, 30, RGB565_GRAY_BATTLESHIP);
    sprite.setTextColor(TFT_ORANGE, RGB565_GRAY_BATTLESHIP);
  } else {
    sprite.fillRect(136, 58, 96, 30, TFT_WHITE);
    sprite.setTextColor(RGB565_GRAY_BATTLESHIP, TFT_WHITE);
  }
  sprite.setTextDatum(0);
  sprite.loadFont(smallFont);
  sprite.drawString(TXT_DM3_TIMEOUT, 140, 66);
  sprite.drawNumber(get_pspref_timeout(), 204, 66);
  sprite.unloadFont();

  // element 6 -- buzzer on/off
  if (cursor == 6) {
    sprite.fillRect(136, 92, 96, 30, RGB565_GRAY_BATTLESHIP);
    sprite.setTextColor(TFT_ORANGE, RGB565_GRAY_BATTLESHIP);
  } else {
    sprite.fillRect(136, 92, 96, 30, TFT_WHITE);
    sprite.setTextColor(RGB565_GRAY_BATTLESHIP, TFT_WHITE);
  }
  sprite.loadFont(smallFont);
  sprite.drawString(TXT_DM3_BUZZER, 140, 100);

  if (get_pspref_buzzer()) {
    sprite.drawString(TXT_DM3_ON, 196, 100);
  } else {
    sprite.drawString(TXT_DM3_OFF, 196, 100);
  }

  sprite.unloadFont();
  StickCP2.Display.pushImage(0, 0, MY_WIDTH, MY_HEIGHT, (uint16_t *)sprite.getPointer());

  // button action
  if (StickCP2.BtnPWR.wasPressed()) {
    if (get_pspref_buzzer()) {
      StickCP2.Speaker.tone(6000, 100);
    }
    set_devicemode(4);
  }

  if (StickCP2.BtnB.wasPressed()) {
    if (get_pspref_buzzer()) {
      StickCP2.Speaker.tone(6000, 100);
    }
    switch (cursor) {
      case 0: cursor = 6; break;
      case 1: cursor = 2; break;
      case 2: cursor = 3; break;
      case 3: cursor = 4; break;
      case 4: cursor = 5; break;
      case 5: cursor = 6; break;
      case 6: cursor = 1; break;
    }
  }

  if (StickCP2.BtnA.wasPressed()) {
    if (get_pspref_buzzer()) {
      StickCP2.Speaker.tone(6000, 100);
    }
    if (cursor == 1) {
      // increment hour
      hour++;
      if (hour > 23) {
        hour = 0;
      }
      StickCP2.Rtc.setDateTime({{dt.date.year, dt.date.month, dt.date.date}, {hour, minute, second}});
      delay(200);
    } else if (cursor == 2) {
      // increment minute
      minute++;
      if (minute > 59) {
        minute = 0;
      }
      StickCP2.Rtc.setDateTime({{dt.date.year, dt.date.month, dt.date.date}, {hour, minute, second}});
      delay(200);
    } else if (cursor == 3) {
      // increment second
      second++;
      if (second > 59) {
        second = 0;
      }
      StickCP2.Rtc.setDateTime({{dt.date.year, dt.date.month, dt.date.date}, {hour, minute, second}});
      delay(200);
    } else if (cursor == 4) {
      // increment brightness
      incr_pspref_brightness();
      delay(200);
    } else if (cursor == 5) {
      // increment timeout
      incr_pspref_timeout();
      delay(200);
    } else if (cursor == 6) {
      if (get_pspref_buzzer()) {
        set_pspref_buzzer(false);
      } else {
        set_pspref_buzzer(true);
      }
    }
  }
}