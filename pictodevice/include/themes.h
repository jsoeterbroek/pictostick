// ABOUTME: This file defines the color themes for the PictoStick device.
// ABOUTME: It contains the structures and definitions for different color schemes.

#ifndef THEMES_H
#define THEMES_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "colors.h"

// Enum for theme selection
enum ThemeName {
  THEME_DEFAULT,
  THEME_DARK,
  THEME_BLUE
};

// Struct to hold all theme colors
struct Theme {
  uint16_t textColor;
  uint16_t fgColor;
  uint16_t bgColor;
  uint16_t lineColor;
  uint16_t rectBgColor;
  uint16_t warnNok;
  uint16_t warnOk;
  uint16_t dayPeriodCircleBgColor;
  uint16_t dayPeriodCircleFgColor;
  uint16_t rightRectBgColor1;
  uint16_t rightRectTextColor1;
  uint16_t rightRectBgColor2;
  uint16_t rightRectTextColor2;
  uint16_t colorTodo;
  uint16_t colorDone;
};

// Array of themes
static Theme themes[] = {
  // THEME_DEFAULT
  {
    TFT_DARKGREY,            // textColor
    TFT_WHITE,               // fgColor
    RGB565_GRAY_LIGHT,       // bgColor
    TFT_DARKGREY,            // lineColor
    TFT_BLACK,               // rectBgColor
    TFT_ORANGE,              // warnNok
    TFT_GREEN,               // warnOk
    RGB565_GRAY_BATTLESHIP,  // dayPeriodCircleBgColor
    TFT_RED,                 //  dayPeriodCircleFgColor
    RGB565_GRAY_STONE,       // rightRectBgColor1
    RGB565_GRAY_LIGHT,       // rightRectTextColor1
    RGB565_GRAY_LIGHT,       // rightRectBgColor2
    RGB565_GRAY_STONE,       // rightRectTextColor2
    RGB565_CORAL,            // colorTodo
    RGB565_GREEN_DARKSEA     // colorDone
  },
  // THEME_DARK
  {
    TFT_WHITE,                 // textColor
    TFT_BLACK,                 // fgColor
    RGB565_BLACK_OUTER_SPACE,  // bgColor
    TFT_WHITE,                 // lineColor
    TFT_WHITE,                 // rectBgColor
    RGB565_ORANGE_CADMIUM,     // warnNok
    RGB565_GREEN_GO,           // warnOk
    RGB565_GRAY_WEB,           // dayPeriodCircleBgColor
    RGB565_YELLOW,             // dayPeriodCircleFgColor
    RGB565_BLACK_DIM,          // rightRectBgColor1
    TFT_WHITE,                 // rightRectTextColor1
    RGB565_GRAY_MEDIUM,        // rightRectBgColor2
    TFT_BLACK,                 // rightRectTextColor2
    RGB565_RED_BRICK,          // colorTodo
    RGB565_GREEN_FERN          // colorDone
  },
  // THEME_BLUE
  {
    RGB565_BLUE_AZURE,       // textColor
    TFT_WHITE,               // fgColor
    RGB565_BLUE_LIGHT,       // bgColor
    RGB565_BLUE_AZURE,       // lineColor
    TFT_BLACK,               // rectBgColor
    RGB565_ORANGE,           // warnNok
    RGB565_GREEN_GO,         // warnOk
    RGB565_GRAY_BATTLESHIP,  // dayPeriodCircleBgColor
    RGB565_YELLOW,           // dayPeriodCircleFgColor
    RGB565_BLUE_AZURE,       //  rightRectBgColor1
    TFT_WHITE,               // rightRectTextColor1
    RGB565_BLUE_LIGHT,       // rightRectBgColor2
    RGB565_BLUE_AZURE,       // rightRectTextColor2
    RGB565_RED_MAXIMUM,      // colorTodo
    RGB565_GREEN_DARKSEA     // colorDone
  }
};

// Global variable for the current theme
extern Theme currentTheme;

// Function to set the current theme
void setTheme(ThemeName themeName);

#endif  // THEMES_H
