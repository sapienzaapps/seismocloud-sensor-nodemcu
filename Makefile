
ifeq (${PORT},)
PORT = /dev/ttyUSB0
endif

PREFS = --pref build.path=$(shell pwd)/tmp/ --pref serial.port=${PORT} --preferences-file $(shell pwd)/preferences.txt

all:
	$(info Please use verify|upload)

prepare:
	-arduino ${PREFS} --install-boards "esp8266:esp8266:2.6.2"
	arduino ${PREFS} --install-library "PubSubClient:2.7,WiFiManager:0.15.0-beta,SSD1306Ascii:1.3.0"

verify:
	arduino --verify -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino

verify-debug:
	make set-debug
	arduino --verify -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino || (make unset-debug && exit 1)
	make unset-debug

upload:
	arduino --upload -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino

upload-debug:
	make set-debug
	arduino --upload -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino || (make unset-debug && exit 1)
	make unset-debug

console:
	minicom -o -D ${PORT} -b 115200

clean:
	rm -rf tmp/

set-debug:
	@sed -i 's%^// #define DEBUG%#define DEBUG%' seismoclouddevice-nodemcu/common.h

unset-debug:
	@sed -i 's%^#define DEBUG%// #define DEBUG%' seismoclouddevice-nodemcu/common.h
