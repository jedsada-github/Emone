#!/bin/sh
# 2018, Michael Welling <mwelling@ieee.org>

BOARD_DIR="$(dirname $0)"

# copy the uEnv.txt to the output/images directory
cp board/emone/pocketbeagle/uEnv.txt $BINARIES_DIR/uEnv.txt
cp board/emone/pocketbeagle/dts/am335x-pocketbeagle-common.dtsi $BINARIES_DIR/am335x-pocketbeagle-common.dtsi
#cp board/emone/pocketbeagle/dts/*.dtb $BINARIES_DIR/
#cp board/emone/pocketbeagle/dts/am335x-pocketbeagle.dtb $BINARIES_DIR/am335x-pocketbeagle.dtb

GENIMAGE_CFG="${BOARD_DIR}/genimage.cfg"
GENIMAGE_TMP="${BUILD_DIR}/genimage.tmp"

rm -rf "${GENIMAGE_TMP}"

genimage \
    --rootpath "${TARGET_DIR}" \
    --tmppath "${GENIMAGE_TMP}" \
    --inputpath "${BINARIES_DIR}" \
    --outputpath "${BINARIES_DIR}" \
    --config "${GENIMAGE_CFG}"
