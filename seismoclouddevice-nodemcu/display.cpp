#include "common.h"
#include "logo.h"

// TODO: implement also SSD1306 with a conditional preprocessor directive

// I2C code for SH1106 128x64 oled

// 0X3C+SA0 - 0x3C or 0x3D
#define DISPLAY_I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled;

#define COLUMNADDR 0x21
#define PAGEADDR 0x22
#define SETSTARTLINE 0x40

void sendCommand(unsigned char com) {
  Wire.beginTransmission(DISPLAY_I2C_ADDRESS);      //begin transmitting
  Wire.write(0x80);                          //command mode
  Wire.write(com);
  Wire.endTransmission();                    // stop transmitting
}

void displayInit() {
  oled.begin(&SH1106_128x64, DISPLAY_I2C_ADDRESS);
  oled.setFont(Adafruit5x7);

  uint32_t m = micros();
  oled.clear();

  // Display SeismoCloud logo!
  // See X BitMap format here:
  // https://en.wikipedia.org/wiki/X_BitMap
  // Use convert seismocloud.bmp seismocloud.xbm

  // The SSD1306/SH1106 manages the monitor as follow:
  // * each bit represent pixel ON or OFF
  // * each byte represent 8 pixel vertically aligned in that row
  // * each row is an horizontal vector of bytes (vertically aligned 8 pixel sequences)
  // So the monitor has 64 pixel but 8 rows (as each row is actually 8 vertical bits)

  // The difference between SSD1306 and SH1106 is the buffer/RAM size:
  // * SH1106: 132x64
  // * SSD1306: 128x64
  // The image seems centered, so there are 2 columns lost on left and 2 on right border
  // See https://forum.arduino.cc/index.php?PHPSESSID=hjbh8nbi990tntsts1d57e98s5&topic=256374.msg1814470#msg1814470

  // The X BitMap stores the same information BUT horizontally aligned (a byte represents 8 columns)
  // So, a rotation is necessary to directly draw an X BitMap image to this screen

  int i, j;
  for (j = 0; j < 64; j++) {
    oled.setCursor(0, j);
    for (i = 0; i < 128; i++) {
      oled.ssd1306WriteRamBuf(seismocloud_bits[i+(j*128)]);
    }
    delay(1);
  }
  oled.setCursor(0, 0);
  delay(5000);
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
