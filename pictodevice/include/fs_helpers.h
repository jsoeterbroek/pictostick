// ABOUTME: This file contains declarations for filesystem helper functions.
// ABOUTME: These functions are used for reading, writing, and deleting configuration files.
#pragma once

#include <ArduinoJson.h>
#include <FS.h>

void deleteConfigFile(fs::FS &fs, const char *path);
void writeConfigFile(fs::FS &fs, const char *path, JsonObject _json);
void readConfigFile(fs::FS &fs, const char *path);
void writeConfigDataSPIFF(JsonObject _json);
void getConfigDataSPIFF();
