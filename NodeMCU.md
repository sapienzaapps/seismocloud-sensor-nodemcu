# Hardware

## Requirements

* Arduino/Genuino IDE with ESP8266 sdk installed. If you haven't ESP8266 sdk:
	* Open *Preferences* window (from *File* menu)
	* Enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json` into *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.
	* Close with "OK", open *Boards Manager* from *Tools* > *Board* menu and install *esp8266* platform (and don't forget to select *NodeMCU 1.0 (ESP-12E)* board from *Tools* > *Board* menu after installation).
* NodeMCU 1.0 devkit board with ESP-12E module
* MPU6050 Accelerometer
* (optional) 3 LEDs (red-green-yellow) with 3 resistors

## Accelerometer MPU6050

Link these pins from Accelerometer MPU6050 to NodeMCU board:

* 3v3: 3v3
* GND: GND
* SDA: D1
* SCL: D2

## (optional) LEDs

Remember to put a resistor with LED (after/before is not really important) to limit
current flowing, otherwise you may damage the NodeMCU board.

By default, LED pins are:

* Pin D5 : Green
* Pin D6 : Yellow
* Pin D7 : Red

# Software

## [NodeMCU] Build and upload code

For NodeMCU, you need to download `WifiManager` library

1. Download the source code (for stable releases, please checkout latest git tag)
2. Open project in Arduino IDE
3. Choose the right **Port** and **Board** into **Tools** menu
4. Compile and upload (2nd button below menus) in your board
5. Unplug the board from PC and plug Accelerometer and Leds, then power on the board
6. For NodeMCU boards, connect to `SeismoCloud` Wi-Fi network and configure Wi-Fi client network parameters. On save, the board reboots and try to connect to Wi-Fi network. If it fails, you can reconnect to `SeismoCloud` network and modify/fix network parameters.
7. Open SeismoCloud app, connect to the same network of the board and register your device. Enjoy!
