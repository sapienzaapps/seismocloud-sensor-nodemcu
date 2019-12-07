SeismoCloud project: http://www.seismocloud.com

[![Build Status](https://travis-ci.org/sapienzaapps/seismoclouddevice-nodemcu.svg?branch=master)](https://travis-ci.org/sapienzaapps/seismoclouddevice-nodemcu)

# Supported boards

* NodeMCU 1.0 devkit (with ESP-12E module - ESP8266)

# Network requirements

If you have any firewall in your network, please allow these ports (outgoing, to internet):

* TCP: 80, 443, 1883

Also, Wi-Fi or Ethernet networks should have a DHCP service running.

# LED status description

LEDs can be in these different states:

* **Green**: device is ready
* **Green + Yellow**: device is ready but there is an issue connecting to SeismoCloud APIs
* **Green + Red (only for about 5 seconds)**: shake detected
* **Green + Yellow + Red**: software is loading
* **Green + Yellow + Red - ALL blinking fast**: software is loaded, starting accelerometer
* **Green + Yellow + Red - ALL blinking slow**: network init failed
* **Yellow + Red - ALL blinking**: EEPROM failed

# How to build the device (hardware)

## Requirements

* Wi-Fi network (WPA-PSK, WPA2-PSK, open)
* Arduino/Genuino IDE with ESP8266 sdk installed. If you haven't ESP8266 sdk:
	* Open *Preferences* window (from *File* menu)
	* Enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json` into *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.
	* Close with "OK", open *Boards Manager* from *Tools* > *Board* menu and install *esp8266* platform (and don't forget to select *NodeMCU 1.0 (ESP-12E)* board from *Tools* > *Board* menu after installation).
* Arduino/Genuino IDE libraries: `WiFiManager, PubSubClient`
	* Open *Include Libraries* from *Sketch* menu, and choose *Manage Libraries*
	* Type `WiFiManager` into search bar, then install it by clicking on *Install* button below; the same for `PubSubClient`
	* Close window
* NodeMCU 1.0 devkit board with ESP-12E module
* MPU6050 Accelerometer
* (optional) 3 LEDs (red-green-yellow) with 3 resistors

Tested with `Arduino/Genuino IDE 1.8.10`, board SDK `esp8266 2.6.2`, libraries `PubSubClient 2.7` and `WiFiManager 0.15.0-beta`

## Step 1: Accelerometer MPU6050

Link these pins from Accelerometer MPU6050 to NodeMCU board:

* 3v3: 3v3
* GND: GND
* SDA: D1
* SCL: D2

## Step 2: (optional) LEDs

Remember to put a resistor with LED (after/before is not really important) to limit
current flowing, otherwise you may damage the NodeMCU board.

By default, LED pins are:

* Pin D5 : Green
* Pin D6 : Yellow
* Pin D7 : Red

# How to build/upload the software (step 3)

For NodeMCU, you need to download `WifiManager` library (by using *Sketch > include library > library manager*)

1. Download the source code (for stable releases, please checkout latest git tag)
2. Open project in Arduino IDE
3. Choose the right **Port** and **Board** into **Tools** menu (if you're unsure, use `NodeMCU 1.0`)
4. Compile and upload (2nd button below menus) in your board
5. Connect to `SeismoCloud` Wi-Fi network and configure Wi-Fi client network parameters. On save, the board reboots and will try to connect to Wi-Fi network. If it fails, you can reconnect to `SeismoCloud` network and modify/fix network parameters.
6. Open SeismoCloud app, connect to the same network of the board and register your device. Enjoy!

# License

See `LICENSE` file
