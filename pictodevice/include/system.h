// ABOUTME: This file contains declarations for system-level functions.
// ABOUTME: These functions handle time synchronization, WiFi connection, and the web server.
#pragma once

#include <Arduino.h>
#include <WiFiManager.h>

void initTime(String timezone);
bool initWiFi();
void init_ESPAsync_Ws();
void configModeCallback(WiFiManager *myWiFiManager);
