#include "devicemode.h"

Preferences dmPrefs; // preferences

int devicemode = 0;
bool devicemode_1_flag = false;
bool devicemode_2_flag = false;
bool devicemode_3_flag = false;
const char* NS = "DMPrefs";

uint8_t mode = 0;    
uint8_t desired_mode = 3;  // device mode 3 is default, normal mode
bool draw_device_mode_config = false;
bool draw_device_mode_1 = false;
bool draw_device_mode_2 = false;

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
            break;
        case 2:
            draw_device_mode_1 = false;
            draw_device_mode_2 = true;
            break;
        case 3:
            draw_device_mode_1 = false;
            draw_device_mode_2 = false;
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

void set_devicemode_1_flag(bool _devicemode_1_flag) {
    dmPrefs.end();
    dmPrefs.begin(NS, RW_MODE);
    dmPrefs.putBool("devicemode_1_flag", _devicemode_1_flag);
    dmPrefs.end();                           
    dmPrefs.begin(NS, RO_MODE);
}
