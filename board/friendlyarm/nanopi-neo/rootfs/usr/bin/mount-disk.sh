#!/bin/sh

mkdir -p /media/$1
mount /dev/$1 /media/$1

if [ -f "/media/$1/cboxtools.i2c" ]; then
	cp /media/$1/cboxtools.i2c /tmp
	mkdir -p /tmp/cbox
	tar -zxvf /tmp/cboxtools.i2c -C /tmp/cbox
	if [ -f "/tmp/cbox/root/install.sh" ]; then
		chmod +x /tmp/cbox/root/install.sh
	fi
	sync
fi
