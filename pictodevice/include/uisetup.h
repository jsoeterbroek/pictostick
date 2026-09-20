// ABOUTME: This file contains declarations for the setup user interface.
// ABOUTME: These functions are responsible for drawing the different device mode screens.
#pragma once
#include <stdint.h>

void drawDeviceMode1();
void drawDeviceMode2();
void drawDeviceMode3();

// Day index validation
int validateDayIndex(int dayIndex);

// Activity functions
void initDefaultActivity();
bool getActivityFromDoc(const char* filename, int* activityIndex, int* activityCount);

// Time validation
bool validateTime(int hour, int minute);

// Internal helpers
void initDeviceMode3();
