arm-none-eabi-as -o startup.o startup.s
arm-none-eabi-gcc -c -nostdlib -nostartfiles -lgcc -o cstart.o cstart.c
arm-none-eabi-ld -T linkscript.ld -o cenv.elf startup.o cstart.o
arm-none-eabi-objcopy -O binary cenv.elf cenv.bin
mkimage -A arm -C none -T kernel -a 0x60000000 -e 0x60000000 -d cenv.bin bare-arm.uimg
./create-sd.sh sdcard.img bare-arm.uimg

qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait -kernel ../common_uboot/u-boot - sd sdcard.img


