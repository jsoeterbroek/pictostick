// ABOUTME: This file implements the theme management for the PictoStick device.
// ABOUTME: It allows setting and applying different color themes.

#include "themes.h"

// Global variable for the current theme
Theme currentTheme;

// Function to set the current theme
void setTheme(ThemeName themeName) {
    currentTheme = themes[themeName];
}
