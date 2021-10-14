SeismoCloud project: http://www.seismocloud.com

For 🇮🇹 Italian, [please follow this page](https://www.seismocloud.com/il-sismometro/).

Want to install a new sensor? [All instructions in this page!](https://www.seismocloud.com/nodemcu-instructions/)

# SeismoCloud

SeismoCloud is a community network about the tracking of seismism whose goal is an Early Warning, i.e. an immediate alert to the citizens of an earthquake area. The network works with low-cost seismometers (physical devices or smartphone sensors) stored in a cloud that receives and analyzes informations from seisometers, decide if an earthquake is taking place and send an Early Warning to the provinces potentially affected

This repository contains the code for ESP8266-compatible boards. [If you want to build a new sensor, you can find the complete guide at this link.](https://www.seismocloud.com/nodemcu-instructions/)

# Supported boards

* NodeMCU 1.0 devkit (with ESP-12E module - ESP8266)

# Network requirements

The device will acquire all IPv4 configurations (address, netmask, gateway, DNS) via DHCP.

If you have any firewall in your network, please allow these ports (outgoing, to internet):

* TCP: 443

# LED status description

LEDs can be in these different states (empty cells means "OFF"):

| Green | Yellow | Red  | Status
|:-----:|:------:|:----:| ------
| ON    |        |      | Device is idle and listening
| ON    | ON     |      | The device has lost its connection, and it's trying to reconnect
| ON    | ON     | ON   | The device is connecting to the SeismoCloud network
|       | ON     | ON   | Checking for updates or updating
|       | ON     |      | Connecting to Wi-Fi network*
|       |        | ON   | Calibration in progress

At the end of the boot sequence all three LEDs will blink rapidly to signal that it's OK.

*: If only the yellow LED is ON for more than 10 seconds then the NodeMCU is waiting for
Wi-Fi network configuration (see the chapter "How to build/upload the software")

# Hardware

* NodeMCU 1.0 devkit board with ESP-12E module
* MPU6050 Accelerometer
* (optional) 3 LEDs (red-green-yellow) with 3 resistors

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

# Software upload (binary)

[See this page (bottom section)](https://www.seismocloud.com/nodemcu-instructions/)

# FAQ

## My device has only the yellow LED powered on for more than 10 seconds

The sensor failed to connect to the Wi-Fi network. Please follow the instruction in chapter "How to build/upload the software" step 5

## I'm connected to the Wi-Fi network, but no configuration portal appears.

Try to open a browser and navigate to http://192.168.4.1 . If the Wi-Fi portal still not showing, disconnect the board from
the power source for few seconds. If it still fails, flash again the board erasing the Wi-Fi configuration using the option
from the **Tools** menu.


# For advanced users

This way of installing a sensor is not supported. If you're installing a sensor please refer to the official guide in the website: https://www.seismocloud.com/the-seismometer/

TODO

# License

See `LICENSE` file
