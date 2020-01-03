SeismoCloud project: http://www.seismocloud.com

For 🇮🇹 Italian, see (README.ita.md)[README.ita.md]

[![Build Status](https://travis-ci.org/sapienzaapps/seismoclouddevice-nodemcu.svg?branch=master)](https://travis-ci.org/sapienzaapps/seismoclouddevice-nodemcu)

# Supported boards

* NodeMCU 1.0 devkit (with ESP-12E module - ESP8266)

# Network requirements

The device will acquire all IPv4 configurations (address, netmask, gateway, DNS) via DHCP.

If you have any firewall in your network, please allow these ports (outgoing, to internet):

* TCP: 443, 1883

# LED status description

LEDs can be in these different states (empty cells means "OFF"):

| Green | Yellow | Red  | Status
|:-----:|:------:|:----:| ------
| ON    |        |      | Device is idle and listening
| ON    | ON     |      | The device has lost its connection, and it's trying to reconnect
| ON    |        | ON   | A vibration/shake is detected!
| ON    | ON     | ON   | The device is connecting to the SeismoCloud network
|       | ON     | ON   | Checking for updates or updating
|       | ON     |      | Connecting to Wi-Fi network*
|       |        | ON   | Calibration in progress

At the end of the boot sequence all three LEDs will blink rapidly to signal that it's OK.

*: If only the yellow LED is ON for more than 10 seconds then the NodeMCU is waiting for
Wi-Fi network configuration (see the chapter "How to build/upload the software")

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

## Wiring the Accelerometer MPU6050

Link these pins from Accelerometer MPU6050 to NodeMCU board:

* 3v3: 3v3
* GND: GND
* SDA: D1
* SCL: D2

## Wiring LEDs

Remember to put a resistor with LED (after/before is not really important) to limit
current flowing, otherwise you may damage the NodeMCU board.

By default, LED pins are:

* Pin D5 : Green
* Pin D6 : Yellow
* Pin D7 : Red

# How to build/upload the software

For NodeMCU, you need to download `WifiManager` library (by using *Sketch > include library > library manager*)

1. Download the source code (for stable releases, please checkout latest git tag)
2. Open project in Arduino IDE
3. Choose the right **Port** and **Board** into **Tools** menu (if you're unsure, use `NodeMCU 1.0`)
4. Compile and upload (2nd button below menus) in your board
5. Connect to `SeismoCloud` Wi-Fi network and configure Wi-Fi client network parameters. On save, the board reboots and will try to connect to Wi-Fi network. If it fails, you can reconnect to `SeismoCloud` network and modify/fix network parameters.
6. Open SeismoCloud app, connect to the same network of the board and register your device. Enjoy!

# FAQ

## My device has only the yellow LED powered on for more than 10 seconds

The sensor failed to connect to the Wi-Fi network. Please follow the instruction in chapter "How to build/upload the software" step 5

## I'm connected to the Wi-Fi network, but no configuration portal appears.

Try to open a browser and navigate to http://192.168.4.1 . If the Wi-Fi portal still not showing, disconnect the board from
the power source for few seconds. If it still fails, flash again the board erasing the Wi-Fi configuration using the option
from the **Tools** menu.

# License

See `LICENSE` file
