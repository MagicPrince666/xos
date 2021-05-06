setenv bootargs console=ttyS0,115200 earlyprintk root=/dev/mmcblk0p2 rootwait
load mmc 0:1 0x41000000 zImage
load mmc 0:1 0x41800000 sun8i-v3s-licheepi-zero.dtb
bootz 0x41000000 - 0x41800000
