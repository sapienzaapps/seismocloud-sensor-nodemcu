
ifeq (${PORT},)
PORT = /dev/ttyUSB0
endif

PREFS = --pref build.path=$(shell pwd)/tmp/ --pref serial.port=${PORT} --preferences-file $(shell pwd)/preferences.txt
ARDUINO = arduino

ifneq (${USE_DOCKER},)
ARDUINO = ${USE_DOCKER} run --rm -it -v $(pwd):/src arduino-ide-env:latest arduino
endif

all:
	$(info Please use verify|upload)

prepare:
	-${ARDUINO} ${PREFS} --install-boards "esp8266:esp8266:2.6.2"
	${ARDUINO} ${PREFS} --install-library "PubSubClient:2.7,WiFiManager:0.15.0-beta"

verify:
	${ARDUINO} --verify -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino

verify-debug:
	make set-debug
	${ARDUINO} --verify -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino || (make unset-debug && exit 1)
	make unset-debug

upload:
	${ARDUINO} --upload -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino

upload-debug:
	make set-debug
	${ARDUINO} --upload -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino || (make unset-debug && exit 1)
	make unset-debug

console:
	minicom -o -D ${PORT} -b 115200

clean:
	rm -rf tmp/

set-debug:
	@sed -i 's%^// #define DEBUG%#define DEBUG%' seismoclouddevice-nodemcu/common.h

unset-debug:
	@sed -i 's%^#define DEBUG%// #define DEBUG%' seismoclouddevice-nodemcu/common.h
