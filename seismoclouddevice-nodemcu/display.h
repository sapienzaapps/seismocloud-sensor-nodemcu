#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

void displayInit();

void displayOff();

void displayQuake();

extern SSD1306AsciiWire oled;

#endif