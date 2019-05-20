led_wtd.bin: start.o delay.o
	arm-linux-ld -Ttext 0x0 -o led_wtd.elf $^
	arm-linux-objcopy -O binary led_wtd.elf led_wtd.bin
	arm-linux-objdump -D led_wtd.elf > led_wtd_elf.dis
	gcc mkv210_image.c -o mkmini210
	./mkmini210 led_wtd.bin 210.bin

%.o : %.S
	arm-linux-gcc -o $@ $< -c

%.o : %.c
	arm-linux-gcc -o $@ $< -c 


clean:
	rm *.o *.elf *.bin *.dis mkmini210 -f
