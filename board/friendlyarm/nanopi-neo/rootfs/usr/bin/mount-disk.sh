#!/bin/sh
mkdir -p /media/$1
mount /dev/$1 /media/$1

if [ -f "/media/$1/c-box" ]; then
	echo `Update checkra1n...`
	cp -r /media/$1/c-box/* /root
fi
