// ABOUTME: Application status management for PictoStick
// ABOUTME: Centralized status flags using structured approach

#ifndef STATUS_H_
#define STATUS_H_

#include <Arduino.h>

// =====================================================
// Application Status Structure
// =====================================================
// Uses individual boolean flags for clarity and ease of use
// Alternative: bit-field enum for memory efficiency (future optimization)

struct AppStatus {
    // WiFi Status
    boolean wifi_ok = false;                    // Connected to WiFi network
    boolean wifi_mgr_ok = false;                // WiFiManager initialized
    boolean wifi_mgr_config_mode_ok = false;    // WiFiManager in AP config mode
    
    // Time/NTP Status
    boolean time_ok = false;                    // Time synchronized
    boolean ntp_ok = false;                     // NTP server contacted successfully
    
    // Configuration Status
    boolean config_data_ok = false;             // Configuration data loaded
    boolean get_config_data_spiff_ok = false;   // Config read from SPIFFS
    boolean set_config_data_spiff_ok = false;   // Config written to SPIFFS
    boolean get_config_data_http_ok = false;    // Config retrieved via HTTP
    
    // Hardware Status
    boolean ble_ok = false;                     // Bluetooth initialized
    
    // =====================================================
    // Convenience Methods
    // =====================================================
    
    // Check if system is fully operational
    bool isReady() const {
        return wifi_ok && ntp_ok && config_data_ok;
    }
    
    // Check if WiFi is available (either connected or in config mode)
    bool isWiFiAvailable() const {
        return wifi_ok || wifi_mgr_config_mode_ok;
    }
    
    // Check if configuration is available from any source
    bool hasConfig() const {
        return config_data_ok || get_config_data_spiff_ok || get_config_data_http_ok;
    }
    
    // Reset all status flags
    void reset() {
        wifi_ok = false;
        wifi_mgr_ok = false;
        wifi_mgr_config_mode_ok = false;
        time_ok = false;
        ntp_ok = false;
        config_data_ok = false;
        get_config_data_spiff_ok = false;
        set_config_data_spiff_ok = false;
        get_config_data_http_ok = false;
        ble_ok = false;
    }
};

// Global status instance
extern AppStatus app_status;

#endif // STATUS_H_
