#include "devicemode.h"

uint8_t dmAddress = 0;
uint8_t dm1flagAddress = 1;
uint8_t dm2flagAddress = 2;
uint8_t dm3flagAddress = 3;
uint8_t devicemode = 0;
bool devicemode_1_flag = false;
bool devicemode_2_flag = false;
bool devicemode_3_flag = false;
uint8_t _rc;

void set_devicemode(int _devicemode) {

    EEPROM.put(dmAddress, _devicemode);
    EEPROM.get(dmAddress, _devicemode);  //to confirm that it works
}

uint8_t get_devicemode(void) {

    EEPROM.get(dmAddress, _rc);
    if(_rc == 0) {
        Serial.println("DEBUG: attempting to set devicemode 1 "); // FIXME: debug, remove later
        EEPROM.put(dmAddress, 1); // if devicemode = 0, then set to 1 for first use
    }
    Serial.print("DEBUG: _rc is: "); // FIXME: debug, remove later
    Serial.println(_rc); // FIXME: debug, remove later
    return _rc;
}
