// adapted from https://github.com/lmarzen/esp32-weather-epd/blob/main/platformio/include/_locale.h
#ifndef ___LOCALE_H__
#define ___LOCALE_H__

#include <vector>
#include <Arduino.h>

//#define LOCALE en_GB
#define LOCALE nl_NL

// WIFI
extern const char *TXT_WIFI_EXCELLENT;
extern const char *TXT_WIFI_GOOD;
extern const char *TXT_WIFI_FAIR;
extern const char *TXT_WIFI_WEAK;
extern const char *TXT_WIFI_NO_CONNECTION;

// WIFI STATUS
extern const char *TXT_WL_NO_SHIELD;
extern const char *TXT_WL_IDLE_STATUS;
extern const char *TXT_WL_NO_SSID_AVAIL;
extern const char *TXT_WL_SCAN_COMPLETED;
extern const char *TXT_WL_CONNECTED;
extern const char *TXT_WL_CONNECT_FAILED;
extern const char *TXT_WL_CONNECTION_LOST;
extern const char *TXT_WL_DISCONNECTED;

// WIFI MANAGER
extern const char *TXT_WM_WEBPORTAL_STARTED;
extern const char *TXT_WM_CONNECT_SSID;
extern const char *TXT_WM_WITH_PASSWORD;
extern const char *TXT_WM_CONFIG_DEVICE;

#endif