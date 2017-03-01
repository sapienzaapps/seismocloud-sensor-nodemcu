#!/bin/bash

# This script will prepare Arduino UNO sketch file (HEX) to flashable binary
# (it might be used into SD cards and/or for OTA with avr_boot bootloader)

if [ "$ARDUINOIDE" == "" ]; then
	echo "You need to set ARDUINOIDE env variable with Arduino IDE path";
	exit 1;
fi

if [ "$1" == "" ]; then
	echo "You need to specify where is HEX sketch file";
	exit 2;
fi

$ARDUINOIDE/hardware/tools/avr/bin/avr-objcopy -I ihex -O binary "$1" /tmp/FIRMWARE.BIN

CURSIZE=`du -b /tmp/FIRMWARE.BIN | awk -F' ' '{ print $1 }'`
TOPAD=$((32256-$CURSIZE))

dd if=/dev/zero bs=$TOPAD count=1 >> /tmp/FIRMWARE.BIN

echo "You can find FIRMWARE.BIN into /tmp/"

