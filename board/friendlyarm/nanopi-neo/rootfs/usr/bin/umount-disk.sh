#!/bin/sh

umount /media/$1
rm -rf /media/$1

if [ -f "/tmp/cboxtools.i2c" ]; then
	rm /tmp/cboxtools.i2c
	rm -rf /tmp/cbox
fi
