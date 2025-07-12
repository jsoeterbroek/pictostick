#include <Arduino.h>
#include <PNGdec.h>
#include <SPIFFS.h>

// Here are the callback functions that the decPNG library
// will use to open files, fetch data and close the file.

#pragma once

#include <FS.h>
#include <SPIFFS.h>
#include <PNGdec.h>

#if !defined(PNG_SPIFFS_SUPPORT_H)
#define PNG_SPIFFS_SUPPORT_H

using namespace fs;

extern File pngfile;

// Function to open a PNG file from SPIFFS
void* pngOpen(const char* filename, int32_t* size);

// Function to close the PNG file
void pngClose(void* handle);

// Function to read a block of data from the PNG file
int32_t pngRead(PNGFILE* page, uint8_t* buffer, int32_t length);

// Function to seek to a position in the PNG file
int32_t pngSeek(PNGFILE* page, int32_t position);

#endif // PNG_SPIFFS_SUPPORT_H