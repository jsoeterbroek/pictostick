#pragma once

#include <ArduinoJson.h>
#include <FS.h>

void deleteConfigFile(fs::FS &fs, const char *path);
void writeConfigFile(fs::FS &fs, const char *path, JsonObject _json);
void readConfigFile(fs::FS &fs, const char *path);
void writeConfigDataSPIFF(JsonObject _json);
void getConfigDataSPIFF();
