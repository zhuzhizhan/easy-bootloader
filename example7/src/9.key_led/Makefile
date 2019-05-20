key_led.bin: start.o main.o
	arm-linux-ld -Ttext 0x20000000 -o key_led.elf $^
	arm-linux-objcopy -O binary key_led.elf key_led.bin
	arm-linux-objdump -D key_led.elf > key_led_elf.dis

%.o : %.S
	arm-linux-gcc -o $@ $< -c

%.o : %.c
	arm-linux-gcc -o $@ $< -c 

clean:
	rm *.o *.elf *.bin *.dis *.exe -f
