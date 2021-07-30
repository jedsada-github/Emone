https://blog.csdn.net/freemote/article/list/1

```
cmake -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain-arm-none-eabi.cmake" -DAPPLICATION="LoRaMac" -DSUB_PROJECT="classC" ..
cmake -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain-arm-none-eabi.cmake" -DAPPLICATION="LoRaMac" -DSUB_PROJECT="classC" -DBOARD="SKiM980A" -DACTIVE_REGION="LORAMAC_REGION_AS923" -DREGION_AS923="ON" -DRADIO="sx1272" ..

$ git clone https://github.com/Lora-net/LoRaMac-node
$ mkdir build
$ cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain-arm-none-eabi.cmake" -DAPPLICATION="pkt-fwd" -DBOARD="SKiM980A" -DREGION_AS923="ON" ..
$ make
$ find . -name *.hex
$ st-info --probe
```
# Burn *.bin file
```
$ st-flash write ./src/apps/pkt-fwd/pkt-fwd.bin 0x8000000 
```
# Burn *.hex file
```
$ st-flash --reset --format ihex write ./src/apps/pkt-fwd/pkt-fwd.hex 
```
# Erase flash
```
$ st-flash erase
```
```
$ cmake -DCMAKE_TOOLCHAIN_FILE="../cmake/toolchain-arm-none-eabi.cmake" \
        -DAPPLICATION="pkt-fwd" \
        -DCLASSB_ENABLED="ON" \
        -DREGION_EU868="OFF" \
        -DREGION_US915="OFF" \
        -DREGION_CN779="OFF" \
        -DREGION_EU433="OFF" \
        -DREGION_AU915="OFF" \
        -DREGION_AS923="ON" \
        -DREGION_CN470="OFF" \
        -DREGION_KR920="OFF" \
        -DREGION_IN865="OFF" \
        -DREGION_RU864="OFF" \
        -DBOARD="SKiM980A" \
        -DUSE_RADIO_DEBUG="ON" ..

$ cmake -DCMAKE_TOOLCHAIN_FILE="../cmake/toolchain-arm-none-eabi.cmake" \
        -DAPPLICATION="LoRaMac" \
        -DSUB_PROJECT="periodic-uplink-lpp" \
        -DCLASSB_ENABLED="ON" \
        -DREGION_EU868="OFF" \
        -DREGION_US915="OFF" \
        -DREGION_CN779="OFF" \
        -DREGION_EU433="OFF" \
        -DREGION_AU915="OFF" \
        -DREGION_AS923="ON" \
        -DREGION_CN470="OFF" \
        -DREGION_KR920="OFF" \
        -DREGION_IN865="OFF" \
        -DREGION_RU864="OFF" \
        -DBOARD="SKiM980A" \
        -DUSE_RADIO_DEBUG="ON" ..

$ cmake -DCMAKE_TOOLCHAIN_FILE="../cmake/toolchain-arm-none-eabi.cmake" \
        -DAPPLICATION="ping-pong" \
        -DCLASSB_ENABLED="ON" \
        -DREGION_EU868="OFF" \
        -DREGION_US915="OFF" \
        -DREGION_CN779="OFF" \
        -DREGION_EU433="OFF" \
        -DREGION_AU915="OFF" \
        -DREGION_AS923="ON" \
        -DREGION_CN470="OFF" \
        -DREGION_KR920="OFF" \
        -DREGION_IN865="OFF" \
        -DREGION_RU864="OFF" \
        -DBOARD="SKiM980A" \
        -DUSE_RADIO_DEBUG="ON" ..

```
