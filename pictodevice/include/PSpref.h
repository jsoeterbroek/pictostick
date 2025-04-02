#pragma once
#include <Preferences.h>

#define PS_RW_MODE false
#define PS_RO_MODE true

extern Preferences psPrefs; // preferences

// Namespace
extern const char* PSNS;

extern int pspref_current_activity_index;

void set_pspref_current_activity_index(int pspref_current_activity_index);
int get_pspref_current_activity_index();

// reserve for 20 activities max
void set_pspref_activity_done(int pspref_current_activity_index);
void set_pspref_activity_undone(int pspref_current_activity_index);
int get_pspref_activity_done(int _pspref_current_activity_index);