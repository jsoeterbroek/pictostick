#include "PSpref.h"

Preferences psPrefs; // preferences

int pspref_current_activity_index = 0;
// reserve for 20 activities max
bool pspref_activity_done = false;

const char* PSNS = "PSPrefs";

void set_pspref_current_activity_index(int _pspref_current_activity_index) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putInt("ps_i", _pspref_current_activity_index);
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RO_MODE);
}

uint8_t get_pspref_current_activity_index(void) {
    int _rc = 0;
    _rc = psPrefs.getInt("ps_i");
    Serial.print("DEBUG: _rc is: "); // FIXME: debug, remove later
    Serial.println(_rc); // FIXME: debug, remove later
    return _rc;
}

void set_pspref_activity_00_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_00_done", _pspref_activity_done);
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_01_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_01_done", _pspref_activity_done);
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_02_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_02_done", _pspref_activity_done);
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_03_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_03_done", _pspref_activity_done);
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_04_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_04_done", _pspref_activity_done);
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_05_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_05_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_06_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_06_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_07_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_07_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_08_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_08_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_09_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_09_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_10_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_10_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_11_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_11_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_12_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_12_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_13_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_13_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_14_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_14_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_15_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_15_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_16_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_16_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_17_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_17_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_18_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_18_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}

void set_pspref_activity_19_done(bool _pspref_activity_done) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("pspref_activity_19_done", _pspref_activity_done);
    psPrefs.end();                           
    psPrefs.begin(PSNS, PS_RO_MODE);
}