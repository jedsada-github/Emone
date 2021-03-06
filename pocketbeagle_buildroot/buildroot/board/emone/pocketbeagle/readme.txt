PocketBeagle

Description
===========

PocketBeagle mainline with minimal patches.

How to build it
===============
Clear the package / it's take time to complile. just do only first time.
$ make clean

Select the default configuration for the target: 
$ make pocketbeagle_defconfig

Optional: modify the configuration: If do not need to customize the other feature, skip this
$ make menuconfig

Build:
$ make

If error in patches:
$ grep ^BR2_GLOBAL_PATCH_DIR .config
$ make strace
$ make

If error warning about device tree (dts):
$ cp board/emone/pocketbeagle/dts/* output/build/linux-4.14.18/arch/arm/boot/dts/
$ make linux-rebuild
$ make

Modified the device tree: am335x-pocketbeagle.dts
$ vim output/build/linux-4.14.18/arch/arm/boot/dts/am335x-pocketbeagle.dts
$ make linux-rebuild
$ make

Result of the build
===================
output/images/
├── am335x-pocketbeagle.dtb
├── boot.vfat
├── MLO
├── rootfs.ext2
├── rootfs.tar
├── sdcard.img
├── u-boot.img
├── uEnv.txt
└── zImage

To copy the image file to the sdcard use dd:
$ sudo dd if=output/images/sdcard.img of=/dev/sdb bs=1M

Tested hardware
===============
----------------------------------------------------------
# User name and password for login
----------------------------------------------------------
Login username/password
user: root
pass: emone123

----------------------------------------------------------
# Testing on-off gpio on pocketbeagle board
# Using command line
# On the P2_33 GPIO
# GPIO 1_13 On
# Port P1_13 = 1 * 32 + 13 = 45
----------------------------------------------------------
$ echo 45 > /sys/class/gpio/export
$ echo out > /sys/class/gpio/gpio45/direction
$ echo 1 > /sys/class/gpio/gpio45/value

# GPIO 1_13 Off
$ echo 45 > /sys/class/gpio/unexport

----------------------------------------------------------
# Testing on-off gpio on pocketbeagle board
# Creating the .profile file in home directory.
# So the gpio will on when the user already login to system.
# Created the .profile in home directory and write the command below:
----------------------------------------------------------
$ vi .profile

# GPIO 1_13 On
$ echo 45 > /sys/class/gpio/export
$ echo out > /sys/class/gpio/gpio45/direction
$ echo 1 > /sys/class/gpio/gpio45/value

## Save vim and reboot the system.

----------------------------------------------------------
# USB test
# Pocketbeagle system.
----------------------------------------------------------

$ dmesg | grep usb0
$ ifconfig usb0 192.168.0.199 up
$ setenv ipaddr 192.168.7.2
$ setenv serverip 192.168.7.1
$ setenv netmask 255.255.255.0
$ saveenv

$ fdisk -l
$ mkdir /media/pendrive
$ mount /dev/sda1 /media/pendrive
$ umount /media/pendrive/

----------------------------------------------------------
# Modified Devicetree for the pocketbeagle board.
# Ubuntu system
----------------------------------------------------------

$ vim output/build/linux-4.14-xenomai/arch/arm/boot/dts/am335x-pocketbeagle-common.dtsi

led@1 {
   gpios = <&gpio1 13 GPIO_ACTIVE_HIGH>;
   linux,default-trigger = "default-on";
   default-state = "on";
};  

$ make linux-rebuild
$ make

----------------------------------------------------------
# Note: Other optional to modified the device for pocketbeagle board.
# get the ti device tree (dts) am335x-osd335x-common.dtsi am335x-pocketbeagle.dts
# Ubuntu system
----------------------------------------------------------
$ git clone https://github.com/RobertCNelson/dtb-rebuilder
$ cd dtb-rebuilder
$ make ARCH=arm CROSS_COMPILE=buildroot/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-

On the GPIO13 (P2,33): Can do both structure 
GPIO_ACTIVE_HIGH 0
GPIO_ACTIVE_LOW 1

# Struture #1
gpios = <&gpio1 13 GPIO_ACTIVE_LOW>; 

# Struture #2
usr4 {  
  label = "beaglebone:green:usr4";
  gpios = <&gpio1 13 GPIO_ACTIVE_LOW>;
  default-state = "on"; 
}; 


----------------------------------------------------------
# Forwarding the port for internet connection via USB Host 
# option 1 
----------------------------------------------------------

# Forward port with PC LAN Port (In the Ubuntu PC)
$ iptables --append FORWARD --in-interface enp0s31f6 -j ACCEPT
$ iptables --table nat --append POSTROUTING --out-interface wlx60634c283bb7 -j MASQUERADE
$ echo 1 > /proc/sys/net/ipv4/ip_forward

If IPV6 or IPV4
$ sysctl net.ipv6.conf.all.forwarding=1
$ sysctl -w net.ipv4.ip_forward=1
$ ifconfig enp0s31f6 192.168.1.1 up

# Setting IP Address in pocketbeagle
$ ifconfig eth0 192.168.1.100 up
$ route add default gw 192.168.1.1
$ echo "nameserver 8.8.8.8" >> /etc/resolv.conf
$ vi /etc/network/interfaces

# interface file auto-generated by buildroot

iface eth0 inet static
        address 192.168.1.100
        netmask 255.255.255.0
        gateway 192.168.1.1
        dns-nameservers 8.8.8.8
        dns-nameservers 8.8.4.4

----------------------------------------------------------
# Forwarding the port for internet connection via USB Host 
# option 2 
----------------------------------------------------------
# Forward port with PC LAN Port (In the Ubuntu PC)
$ iptables --append FORWARD --in-interface enp0s20f0u11 -j ACCEPT
$ iptables --table nat --append POSTROUTING --out-interface wlx60634c283bb7 -j MASQUERADE
$ echo 1 > /proc/sys/net/ipv4/ip_forward
$ ifconfig enp0s20f0u11 192.168.1.1 up

# Setting IP Address in pocketbeagle
$ ifconfig usb0 192.168.1.100 up
$ route add default gw 192.168.1.1
$ echo "nameserver 8.8.8.8" >> /etc/resolv.conf
$ vi /etc/network/interfaces

2018, Michael Welling <mwelling@ieee.org>
2020, Jedsada Udomsed <jedsada.us@gmail.com>
