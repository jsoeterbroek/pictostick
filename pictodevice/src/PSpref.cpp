#include "PSpref.h"

Preferences psPrefs; // preferences

int pspref_current_activity_index = 0;
// reserve for 20 activities max
bool pspref_activity_done = false;

const char* PSNS = "PSPrefs";

void set_pspref_buzzer(bool _pspref_buzzer) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putBool("ps_z", _pspref_buzzer);
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RO_MODE);
}

bool get_pspref_buzzer(void) {
    bool _rc = false;
    _rc = psPrefs.getBool("ps_z");
    return _rc;
}

void set_pspref_brightness(int _pspref_brightness) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putInt("ps_b", _pspref_brightness);
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RO_MODE);
}

int get_pspref_brightness(void) {
    int _rc = 0;
    _rc = psPrefs.getInt("ps_b");
    //Serial.print("DEBUG: PSpref _rc is: "); // FIXME: debug, remove later
    //Serial.println(_rc); // FIXME: debug, remove later
    if (_rc == 0) {
        set_pspref_brightness(16);
        _rc = 16;
    }
    return _rc;
}

void incr_pspref_brightness(void) {
    int _b = get_pspref_brightness();
    switch (_b) {
    case 16:
        set_pspref_brightness(32); break;
    case 32:
        set_pspref_brightness(48); break;
    case 48:
        set_pspref_brightness(64); break;
    case 64:
        set_pspref_brightness(96); break;
    case 96:
        set_pspref_brightness(132); break;
    case 132:
        set_pspref_brightness(164); break;
    case 164:
        set_pspref_brightness(180); break;
    case 180:
        set_pspref_brightness(16); break;
    default:
        set_pspref_brightness(32); break;
    }
}

void decr_pspref_brightness(void) {
    int _b = get_pspref_brightness();
    switch (_b) {
    case 16:
        set_pspref_brightness(16); break;
    case 32:
        set_pspref_brightness(16); break;
    case 48:
        set_pspref_brightness(32); break;
    case 64:
        set_pspref_brightness(48); break;
    case 96:
        set_pspref_brightness(64); break;
    case 132:
        set_pspref_brightness(96); break;
    case 164:
        set_pspref_brightness(132); break;
    case 180:
        set_pspref_brightness(164); break;
    default:
        set_pspref_brightness(61); break;
    }
}

void set_pspref_current_activity_index(int _pspref_current_activity_index) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);
    psPrefs.putInt("ps_i", _pspref_current_activity_index);
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RO_MODE);
}

//uint8_t get_pspref_current_activity_index(void) {
int get_pspref_current_activity_index(void) {
    int _rc = 0;
    _rc = psPrefs.getInt("ps_i");
    //Serial.print("DEBUG: PSpref _rc is: "); // FIXME: debug, remove later
    //Serial.println(_rc); // FIXME: debug, remove later
    return _rc;
}

void set_pspref_activity_undone(int _pspref_current_activity_index) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);

    switch (_pspref_current_activity_index) {
        case 0:
            psPrefs.putInt("ps_a_0", 0); break;
        case 1:
            psPrefs.putInt("ps_a_1", 0); break;
        case 2:
            psPrefs.putInt("ps_a_2", 0); break;
        case 3:
            psPrefs.putInt("ps_a_3", 0); break;
        case 4:
            psPrefs.putInt("ps_a_4", 0); break;
        case 5:
            psPrefs.putInt("ps_a_5", 0); break;
        case 6:
            psPrefs.putInt("ps_a_6", 0); break;
        case 7:
            psPrefs.putInt("ps_a_7", 0); break;
        case 8:
            psPrefs.putInt("ps_a_8", 0); break;
        case 9:
            psPrefs.putInt("ps_a_9", 0); break;
        case 10:
            psPrefs.putInt("ps_a_10", 0); break;
        case 11:
            psPrefs.putInt("ps_a_11", 0); break;
        case 12:
            psPrefs.putInt("ps_a_12", 0); break;
        case 13:
            psPrefs.putInt("ps_a_13", 0); break;
        case 14:
            psPrefs.putInt("ps_a_14", 0); break;
        case 15:
            psPrefs.putInt("ps_a_15", 0); break;
        case 16:
            psPrefs.putInt("ps_a_16", 0); break;
        case 17:
            psPrefs.putInt("ps_a_17", 0); break;
        case 18:
            psPrefs.putInt("ps_a_18", 0); break;
        case 19:
            psPrefs.putInt("ps_a_19", 0); break;
    }
}

