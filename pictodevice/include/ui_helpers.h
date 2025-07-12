#pragma once
#include <Arduino.h>
#include <PNGdec.h>

void drawSplash();
void drawPicto(String _strname);
void drawName(String _strname, int _marked_done);
void drawTime();
void drawUserName();
void drawBatt();
void drawMarkedDone();
void pngDraw(PNGDRAW *pDraw);
