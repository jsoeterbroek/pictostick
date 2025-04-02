#pragma once
#include <Preferences.h>

#define PS_RW_MODE false
#define PS_RO_MODE true

extern Preferences psPrefs; // preferences

// Namespace
extern const char* PSNS;

extern int pspref_current_activity_index;
extern bool pspref_activity_done;

void set_pspref_current_activity_index(int pspref_current_activity_index);
uint8_t get_pspref_current_activity_index();
// reserve for 20 activities max
void set_pspref_activity_00_done(bool pspref_activity_done);
void set_pspref_activity_01_done(bool pspref_activity_done);
void set_pspref_activity_02_done(bool pspref_activity_done);
void set_pspref_activity_03_done(bool pspref_activity_done);
void set_pspref_activity_04_done(bool pspref_activity_done);
void set_pspref_activity_05_done(bool pspref_activity_done);
void set_pspref_activity_06_done(bool pspref_activity_done);
void set_pspref_activity_07_done(bool pspref_activity_done);
void set_pspref_activity_08_done(bool pspref_activity_done);
void set_pspref_activity_09_done(bool pspref_activity_done);
void set_pspref_activity_10_done(bool pspref_activity_done);
void set_pspref_activity_11_done(bool pspref_activity_done);
void set_pspref_activity_12_done(bool pspref_activity_done);
void set_pspref_activity_13_done(bool pspref_activity_done);
void set_pspref_activity_14_done(bool pspref_activity_done);
void set_pspref_activity_15_done(bool pspref_activity_done);
void set_pspref_activity_16_done(bool pspref_activity_done);
void set_pspref_activity_17_done(bool pspref_activity_done);
void set_pspref_activity_18_done(bool pspref_activity_done);
void set_pspref_activity_19_done(bool pspref_activity_done);

bool get_pspref_activity_00_done();