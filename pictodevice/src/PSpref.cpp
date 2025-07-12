#include "PSpref.h"

Preferences psPrefs;  // preferences

int pspref_current_activity_index = 0;
int pspref_timeout = 25;
// reserve for 20 activities max
bool pspref_activity_done = false;

const char *PSNS = "PSPrefs";

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
  if (_rc == 0) {
    set_pspref_brightness(64);
    _rc = 64;  // default brightness
  }
  return _rc;
}

void incr_pspref_brightness(void) {
  int _b = get_pspref_brightness();
  switch (_b) {
    case 16:  set_pspref_brightness(32); break;
    case 32:  set_pspref_brightness(48); break;
    case 48:  set_pspref_brightness(64); break;
    case 64:  set_pspref_brightness(96); break;
    case 96:  set_pspref_brightness(132); break;
    case 132: set_pspref_brightness(164); break;
    case 164: set_pspref_brightness(180); break;
    case 180: set_pspref_brightness(16); break;
    default:  set_pspref_brightness(32); break;
  }
}

void decr_pspref_brightness(void) {
  int _b = get_pspref_brightness();
  switch (_b) {
    case 16:  set_pspref_brightness(16); break;
    case 32:  set_pspref_brightness(16); break;
    case 48:  set_pspref_brightness(32); break;
    case 64:  set_pspref_brightness(48); break;
    case 96:  set_pspref_brightness(64); break;
    case 132: set_pspref_brightness(96); break;
    case 164: set_pspref_brightness(132); break;
    case 180: set_pspref_brightness(164); break;
    default:  set_pspref_brightness(61); break;
  }
}

void set_pspref_current_activity_index(int _pspref_current_activity_index) {
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RW_MODE);
  psPrefs.putInt("ps_i", _pspref_current_activity_index);
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RO_MODE);
}

int get_pspref_current_activity_index(void) {
  int _rc = 0;
  _rc = psPrefs.getInt("ps_i");
  return _rc;
}

void set_pspref_timeout(int _pspref_timeout) {
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RW_MODE);
  psPrefs.putInt("ps_t", _pspref_timeout);
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RO_MODE);
}

int get_pspref_timeout(void) {
  int _rc = 0;
  _rc = psPrefs.getInt("ps_t");
  if (_rc == 0) {
    set_pspref_timeout(25);
    _rc = 25;  // default timeout 25 seconds
  }
  return _rc;
}

void incr_pspref_timeout(void) {
  int _t = get_pspref_timeout();
  switch (_t) {
    case 10:  set_pspref_timeout(15); break;
    case 15:  set_pspref_timeout(25); break;
    case 25:  set_pspref_timeout(30); break;
    case 30:  set_pspref_timeout(45); break;
    case 45:  set_pspref_timeout(60); break;
    case 60:  set_pspref_timeout(90); break;
    case 90:  set_pspref_timeout(120); break;
    case 120: set_pspref_timeout(10); break;
    default:  set_pspref_timeout(25); break;
  }
}

void decr_pspref_timeout(void) {
  int _t = get_pspref_timeout();
  switch (_t) {
    case 10:  set_pspref_timeout(120); break;
    case 15:  set_pspref_timeout(10); break;
    case 25:  set_pspref_timeout(15); break;
    case 30:  set_pspref_timeout(25); break;
    case 45:  set_pspref_timeout(30); break;
    case 60:  set_pspref_timeout(45); break;
    case 90:  set_pspref_timeout(60); break;
    case 120: set_pspref_timeout(90); break;
    default:  set_pspref_timeout(25); break;
  }
}

void set_pspref_activity_done(int _pspref_current_activity_index, bool is_done) {
  char key[8];
  snprintf(key, sizeof(key), "ps_a_%d", _pspref_current_activity_index);
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RW_MODE);
  psPrefs.putBool(key, is_done);
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RO_MODE);
}

bool get_pspref_activity_done(int _pspref_current_activity_index) {
  char key[8];
  snprintf(key, sizeof(key), "ps_a_%d", _pspref_current_activity_index);
  return psPrefs.getBool(key, false);
}
