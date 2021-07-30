--------------------------------------------------------------------------------------
1. Update OS
--------------------------------------------------------------------------------------
$ sudo apt install --no-install-recommends git cmake ninja-build gperf
  ccache dfu-util device-tree-compiler wget
  python3-pip python3-setuptools python3-tk python3-wheel xz-utils file
  make gcc gcc-multilib g++-multilib libsdl2-dev

# CMake version 3.8.2 or higher is required
$ cmake --version  
$ wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -  

# Add the kitware repo corresponding to the Ubuntu 18.04 LTS release:
$ sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'  
$ sudo apt update  
$ sudo apt install cmake  

# Install west:
$ pip3 install --user -U west  
$ echo 'export PATH=~/.local/bin:"$PATH"' >> ~/.bashrc  
$ source ~/.bashrc  

--------------------------------------------------------------------------------------
2. Create project
--------------------------------------------------------------------------------------
$ cd ~  
$ mkdir emone  
$ cd emone  
$ west init zephyrproject  
$ cd zephyrproject  
$ west update  
$ west zephyr-export  

# Install needed Python packages
$ pip3 install --user -r ./zephyr/scripts/requirements.txt  

--------------------------------------------------------------------------------------
3. Install Software Development Toolchain
--------------------------------------------------------------------------------------
$ cd ~  
$ wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.11.2/zephyr-sdk-0.11.2-setup.run  
$ chmod +x zephyr-sdk-0.11.2-setup.run  
$ ./zephyr-sdk-0.11.2-setup.run -- -d ~/zephyr-sdk-0.11.2  
$ export ZEPHYR_TOOLCHAIN_VARIANT=zephyr  
$ export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-0.11.2  
$ sudo cp ${ZEPHYR_SDK_INSTALL_DIR}/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d  
$ sudo udevadm control --reload  


--------------------------------------------------------------------------------------
4. Testing hardware
--------------------------------------------------------------------------------------
$ cd ~  
$ cd emone/zephyrproject  
$ mkdir emone_project  
$ cd emone_project  

# Hello print program
$ cp -r ../zephyr/samples/hello_world/ ./  
$ cd hello_world  
$ mkdir build  
$ cd build  

# Blink Led program
$ cp -r ../zephyr/samples/basic/blinky ./  
$ cd blinky  
$ mkdir build  
$ cd build  

# Use cmake to configure a Ninja-based build system:
$ cmake -GNinja -DBOARD=stm32f4_disco ..  

# Now run ninja on the generated build system:
$ ninja  
$ ninja flash  

# using stlink flash program
# Burn *.bin file
$ st-flash write ./zephyr/zephyr.bin 0x8000000   
# Burn *.hex file
$ st-flash --reset --format ihex write ./zephyr/zephyr.hex  
# Erase flash
$ st-flash erase  
