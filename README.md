SeismoCloud project: http://www.seismocloud.com

# Supported boards

* NodeMCU 1.0 devkit (with ESP-12E module - ESP8266)
* Arduino/Genuino Uno (with Ethernet Shield)

## Hardware: assemble a NodeMCU

### Requirements for NodeMCU

* Arduino/Genuino IDE with ESP8266 sdk installed. If you haven't ESP8266 sdk:
	* Open *Preferences* window (from *File* menu)
	* Enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json` into *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.
	* Close with "OK", open *Boards Manager* from *Tools* > *Board* menu and install *esp8266* platform (and don't forget to select *NodeMCU 1.0 (ESP-12E)* board from *Tools* > *Board* menu after installation).
* NodeMCU 1.0 devkit board with ESP-12E module
* MPU6050 Accelerometer
* (optional) 3 LEDs (red-green-yellow) with 3 resistors

### Accelerometer MPU6050

Link these pins from Accelerometer MPU6050 to NodeMCU board:

* 3v3: 3v3
* GND: GND
* SDA: D1
* SCL: D2

### (optional) LEDs

Remember to put a resistor with LED (after/before is not really important) to limit
current flowing, otherwise you may damage the NodeMCU board.

By default, LED pins are:

* Pin D5 : Green
* Pin D6 : Yellow
* Pin D7 : Red

## Hardware: assemble an Arduino/Genuino

### Requirements for Arduino/Genuino

* Arduino/Genuino IDE
* Arduino/Genuino UNO board
* Arduino ISP and SD-card (any size >= 10 MB)
* WizNet-compatible Ethernet shield (with SD reader)
* MMA7361 Accelerometer
* (optional) 3 LEDs (red-green-yellow) with 3 resistors

We recommend to buy Arduinos on official website https://store.arduino.cc

### Accelerometer MMA7361

Link these pins from Accelerometer MMA7361 to Arduino board:

* Vin: 5v
* GND: GND
* SEL: GND
* X: A0
* Y: A1
* Z: A2

Loop back **3v3** pin to **SLP** on Accelerometer.

### Burning a new bootloader with Arduino ISP for self-upgrade

In order to self-upgrade itself, Arduino must use an SD-card and a custom bootloader.

**Please note that, with avr_boot bootloader, you'll not be able to debug/upload sketch using standard method**.
To restore old bootloader, follow these instructions except for `10`, when you have to
choose `Arduino/Genuino Uno` as board (then proceed with `11`).

Steps:

1. Unplug all hardware from Arduino Uno
2. Plug the ArduinoISP into ICSP 6-pin-header of Arduino Uno (you can find ICSP
	header on the left side, having USB port on right side)
3. Power up Arduino Uno with a power supply (either with USB port or power jack port)
4. Plug ArduinoISP to PC (with microUSB port on ArduinoISP)
5. Open Arduino IDE (on Linux you may need to open it as root)
6. Open `File > Preferences`. Into `Additional Boards Manager URLs` put `https://zevero.github.io/avr_boot/package_zevero_avr_boot_index.json`
7. Save and close
8. Open `Tools > Board > Boards Manager`. Search for `avr_boot` and install.
9. Close `Boards Manager`
10. On `Tools > Board` choose `ATmega328/P`
11. Launch bootloader burn using `Tools > Burn bootloader`

On finish, you can close the IDE, unplug all and build your SeismoCloud sensor! (remember also to follow `Software` instructions)

### (optional) LEDs

Remember to put a resistor with LED (after/before is not really important) to limit
current flowing, otherwise you may damage the Arduino board.

By default, LED pins are:

* Pin 2 : Yellow
* Pin 5 : Green
* Pin 3 : Red

## Software

### Network requirements

If you have any firewall in your network, please allow these ports:

* TCP: 1883 (outgoing)

### [NodeMCU] Build and upload code

For NodeMCU, you need to download `WifiManager` library

1. Download the source code (for stable releases, please checkout latest git tag)
2. Open project in Arduino IDE
3. Choose the right **Port** and **Board** into **Tools** menu
4. Compile and upload (2nd button below menus) in your board
5. Unplug the board from PC and plug Accelerometer and Leds, then power on the board
6. For NodeMCU boards, connect to `SeismoCloud` Wi-Fi network and configure Wi-Fi client network parameters. On save, the board reboots and try to connect to Wi-Fi network. If it fails, you can reconnect to `SeismoCloud` network and modify/fix network parameters.
7. Open SeismoCloud app, connect to the same network of the board and register your device. Enjoy!

### [Arduino] Download sketch into SD-card

After hardware build, you need to setup the software:

1. Format your SD-card with FAT32 (or FAT16)
2. Download FIRMWARE.BIN from http://www.seismocloud.com/page.php?id=instr-arduino
3. Place FIRMWARE.BIN into SD-card
4. Insert SD-card into Arduino Uno and power on. Done!

## LED status description

LEDs can be in these different states:

* **Green**: device is ready
* **Green + Yellow**: device is ready but there is an issue connecting to SeismoCloud APIs
* **Green + Red (only for about 5 seconds)**: shake detected
* **Yellow ONLY - blinking**: no position available - initialize Seismometer with Android/iOS App
* **Green + Yellow + Red**: software is loading
* **Green + Yellow + Red - ALL blinking fast**: software is loaded, starting accelerometer
* **Green + Yellow + Red - ALL blinking slow**: network init failed
* **Yellow + Red - ALL blinking**: EEPROM failed
