CFLAGS := -Wall -Wstrict-prototypes -g -fomit-frame-pointer -ffreestanding

all: start.s led.s
	arm-linux-gnueabi-gcc $(CFLAGS) -c -o start.o start.s
	arm-linux-gnueabi-gcc $(CFLAGS) -c -o led.o led.s
	arm-linux-gnueabi-ld -T led.lds start.o led.o -o led_elf
	arm-linux-gnueabi-objcopy -O binary -S led_elf led.bin
	arm-linux-gnueabi-objdump -D -m arm led_elf > led.dis

clean:
	rm -f led.dis led.bin led_elf *.o
