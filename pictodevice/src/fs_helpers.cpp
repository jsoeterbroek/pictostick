#include "fs_helpers.h"
#include <Arduino.h>
#include <SPIFFS.h>
#include "status.h"
#include <ArduinoJson.h>
#include "common.h"

// extern declarations for global variables defined in main.cpp
extern AppStatus app_status;
extern JsonDocument cdoc;
extern const char *cfilename;

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
