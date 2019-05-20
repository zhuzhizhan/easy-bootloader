clock.bin: start.o main.o clock.o
	arm-linux-ld -Ttext 0x20000000 -o clock.elf $^
	arm-linux-objcopy -O binary clock.elf clock.bin
	arm-linux-objdump -D clock.elf > clock_elf.dis

%.o : %.S
	arm-linux-gcc -o $@ $< -c

%.o : %.c
	arm-linux-gcc -o $@ $< -c 

clean:
	rm *.o *.elf *.bin *.dis *.exe -f
