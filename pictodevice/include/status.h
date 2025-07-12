#pragma once
#include <Arduino.h>

struct AppStatus {
    boolean wifi_ok = false;
    boolean wifi_mgr_ok = false;
    boolean wifi_mgr_config_mode_ok = false;
    boolean time_ok = false;
    boolean ntp_ok = false;
    boolean config_data_ok = false;
    boolean ble_ok = false;
    boolean get_config_data_spiff_ok = false;
    boolean set_config_data_spiff_ok = false;
    boolean get_config_data_http_ok = false;
};

extern AppStatus app_status;
