#!/bin/bash
set -e
STARTDIR=`pwd`
SELFDIR=`dirname \`realpath ${0}\``
MKIMAGE="${HOST_DIR}/bin/mkimage"
IMAGE_ITS="kernel.its"
OUTPUT_NAME="kernel.itb"

[ $# -eq 2 ] || {
    echo "SYNTAX: $0 <output dir> <u-boot-with-spl image>"
    echo "Given: $@"
    exit 1
}

cp board/widora/mangopi/card/kernel.its "${BINARIES_DIR}"
cd "${BINARIES_DIR}"
"${MKIMAGE}" -f ${IMAGE_ITS} ${OUTPUT_NAME}
rm ${IMAGE_ITS}

cd "${SELFDIR}/../"
cp splash.bmp "${BINARIES_DIR}/"

cd "${STARTDIR}/"

board/widora/mangopi/card/scripts/mknanduboot.sh ${1}/${2} ${1}/u-boot-sunxi-with-nand-spl.bin
support/scripts/genimage.sh ${1} -c board/widora/mangopi/card/genimage-sdcard.cfg
support/scripts/genimage.sh ${1} -c board/widora/mangopi/card/genimage-nor.cfg
support/scripts/genimage.sh ${1} -c board/widora/mangopi/card/genimage-nand.cfg
