
ifeq (${PORT},)
PORT = /dev/ttyUSB0
endif

VERSION = 1.30.8

ARDUINO ?= arduino
#MODEL = $(shell grep MODEL seismoclouddevice-nodemcu/common.h | cut -d '"' -f 2)
MODEL ?= esp8266
PREFFILE = $(shell pwd)/preferences.${MODEL}.txt
PREFS = --pref build.path=$(shell pwd)/tmp/ --pref serial.port=${PORT} --preferences-file ${PREFFILE}

ifeq ($(shell test -f ${PREFFILE} || echo x),x)
$(error Unsupported platform)
endif

OUTDIR=out/prod
ifneq (${DEBUG},)
OUTDIR=out/test
endif

.PHONY: all
all:
	$(info Please use verify|build|upload (add -prod to target prod versions))

.PHONY: info
info:
ifneq (${DEBUG},)
	$(info *** DEBUG MODE ***)
endif
	$(info Arduino command: ${ARDUINO})
	$(info Platform: ${MODEL})
	$(info Version: ${VERSION})
	$(info Port: ${PORT})
	$(info Pref command line: ${PREFS})
	@:

.PHONY: prepare
prepare:
	-${ARDUINO} ${PREFS} --install-boards "esp8266:esp8266:2.6.2"
	${ARDUINO} ${PREFS} --install-library "PubSubClient:2.7,WiFiManager:0.15.0-beta"

.PHONY: seismoclouddevice-nodemcu/config.h
seismoclouddevice-nodemcu/config.h:
	echo "#define MODEL \"${MODEL}\"" > seismoclouddevice-nodemcu/config.h
	echo "#define MODEL_${MODEL}" | tr -d '-' >> seismoclouddevice-nodemcu/config.h
	echo "#define VERSION \"${VERSION}\"" >> seismoclouddevice-nodemcu/config.h
ifneq (${DEBUG},)
	echo "#define DEBUG" >> seismoclouddevice-nodemcu/config.h
	echo "#define DONT_UPDATE" >> seismoclouddevice-nodemcu/config.h
endif

.PHONY: verify
verify: seismoclouddevice-nodemcu/config.h
	${ARDUINO} --verify -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino

.PHONY: build
build: seismoclouddevice-nodemcu/config.h
	${ARDUINO} --verify -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino
	mkdir -p ${OUTDIR}
	cp -v tmp/seismoclouddevice-nodemcu.ino.bin ${OUTDIR}/${MODEL}-${VERSION}.bin
	cd ${OUTDIR}/ && md5sum ${MODEL}-${VERSION}.bin > ${MODEL}-${VERSION}.md5

.PHONY: upload
upload: seismoclouddevice-nodemcu/config.h
	${ARDUINO} --upload -v ${PREFS} seismoclouddevice-nodemcu/seismoclouddevice-nodemcu.ino

.PHONY: console
console:
	minicom -o -D ${PORT} -b 115200

.PHONY: clean
clean:
	rm -rf tmp/ out/
