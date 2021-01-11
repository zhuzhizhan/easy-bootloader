arm-none-eabi-as -o startup.o startup.s
arm-none-eabi-gcc -c -nostdlib -nostartfiles -lgcc -o cstart.o cstart.c
arm-none-eabi-ld -T linkscript.ld -o cenv.elf startup.o cstart.o
arm-none-eabi-objcopy -O binary cenv.elf cenv.bin
