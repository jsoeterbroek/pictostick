// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: pictodevice_ui

#ifndef _PICTODEVICE_UI_UI_H
#define _PICTODEVICE_UI_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_ScreenMAIN240x135
void ui_ScreenMAIN240x135_screen_init(void);
extern lv_obj_t * ui_ScreenMAIN240x135;
// CUSTOM VARIABLES

// SCREEN: ui_ScreenMAIN240x320
void ui_ScreenMAIN240x320_screen_init(void);
extern lv_obj_t * ui_ScreenMAIN240x320;
// CUSTOM VARIABLES

// EVENTS
extern lv_obj_t * ui____initial_actions0;

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
