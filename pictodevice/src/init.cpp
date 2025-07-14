#include "init.h"
#include <Arduino.h>
#include <M5StickCPlus2.h>
#include "devicemode.h"
#include "PSpref.h"
#include "common.h"
#include "ui_helpers.h"
#include "status.h"
#include "uisetup.h"
#include "fs_helpers.h"

void init_device() {
  auto cfg = M5.config();
  StickCP2.begin(cfg);
  StickCP2.Power.begin();
  StickCP2.Display.setRotation(3);
  StickCP2.Display.fillScreen(TFT_WHITE);
  Serial.begin(115200);
  Serial.println("start initialization..");
}

void init_preferences() {
  psPrefs.begin(PSNS, PS_RO_MODE);
  dmPrefs.begin(NS, RO_MODE);

  bool dmInit = dmPrefs.isKey("devicemode");
  if (dmInit == false) {
    dmPrefs.end();
    dmPrefs.begin(NS, RW_MODE);
    dmPrefs.putInt("devicemode", 1);
    dmPrefs.end();
    dmPrefs.begin(NS, RO_MODE);
  }
}

void init_filesystem() {
  // This is a placeholder.
  // Filesystem-related initializations will go here.
}

void init_devicemode() {
  Serial.print(" * Starting in mode:");
  devicemode = get_devicemode();
  Serial.println(devicemode);

  switch (devicemode) {
    case 0:
      Serial.println("mode 0");
      Serial.println("rebooting..");
      delay(10000);
      ESP.restart();
      break;
    case 1: handle_devicemode_1(); break;
    case 2: handle_devicemode_2(); break;
    case 3: handle_devicemode_3(); break;
    case 4: handle_devicemode_4(); break;
  }
}

void handle_devicemode_1() {
  Serial.println("mode 1");
  draw_device_mode_1 = true;
}

void handle_devicemode_2() {
  Serial.println("mode 2");
  draw_device_mode_2 = true;
}

void handle_devicemode_3() {
  Serial.println("mode 3");
  draw_device_mode_3 = true;
}

void handle_devicemode_4() {
  Serial.println("mode 4");
  if (GET_CONFIG_DATA_SPIFF) {
    getConfigDataSPIFF();
  }
  if (app_status.get_config_data_spiff_ok) {
    app_status.config_data_ok = true;
    Serial.println("config successfully read from fs");
  } else {
    Serial.println("ERROR: error reading config from fs");
  }
  Serial.println("initialization complete");
  drawSplash();
}
