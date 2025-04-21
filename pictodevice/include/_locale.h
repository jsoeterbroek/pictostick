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
extern const char *TXT_WM_START;
extern const char *TXT_WM_WEBPORTAL_STARTED;
extern const char *TXT_WM_CONNECT_SSID;
extern const char *TXT_WM_WITH_PASSWORD;
extern const char *TXT_WM_CONFIG_DEVICE;

// Device modes
extern const char *TXT_DM_SELECT;
extern const char *TXT_DM_NET_CONF;
extern const char *TXT_DM_PICTO_UPD;
extern const char *TXT_DM_NORMAL_MODE;
extern const char *TXT_DM_SET_1;
extern const char *TXT_DM_SET_2;
extern const char *TXT_DM_SET_3;
extern const char *TXT_DM_RESTART;

// device mode 2
extern const char *TXT_DM2;
extern const char *TXT_DM2_WIFI_START;
extern const char *TXT_DM2_WS_START;
extern const char *TXT_DM2_FILE_WAITING;
extern const char *TXT_DM2_FILE_OK;
extern const char *TXT_DM2_FILE_ERR;
extern const char *TXT_DM2_WIFI_DISC;

// device mode 1
extern const char *TXT_DM1;

#endif