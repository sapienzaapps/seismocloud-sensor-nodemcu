# BETA: NOT READY FOR PRODUCTION

# Arduino/Genuino sketch

## Requirements

* Arduino/Genuino IDE
* Arduino/Genuino board (Arduino Uno with Ethernet Shield or Arduino Ethernet)
* MMA7361 Accelerometer

## Network requirements

If you have any firewall in your network, please allow these ports:

* TCP: 80, 443
* UDP: 123

## How to build

1. Download the source code
2. Open project in Arduino IDE
3. Compile and upload in your Arduino Board

# Hardware
## LEDs

LEDs can be in these different states:

* **Green**: device is ready
* **Green + Yellow**: device is ready but there is an issue connecting to SeismoCloud APIs
* **Green + Red (only for about 5 seconds)**: shake detected
* **Yellow ONLY - blinking**: no position available - initialize Seismometer with Android/iOS App
* **Green + Yellow + Red**: software is loading
* **Green + Yellow + Red - ALL blinking fast**: software is loaded, starting accelerometer
* **Green + Yellow + Red - ALL blinking slow**: network init failed
* **Yellow + Red - ALL blinking**: EEPROM failed

### LED pins

* Pin 2 : Yellow
* Pin 5 : Green
* Pin 3 : Red

Please note that some pins are reserved to Ethernet (SPI). If you use different boards (eg. Arduino Yun, etc), please refer to Arduino documentation. You can change LED pin numbers into main sketch file (`seismoclouddevice-arduino.ino`).

## Link Accelerometer MMA7361 to Arduino

Link these pins from Accelerometer MMA7361 to Arduino board:

* Vin: 5v
* GND: GND
* SEL: GND
* X: A0
* Y: A1
* Z: A2

Loop back **3v3** pin to **SLP** on Accelerometer.
