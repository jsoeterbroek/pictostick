#include "ui.h"
#include <Arduino.h>
#include "global_flags.h"

// void drawSplash() {
//   sprite.createSprite(MY_WIDTH, MY_HEIGHT);
//   sprite.fillSprite(TFT_WHITE);
//   sprite.loadFont(NotoSansBold15);
//   sprite.setTextColor(TFT_DARKGRAY, TFT_WHITE);
//   String software = " PictoStick ";
//   // software += String("v") + pd_version_major() + "." +
//   // pd_version_minor() +
//   // "." + pd_version_patch();
//   software += String("v") + pd_version_major() + "." + pd_version_minor() + " PROTOTYPE";
//   String maker = " Joost Soeterbroek";
//   String maker_email = " <joost.soeterbroek@gmail.com>";
//   String code = " github.com/jsoeterbroek/pictostick";

//   sprite.drawString(software, 4, 24); 
//   sprite.unloadFont();
//   sprite.loadFont(smallFont);
//   sprite.drawString(maker, 4, 52); 
//   sprite.drawString(maker_email, 4, 72); 
//   sprite.drawString(code, 4, 92); 
//   sprite.unloadFont();
//   StickCP2.Display.pushImage(0, 0, MY_WIDTH, MY_HEIGHT, (uint16_t *)sprite.getPointer());
//   delay(10000);
// }
