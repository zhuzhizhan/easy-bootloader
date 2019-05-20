buzzer.bin: start.o main.o buzzer.o
	arm-linux-ld -Ttext 0x20000000 -o buzzer.elf $^
	arm-linux-objcopy -O binary buzzer.elf buzzer.bin
	arm-linux-objdump -D buzzer.elf > buzzer_elf.dis

%.o : %.S
	arm-linux-gcc -o $@ $< -c

%.o : %.c
	arm-linux-gcc -o $@ $< -c 

clean:
	rm *.o *.elf *.bin *.dis *.exe -f
