#!/bin/sh

#avail=`sed -n 2p tmp.log | awk '{print $2}'`
avail=`df -h | grep /dev/root | awk '{print $2}'`

case "$avail" in
    *M)
            echo "resize file system"
			resize2fs /dev/mmcblk0p2
            rm /etc/init.d/S54resize2fs
            ;;
    *G)
            echo "file system normal"
            ;;
    *)
            echo "file system info unknow"
			exit 1
            ;;

esac
