#pragma once

#include <Arduino.h>
#include <WiFiManager.h>

void initTime(String timezone);
bool initWiFi();
void init_ESPAsync_Ws();
void configModeCallback(WiFiManager *myWiFiManager);
