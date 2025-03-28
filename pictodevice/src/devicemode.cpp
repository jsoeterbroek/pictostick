#include "devicemode.h"

Preferences dmPrefs; // preferences

int devicemode = 0;
bool devicemode_1_flag = false;
bool devicemode_2_flag = false;
bool devicemode_3_flag = false;
const char* NS = "DMPrefs";

void set_devicemode(int _devicemode) {
    dmPrefs.end();
    dmPrefs.begin(NS, RW_MODE);
    dmPrefs.putInt("devicemode", _devicemode);
    dmPrefs.end();                           
    dmPrefs.begin(NS, RO_MODE);
}

uint8_t get_devicemode(void) {
    int _rc = 0;
    _rc = dmPrefs.getInt("devicemode");
    Serial.print("DEBUG: _rc is: "); // FIXME: debug, remove later
    Serial.println(_rc); // FIXME: debug, remove later
    return _rc;
}

void set_devicemode_1_flag(bool _devicemode_1_flag) {
    dmPrefs.end();
    dmPrefs.begin(NS, RW_MODE);
    dmPrefs.putBool("devicemode_1_flag", _devicemode_1_flag);
    dmPrefs.end();                           
    dmPrefs.begin(NS, RO_MODE);
}
