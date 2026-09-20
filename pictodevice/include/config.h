// ABOUTME: Configuration constants for PictoStick device
// ABOUTME: Separates compile-time configuration from runtime state

#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>

// =====================================================
// Screen Configuration
// =====================================================
// 135 x 240 pixels wordt aangestuurd door de ST7789V2.
// NOTE: LCD_HEIGHT and LCD_WIDTH are switched around
// in this case m5stcik: LCD_HEIGHT=240 and LCD_WIDTH=135
// because the screen is rotated!
#define MY_WIDTH      TFT_HEIGHT
#define MY_HEIGHT     TFT_WIDTH

// =====================================================
// SPIFFS Configuration
// =====================================================
#define FORMAT_SPIFFS_IF_FAILED true

// =====================================================
// NTP Configuration
// =====================================================
#define NTP_TIMEZONE          "UTC+1"

// =====================================================
// Version Information
// =====================================================
#define PD_VERSION_MAJOR      1
#define PD_VERSION_MINOR      6
#define PD_VERSION_PATCH      1

// =====================================================
// Button States
// =====================================================
#define BUTTON_PRESSED        LOW
#define BUTTON_RELEASED       HIGH

// =====================================================
// Image Configuration
// =====================================================
#define MAX_IMAGE_WIDTH       100  // Adjust for your images

#endif // CONFIG_H_