void set_pspref_activity_done(int _pspref_current_activity_index) {
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RW_MODE);

    switch (_pspref_current_activity_index) {
        case 0:
            psPrefs.putInt("ps_a_0", 1); break;
        case 1:
            psPrefs.putInt("ps_a_1", 1); break;
        case 2:
            psPrefs.putInt("ps_a_2", 1); break;
        case 3:
            psPrefs.putInt("ps_a_3", 1); break;
        case 4:
            psPrefs.putInt("ps_a_4", 1); break;
        case 5:
            psPrefs.putInt("ps_a_5", 1); break;
        case 6:
            psPrefs.putInt("ps_a_6", 1); break;
        case 7:
            psPrefs.putInt("ps_a_7", 1); break;
        case 8:
            psPrefs.putInt("ps_a_8", 1); break;
        case 9:
            psPrefs.putInt("ps_a_9", 1); break;
        case 10:
            psPrefs.putInt("ps_a_10", 1); break;
        case 11:
            psPrefs.putInt("ps_a_11", 1); break;
        case 12:
            psPrefs.putInt("ps_a_12", 1); break;
        case 13:
            psPrefs.putInt("ps_a_13", 1); break;
        case 14:
            psPrefs.putInt("ps_a_14", 1); break;
        case 15:
            psPrefs.putInt("ps_a_15", 1); break;
        case 16:
            psPrefs.putInt("ps_a_16", 1); break;
        case 17:
            psPrefs.putInt("ps_a_17", 1); break;
        case 18:
            psPrefs.putInt("ps_a_18", 1); break;
        case 19:
            psPrefs.putInt("ps_a_19", 1); break;
    }
    psPrefs.end();
    psPrefs.begin(PSNS, PS_RO_MODE);
}

int get_pspref_activity_done(int _pspref_current_activity_index) {
    int _rc = 0;
    switch (_pspref_current_activity_index) {
        case 0:
            _rc = psPrefs.getInt("ps_a_0"); break;
        case 1:
            _rc = psPrefs.getInt("ps_a_1"); break;
        case 2:
            _rc = psPrefs.getInt("ps_a_2"); break;
        case 3:
            _rc = psPrefs.getInt("ps_a_3"); break;
        case 4:
            _rc = psPrefs.getInt("ps_a_4"); break;
        case 5:
            _rc = psPrefs.getInt("ps_a_5"); break;
        case 6:
            _rc = psPrefs.getInt("ps_a_6"); break;
        case 7:
            _rc = psPrefs.getInt("ps_a_7"); break;
        case 8:
            _rc = psPrefs.getInt("ps_a_8"); break;
        case 9:
            _rc = psPrefs.getInt("ps_a_9"); break;
        case 10:
            _rc = psPrefs.getInt("ps_a_10"); break;
        case 11:
            _rc = psPrefs.getInt("ps_a_11"); break;
        case 12:
            _rc = psPrefs.getInt("ps_a_12"); break;
        case 13:
            _rc = psPrefs.getInt("ps_a_13"); break;
        case 14:
            _rc = psPrefs.getInt("ps_a_14"); break;
        case 15:
            _rc = psPrefs.getInt("ps_a_15"); break;
        case 16:
            _rc = psPrefs.getInt("ps_a_16"); break;
        case 17:
            _rc = psPrefs.getInt("ps_a_17"); break;
        case 18:
            _rc = psPrefs.getInt("ps_a_18"); break;
        case 19:
            _rc = psPrefs.getInt("ps_a_19"); break;
    }

    //Serial.print("DEBUG: PSpref _rc is: "); // FIXME: debug, remove later
    //Serial.print(_rc); // FIXME: debug, remove later
    //Serial.print(" for index ");
    //Serial.println(_pspref_current_activity_index);
    return _rc;
}
