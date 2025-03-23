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

// SCREEN: ui_ScreenMAIN
void ui_ScreenMAIN_screen_init(void);
extern lv_obj_t * ui_ScreenMAIN;
extern lv_obj_t * ui_btimeslotMorningLBL;
extern lv_obj_t * ui_btimeslotAfternoonLBL;
extern lv_obj_t * ui_btimeslotEveningLBL;
extern lv_obj_t * ui_nowPNL;
extern lv_obj_t * ui_prevPNL;
extern lv_obj_t * ui_nextPNL;
// CUSTOM VARIABLES

// SCREEN: ui_ScreenSPLASH
void ui_ScreenSPLASH_screen_init(void);
extern lv_obj_t * ui_ScreenSPLASH;
extern lv_obj_t * ui_Image5;
// CUSTOM VARIABLES

// EVENTS
extern lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
LV_IMG_DECLARE(ui_img_logo_cropped_png);    // assets/logo_cropped.png

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
