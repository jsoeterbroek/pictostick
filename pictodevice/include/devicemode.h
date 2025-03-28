#pragma once
#include <Preferences.h>

#define RW_MODE false
#define RO_MODE true

extern Preferences dmPrefs; // preferences

// Namespace
extern const char* NS;

////////////////////////////////////////////////////
//
// Device mode
//
////////////////////////////////////////////////////
// _device_mode and flags are written to and read 
// from Preferences permanent storage.
//
extern int devicemode;
extern bool devicemode_1_flag;
extern bool devicemode_2_flag;
extern bool devicemode_3_flag;
//
// The device can operate in 3 modes, 
// the mode will displayed in the splash screen:
//
// 1. network config mode
//
//   checks first if network config mode has run succesfully
//   once before.
//   If yes, _devicemode is set to 3, and restarted.
//   In this mode, default mode  when first started, or after
//   reset the wifi manager is started and device can be added
//   to internet network to get NTP time and write time to RTC.
//   Sleep/low power mode disabled.
//
// int devicemode = 1;
// bool devicemode_1_flag = true
//
// 2. config mode
//  
//   Checks first if network config mode has run succesfully
//   once before and if rtc time had been set.
//   If not, _devicemode is set to 1, and restarted.
//
//   In this mode the device starts regular WiFI and checks if 
//   a new JSON config is available from HTTP and compares checksums
//   If necessery, a new JSON config is retrieved and stored on
//   SPIFFS fs.
//   Sleep/low power mode disabled.
//
// int _devicemode = 2;
// bool devicemode_2_flag = true
//
// 3. regular mode
// 
//   Checks first if network config mode (1) has run succesfully
//   once before.
//   If not, devicemode is set to 1, and restarted.
//   Checks first if config mode (2) has run succesfully
//   once before.
//   If not, devicemode is set to 2, and restarted.
//
//   In this mode the device will operate with time read from 
//   trc, and reading config file data.json from flash fs.
//   No WiFi, No NTP time config, no checking of data.json.
//   Sleep/low power mode enabled.
//
// int devicemode = 3;
// bool devicemode_3_flag = true
//
// functions to read and write to Prefrences
//
uint8_t get_devicemode();
// returns int with device mode 0,1,2 or 3.
//
// bool get_devicemode_1_flag()
// bool get_devicemode_2_flag()
// bool get_devicemode_3_flag()
// returns if flag is true or false
//
void set_devicemode(int devicemode);
// Set the device mode (options 0, 1, 2, 3) and
// reboots device
// 
// bool set_devicemode_1_flag()
void set_devicemode_1_flag(bool _devicemode_1_flag);
// bool set_devicemode_2_flag()
// bool set_devicemode_3_flag()
// returns succes (options: 0, 1)

////////////////////////////////////////////////////
//
// Device mode end
//
////////////////////////////////////////////////////