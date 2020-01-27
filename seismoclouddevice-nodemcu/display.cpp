#include "common.h"

// TODO: implement also SSD1306 with a conditional preprocessor directive

// I2C code for SH1106 128x64 oled

// 0X3C+SA0 - 0x3C or 0x3D
#define DISPLAY_I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled;

void displayInit() {
  oled.begin(&SH1106_128x64, DISPLAY_I2C_ADDRESS);
  oled.setFont(Adafruit5x7);

  uint32_t m = micros();
  oled.clear();
}

void displayOff() {
  oled.clear();
}

void displayQuake() {
	oled.println(WiFi.SSID());
	oled.println(WiFi.RSSI());

	oled.println(WiFi.macAddress());

	oled.println(WiFi.localIP());

	oled.print(F("Sigma: "));
	oled.println(sigmaIter);
}
