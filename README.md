SeismoCloud project: http://www.seismocloud.com

# Supported boards

* [NodeMCU 1.0](NodeMCU.md) devkit (with ESP-12E module - ESP8266)
* [Arduino/Genuino Uno](Arduino.md) (with Ethernet Shield)

Click on board names to read detailed instructions for hardware and software.

# Network requirements

If you have any firewall in your network, please allow these ports:

* TCP: 1883 (outgoing)

Also, Wi-Fi or Ethernet networks should have a DHCP service running.

# LED status description

LEDs can be in these different states:

* **Green**: device is ready
* **Green + Yellow**: device is ready but there is an issue connecting to SeismoCloud APIs
* **Green + Red (only for about 5 seconds)**: shake detected
* **Yellow ONLY - blinking**: no position available - initialize Seismometer with Android/iOS App
* **Green + Yellow + Red**: software is loading
* **Green + Yellow + Red - ALL blinking fast**: software is loaded, starting accelerometer
* **Green + Yellow + Red - ALL blinking slow**: network init failed
* **Yellow + Red - ALL blinking**: EEPROM failed
