#include "ui_helpers.h"
#include <M5StickCPlus2.h>
#include <TFT_eSPI.h>
#include "common.h"
#include "NotoSansBold15.h"
#include "smallFont.h"
#include <PNGdec.h>
#include "PNG_SPIFFS_Support.h"

File pngfile;

extern TFT_eSprite sprite;
extern int16_t xpos;
extern int16_t ypos;
extern String lang;
extern int vol;
extern int volE;
extern const char *config_name;
extern PNG png;

static uint32_t last_batt_update = 0;
static int last_batt_percent = -1;

void *pngOpen(const char *filename, int32_t *size) {
  pngfile = SPIFFS.open(filename, "r");
  *size = pngfile.size();
  return &pngfile;
}

void pngClose(void *handle) {
  File pngfile = *((File *)handle);
  if (pngfile) {
    pngfile.close();
  }
}

int32_t pngRead(PNGFILE *page, uint8_t *buffer, int32_t length) {
  if (!pngfile) {
    return 0;
  }
  return pngfile.read(buffer, length);
}

int32_t pngSeek(PNGFILE *page, int32_t position) {
  if (!pngfile) {
    return 0;
  }
  return pngfile.seek(position);
}

void drawSplash() {
  sprite.createSprite(MY_WIDTH, MY_HEIGHT);
  sprite.fillSprite(TFT_WHITE);
  sprite.loadFont(NotoSansBold15);
  sprite.setTextColor(TFT_DARKGRAY, TFT_WHITE);
  String software = " PictoStick ";
  software += String("v") + pd_version_major() + "." + pd_version_minor() + " PROTOTYPE";
  String maker = " Joost Soeterbroek";
  String maker_email = " <joost.soeterbroek@gmail.com>";
  String code = " github.com/jsoeterbroek/pictostick";

  sprite.drawString(software, 4, 24);
  sprite.unloadFont();
  sprite.loadFont(smallFont);
  sprite.drawString(maker, 4, 52);
  sprite.drawString(maker_email, 4, 72);
  sprite.drawString(code, 4, 92);
  sprite.unloadFont();
  StickCP2.Display.pushImage(0, 0, MY_WIDTH, MY_HEIGHT, (uint16_t *)sprite.getPointer());
  delay(10000);
}

void drawPicto(String _strname) {
  _strname = "/picto/" + _strname;
  int16_t rc = png.open(_strname.c_str(), pngOpen, pngClose, pngRead, pngSeek, pngDraw);
  if (rc == PNG_SUCCESS) {
    sprite.startWrite();
    rc = png.decode(NULL, 0);
    png.close();
    sprite.endWrite();
  }
}

void drawBatt() {
  int batteryPercent;
  bool isCharging;
  unsigned long currentMillis = millis();

  if (currentMillis - last_batt_update > 60000 || last_batt_percent == -1) {
    last_batt_update = currentMillis;
    vol = StickCP2.Power.getBatteryVoltage();
    isCharging = StickCP2.Power.isCharging();  // Seems to be not working...
    StickCP2.Power.isCharging();

    batteryPercent = map(vol, 3000, 4200, 0, 100);
    if (batteryPercent > 100) {
      batteryPercent = 100;
    }
    if (batteryPercent < 0) {
      batteryPercent = 0;
    }
    last_batt_percent = batteryPercent;
  } else {
    batteryPercent = last_batt_percent;
  }

  volE = map(vol, 3000, 4180, 0, 5);

  //Serial.print("DEBUG: Battery charging ");
  //Serial.println(isCharging);

  sprite.fillRect(116, 0, 120, 20, RIGHT_RECT_BG_COLOR_2);
  if (isCharging) {
    sprite.setTextColor(RGB565_GREEN_DARKSEA, RIGHT_RECT_BG_COLOR_2);
  } else {
    sprite.setTextColor(RIGHT_RECT_TEXT_COLOR_2, RIGHT_RECT_BG_COLOR_2);
  }
  sprite.loadFont(NotoSansBold15);
  sprite.setCursor(160, 3);
  sprite.printf("%d%%", batteryPercent);
  sprite.drawRect(200, 3, 28, 14, RGB565_GRAY_STONE);
  sprite.fillRect(229, 7, 3, 6, RGB565_GRAY_STONE);
  for (int i = 0; i < volE; i++) {
    sprite.fillRect(222 - (i * 5), 5, 3, 10, RGB565_GRAY_STONE);
  }
  sprite.unloadFont();
}

void drawUserName() {
  sprite.loadFont(NotoSansBold15);
  sprite.fillRect(116, 20, 120, 20, RIGHT_RECT_BG_COLOR_1);
  sprite.setTextColor(RIGHT_RECT_TEXT_COLOR_1, RIGHT_RECT_BG_COLOR_1);
  sprite.drawString(config_name, 118, 24);
  sprite.unloadFont();
}

void drawTime() {
  struct tm timeinfo;
  static constexpr const char *const wd_en[7] = {"Sunday", "Monday", "Tuesday", "Wedn", "Thursday", "Friday", "Saturday"};
  static constexpr const char *const wd_nl[7] = {"Zondag", "Maandag", "Dinsdag", "Woensdag", "Donderdag", "Vrijdag", "Zaterdag"};
  auto dt = StickCP2.Rtc.getDateTime();
  auto t = time(nullptr);
  auto tm = localtime(&t);
  char timebuffer[30];
  char daybuffer[30];
  if (lang.equals("en")) {
    snprintf(daybuffer, sizeof(daybuffer), "%s", wd_en[tm->tm_wday]);
  } else {
    snprintf(daybuffer, sizeof(daybuffer), "%s", wd_nl[tm->tm_wday]);
  }
  snprintf(timebuffer, sizeof(timebuffer), "%02d:%02d", dt.time.hours, dt.time.minutes);

  sprite.loadFont(NotoSansBold15);
  sprite.fillRect(116, 40, 120, 20, RIGHT_RECT_BG_COLOR_2);
  sprite.setTextColor(RIGHT_RECT_TEXT_COLOR_2, RIGHT_RECT_BG_COLOR_2);
  sprite.drawString(daybuffer, 118, 44);
  sprite.drawString(timebuffer, 196, 44);
  sprite.unloadFont();
}

void drawName(String _strname, int _marked_done) {
  sprite.loadFont(NotoSansBold15);
  if (_marked_done == 1) {
    sprite.fillRect(116, 70, 120, 40, COLOR_DONE);
    sprite.setTextColor(RIGHT_RECT_TEXT_COLOR_1, COLOR_DONE);
  } else {
    sprite.fillRect(116, 70, 120, 40, COLOR_TODO);
    sprite.setTextColor(RIGHT_RECT_TEXT_COLOR_1, COLOR_TODO);
  }
  sprite.drawString(_strname, 118, 84);
  sprite.unloadFont();
}

void drawMarkedDone() {
  sprite.loadFont(NotoSansBold15);
  sprite.setTextColor(TFT_BLACK, RIGHT_RECT_BG_COLOR_1);
  sprite.drawString("X", 20, 100);
  sprite.unloadFont();
}
