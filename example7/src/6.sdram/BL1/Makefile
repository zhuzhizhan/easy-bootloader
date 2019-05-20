sdram.bin: start.o memory.o mmc_relocate.o
	arm-linux-ld -Tsdram.lds -o sdram.elf $^
	arm-linux-objcopy -O binary sdram.elf sdram.bin
	arm-linux-objdump -D sdram.elf > sdram_elf.dis
	gcc mkv210_image.c -o mkmini210
	./mkmini210 sdram.bin BL1.bin
	
%.o : %.S
	arm-linux-gcc -o $@ $< -c 

%.o : %.c
	arm-linux-gcc -o $@ $< -c 

clean:
	rm *.o *.elf *.bin *.dis mkmini210 -f
