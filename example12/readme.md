## qemu for u-boot

ftp://ftp.denx.de/pub/u-boot/ to get the u-boot-2018.09.tar.bz2

make:
make vexpress_ca9x4_config ARCH=arm CROSS_COMPILE=arm-none-eabi-
make all ARCH=arm CROSS_COMPILE=arm-none-eabi-

qemu:
qemu-system-arm -M vexpress-a9 -m 32M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait -kernel u-boot-2018.09/u-boot

bare-arm.uimg:
u-boot-2018.09/tools/mkimage -A arm -C none -T kernel -a 0x60000000 -e 0x60000000 -d better-hang.elf bare-arm.uimg

sdcard.img:
./create-sd.sh sdcard.img bare-arm.uimg

qemu:
qemu-system-arm -M vexpress-a9 -m 32M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait -kernel u-boot-2018.09/u-boot -sd sdcard.img

u-boot:
ext2load mmc 0 0x60000000 bare-arm.uimg
bootm 0x60000000
