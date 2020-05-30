
ifeq (${PORT},)
PORT = /dev/ttyUSB0
endif

PREFS = --pref build.path=$(shell pwd)/tmp/ --pref serial.port=${PORT} --preferences-file $(shell pwd)/preferences.txt
ARDUINO ?= arduino
PLATFORM = $(shell grep MODEL seismoclouddevice-nodemcu/common.h | cut -d '"' -f 2)
VERSION = $(shell grep VERSION seismoclouddevice-nodemcu/common.h | cut -d '"' -f 2)

all:
	$(info Please use verify|build|upload (add -prod to target prod versions))

prepare:
	-${ARDUINO} ${PREFS} --install-boards "esp8266:esp8266:2.6.2"
	${ARDUINO} ${PREFS} --install-library "PubSubClient:2.7,WiFiManager:0.15.0-beta"

verify-prod:
	${ARDUINO} --verify -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino

verify:
	make set-debug
	${ARDUINO} --verify -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino || (make unset-debug && exit 1)
	make unset-debug

build-prod:
	${ARDUINO} --verify -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino
	mkdir -p out/prod/
	cp -v tmp/seismoclouddevice-nodemcu.ino.bin out/prod/${PLATFORM}-${VERSION}.bin
	cd out/prod/ && md5sum ${PLATFORM}-${VERSION}.bin > ${PLATFORM}-${VERSION}.md5
	$(ccred)
	$(info Environment built: production)

build-test:
	make set-debug
	${ARDUINO} --verify -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino
	make unset-debug
	mkdir -p out/test/
	cp -v tmp/seismoclouddevice-nodemcu.ino.bin out/test/${PLATFORM}-${VERSION}.bin
	cd out/test/ && md5sum ${PLATFORM}-${VERSION}.bin > ${PLATFORM}-${VERSION}.md5

upload-prod:
	${ARDUINO} --upload -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino

upload:
	make set-debug
	${ARDUINO} --upload -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino || (make unset-debug && exit 1)
	make unset-debug

console:
	minicom -o -D ${PORT} -b 115200

clean:
	rm -rf tmp/ out/

set-debug:
	@sed -i 's%^// #define DEBUG%#define DEBUG%' seismoclouddevice-nodemcu/common.h

unset-debug:
	@sed -i 's%^#define DEBUG%// #define DEBUG%' seismoclouddevice-nodemcu/common.h
