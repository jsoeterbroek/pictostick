#include "uiconfig.h"
#include <M5StickCPlus2.h>
#include <TFT_eSPI.h>
#include "common.h"
#include "PSpref.h"
#include "devicemode.h"
#include "NotoSansBold15.h"
#include "_locale.h"

// Forward declarations from main.cpp
extern TFT_eSprite sprite;
extern uint8_t desired_devicemode;
extern int devicemode;
extern bool draw_device_mode_config;

void drawDeviceModeConfig(uint8_t _desired_devicemode) {
  sprite.createSprite(MY_WIDTH, MY_HEIGHT);
  sprite.fillSprite(currentTheme.bgColor);
  sprite.loadFont(NotoSansBold15);
  sprite.setTextColor(currentTheme.textColor, currentTheme.bgColor);
  sprite.drawString(TXT_DM_SELECT, 2, 4);
  devicemode = get_devicemode();

  unsigned short _color1 = currentTheme.textColor;
  unsigned short _color2 = currentTheme.fgColor;

  if (_desired_devicemode == 1) {
    _color1 = currentTheme.warnNok;
    _color2 = currentTheme.textColor;
  } else {
    _color1 = currentTheme.textColor;
    _color2 = currentTheme.fgColor;
  }
  sprite.fillRect(0, 24, 220, 24, _color2);
  sprite.setTextColor(_color1, _color2);
  sprite.drawString(TXT_DM_NET_CONF, 4, 32);

  if (_desired_devicemode == 2) {
    _color1 = currentTheme.warnNok;
    _color2 = currentTheme.textColor;
  } else {
    _color1 = currentTheme.textColor;
    _color2 = currentTheme.fgColor;
  }
  sprite.fillRect(0, 52, 220, 24, _color2);
  sprite.setTextColor(_color1, _color2);
  sprite.drawString(TXT_DM_PICTO_UPD, 4, 60);

  if (_desired_devicemode == 3) {
    _color1 = currentTheme.warnNok;
    _color2 = currentTheme.textColor;
  } else {
    _color1 = currentTheme.textColor;
    _color2 = currentTheme.fgColor;
  }
  sprite.fillRect(0, 80, 220, 24, _color2);
  sprite.setTextColor(_color1, _color2);
  sprite.drawString(TXT_DM_SETUP, 4, 88);

  if (_desired_devicemode == 4) {
    _color1 = currentTheme.warnNok;
    _color2 = currentTheme.textColor;
  } else {
    _color1 = currentTheme.textColor;
    _color2 = currentTheme.fgColor;
  }
  sprite.fillRect(0, 108, 220, 24, _color2);
  sprite.setTextColor(_color1, _color2);
  sprite.drawString(TXT_DM_NORMAL_MODE, 4, 116);

  sprite.unloadFont();
  StickCP2.Display.pushImage(0, 0, MY_WIDTH, MY_HEIGHT, (uint16_t *)sprite.getPointer());

  // button action
  if (StickCP2.BtnPWR.wasPressed()) {
    if (get_pspref_buzzer()) {
      StickCP2.Speaker.tone(6000, 100);
    }
    set_devicemode(4);
  }
  if (StickCP2.BtnB.wasPressed()) {
    if (get_pspref_buzzer()) {
      StickCP2.Speaker.tone(6000, 100);
    }
    switch (desired_devicemode) {
      case 1: desired_devicemode = 2; break;
      case 2: desired_devicemode = 3; break;
      case 3: desired_devicemode = 4; break;
      case 4: desired_devicemode = 1; break;
    }
  }
  if (StickCP2.BtnA.wasPressed()) {
    if (get_pspref_buzzer()) {
      StickCP2.Speaker.tone(6000, 100);
    }
    draw_device_mode_config = false;
    set_devicemode(desired_devicemode);
  }
}
