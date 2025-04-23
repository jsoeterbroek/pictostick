#include "devicemode.h"

Preferences dmPrefs; // preferences

int devicemode = 0;
const char* NS = "DMPrefs";

uint8_t desired_devicemode = 4;  // device mode 4 is default, normal mode
bool draw_device_mode_config = false;
bool draw_device_mode_1 = false;
bool draw_device_mode_2 = false;
bool draw_device_mode_3 = false;

void set_devicemode(int _devicemode) {
    dmPrefs.end();
    dmPrefs.begin(NS, RW_MODE);
    dmPrefs.putInt("devicemode", _devicemode);
    dmPrefs.end();                           
    dmPrefs.begin(NS, RO_MODE);
    switch (_devicemode) {
        case 1:
            draw_device_mode_1 = true;
            draw_device_mode_2 = false;
            draw_device_mode_3 = false;
            break;
        case 2:
            draw_device_mode_1 = false;
            draw_device_mode_2 = true;
            draw_device_mode_3 = false;
            break;
        case 3:
            draw_device_mode_1 = false;
            draw_device_mode_2 = false;
            draw_device_mode_3 = true;
            break;
        case 4:
            draw_device_mode_1 = false;
            draw_device_mode_2 = false;
            draw_device_mode_3 = false;
            break;
    }
}

uint8_t get_devicemode(void) {
    int _rc = 0;
    _rc = dmPrefs.getInt("devicemode");
    //Serial.print("DEBUG: _rc is: "); // FIXME: debug, remove later
    //Serial.println(_rc); // FIXME: debug, remove later
    return _rc;
}