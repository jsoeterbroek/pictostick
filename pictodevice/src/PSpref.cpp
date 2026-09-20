#include "PSpref.h"
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

Preferences psPrefs;  // preferences

int pspref_current_activity_index = 0;
int pspref_timeout = 25;
// reserve for 20 activities max
bool pspref_activity_done = false;

const char *PSNS = "PSPrefs";

// Ordered stepped value levels. The stored preference holds one of these values
// and the incr/decr helpers step through them.
static const int BRIGHTNESS_LEVELS[] = {16, 32, 48, 64, 96, 132, 164, 180};
static const int TIMEOUT_LEVELS[] = {10, 15, 25, 30, 45, 60, 90, 120};

static const int BRIGHTNESS_LEVELS_COUNT = sizeof(BRIGHTNESS_LEVELS) / sizeof(BRIGHTNESS_LEVELS[0]);
static const int TIMEOUT_LEVELS_COUNT = sizeof(TIMEOUT_LEVELS) / sizeof(TIMEOUT_LEVELS[0]);

// Fallback used when the stored preference is not one of the known levels.
// These mirror the values the original per-value switch statements returned
// from their default branches.
static const int BRIGHTNESS_INCR_FALLBACK = 32;
static const int BRIGHTNESS_DECR_FALLBACK = 61;
static const int TIMEOUT_FALLBACK = 25;

// Return the index of _value in the level table, or -1 when it is not a level.
static int levelIndex(const int *levels, int count, int _value) {
  for (int i = 0; i < count; i++) {
    if (levels[i] == _value) {
      return i;
    }
  }
  return -1;
}

// Step to the next level, wrapping from the highest back to the lowest.
static int nextLevel(const int *levels, int count, int fallback, int _value) {
  int i = levelIndex(levels, count, _value);
  if (i < 0) {
    return fallback;
  }
  return levels[(i + 1) % count];
}

// Step to the previous level, stopping at the lowest level.
static int previousLevel(const int *levels, int count, int fallback, int _value) {
  int i = levelIndex(levels, count, _value);
  if (i < 0) {
    return fallback;
  }
  if (i == 0) {
    return levels[0];
  }
  return levels[i - 1];
}

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
  set_pspref_brightness(nextLevel(BRIGHTNESS_LEVELS, BRIGHTNESS_LEVELS_COUNT, BRIGHTNESS_INCR_FALLBACK,
                                  get_pspref_brightness()));
}

void decr_pspref_brightness(void) {
  set_pspref_brightness(previousLevel(BRIGHTNESS_LEVELS, BRIGHTNESS_LEVELS_COUNT, BRIGHTNESS_DECR_FALLBACK,
                                      get_pspref_brightness()));
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
  set_pspref_timeout(nextLevel(TIMEOUT_LEVELS, TIMEOUT_LEVELS_COUNT, TIMEOUT_FALLBACK,
                               get_pspref_timeout()));
}

void decr_pspref_timeout(void) {
  set_pspref_timeout(previousLevel(TIMEOUT_LEVELS, TIMEOUT_LEVELS_COUNT, TIMEOUT_FALLBACK,
                                   get_pspref_timeout()));
}

void set_pspref_activity_done(String dayName, int _pspref_current_activity_index, bool is_done) {
  char key[16]; // Increased size to accommodate day name
  snprintf(key, sizeof(key), "ps_a_%s_%d", dayName.c_str(), _pspref_current_activity_index);
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RW_MODE);
  psPrefs.putBool(key, is_done);
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RO_MODE);
}

bool get_pspref_activity_done(String dayName, int _pspref_current_activity_index) {
  char key[16]; // Increased size to accommodate day name
  snprintf(key, sizeof(key), "ps_a_%s_%d", dayName.c_str(), _pspref_current_activity_index);
  return psPrefs.getBool(key, false);
}

void set_pspref_all_activity_undone(void) {
  String daysOfWeek[] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};

  for (int d = 0; d < 7; d++) {
    String dayName = daysOfWeek[d];
    String filename = "/data_" + dayName + ".json";

    File file = SPIFFS.open(filename, FILE_READ);
    if (!file) {
      Serial.printf("Failed to open %s for reading, skipping\n", filename.c_str());
      continue;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
      Serial.printf("deserializeJson() failed for %s: %s\n", filename.c_str(), error.c_str());
      continue;
    }

    JsonArray activities = doc["activities"].as<JsonArray>();
    for (JsonObject activity : activities) {
      activity["done"] = false;
    }

    file = SPIFFS.open(filename, FILE_WRITE);
    if (!file) {
      Serial.printf("Failed to open %s for writing\n", filename.c_str());
      continue;
    }

    if (serializeJson(doc, file) == 0) {
      Serial.printf("Failed to write to file %s\n", filename.c_str());
    } else {
      Serial.printf("Activities in %s reset to undone.\n", filename.c_str());
    }
    file.close();
  }
  // No need to use psPrefs.begin/end here as we are directly manipulating files
}

void set_pspref_timezone(String _pspref_timezone) {
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RW_MODE);
  psPrefs.putString("ps_tz", _pspref_timezone);
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RO_MODE);
}

String get_pspref_timezone(void) {
  String _rc = "";
  _rc = psPrefs.getString("ps_tz");
  if (_rc == "") {
    set_pspref_timezone("CET-1CEST,M3.5.0,M10.5.0/3");
    _rc = "CET-1CEST,M3.5.0,M10.5.0/3";  // default timezone: Europe/Amsterdam
  }
  return _rc;
}

void set_pspref_color_theme(int _pspref_color_theme) {
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RW_MODE);
  psPrefs.putInt("ps_ct", _pspref_color_theme);
  psPrefs.end();
  psPrefs.begin(PSNS, PS_RO_MODE);
}

int get_pspref_color_theme(void) {
  int _rc = 0;
  _rc = psPrefs.getInt("ps_ct");
  return _rc;
}

String get_pspref_color_theme_by_name(void) {
  String _theme_name;
  int _rc = 0;
  _rc = psPrefs.getInt("ps_ct");
  if (_rc == 1) {
    _theme_name = "DARK";
  } else if (_rc == 2) {
    _theme_name = "BLUE";
  } else {
    _theme_name = "DEFAULT";
  }
  return _theme_name;
}

void incr_pspref_color_theme(void) {
  int _ct = get_pspref_color_theme();
  if (_ct >= 2) {
    _ct = 0;
  } else {
    _ct++;
  }
  set_pspref_color_theme(_ct);
}

void decr_pspref_color_theme(void) {
  int _ct = get_pspref_color_theme();
  if (_ct <= 0) {
    _ct = 2;
  } else {
    _ct--;
  }
  set_pspref_color_theme(_ct);
}
