#pragma once
#include <Preferences.h>

#define PS_RW_MODE false
#define PS_RO_MODE true

extern Preferences psPrefs;  // preferences

// Namespace
extern const char *PSNS;

extern int pspref_brightness;
extern int pspref_current_activity_index;

extern int pspref_timeout;
extern String pspref_timezone;

void set_pspref_buzzer(bool pspref_buzzer);
bool get_pspref_buzzer();
void set_pspref_brightness(int pspref_brightness);
int get_pspref_brightness();
void incr_pspref_brightness();
void decr_pspref_brightness();
void set_pspref_current_activity_index(int pspref_current_activity_index);
int get_pspref_current_activity_index();

void set_pspref_timeout(int pspref_timeout);
int get_pspref_timeout();
void incr_pspref_timeout();
void decr_pspref_timeout();

void set_pspref_timezone(String pspref_timezone);
String get_pspref_timezone();

void set_pspref_color_theme(int pspref_color_theme);
int get_pspref_color_theme();
String get_pspref_color_theme_by_name();
void incr_pspref_color_theme();
void decr_pspref_color_theme();

// reserve for 20 activities max
void set_pspref_activity_done(int pspref_current_activity_index, bool is_done);
bool get_pspref_activity_done(int _pspref_current_activity_index);
