#!/bin/sh
mkdir -p /media/$1
mount /dev/$1 /media/$1
sync
