#pragma once
#include <Preferences.h>

#define RW_MODE false
#define RO_MODE true

extern Preferences dmPrefs; // preferences

// Namespace
extern const char* NS;

// device mode
extern uint8_t desired_devicemode;  // device mode 4 is default, normal mode
extern bool draw_device_mode_config;
extern bool draw_device_mode_1;
extern bool draw_device_mode_2;
extern bool draw_device_mode_3;

////////////////////////////////////////////////////
//
// Device mode
//
////////////////////////////////////////////////////
// _device_mode and flags are written to and read 
// from Preferences permanent storage.
//
extern int devicemode;
//
// The device can operate in 4 modes, 
// 1. wifi config mode
// 2. picto series update mode
// 3. config mode
// 4. regular mode
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

////////////////////////////////////////////////////
//
// Device mode end
//
////////////////////////////////////////////////////