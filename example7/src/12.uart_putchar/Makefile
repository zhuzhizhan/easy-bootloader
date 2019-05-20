uart.bin: start.o main.o uart.o clock.o
	arm-linux-ld -Ttext 0x20000000 -o uart.elf $^
	arm-linux-objcopy -O binary uart.elf uart.bin
	arm-linux-objdump -D uart.elf > uart_elf.dis

%.o : %.S
	arm-linux-gcc -o $@ $< -c -fno-builtin

%.o : %.c
	arm-linux-gcc -o $@ $< -c -fno-builtin

clean:
	rm *.o *.elf *.bin *.dis *.exe *~ -f
