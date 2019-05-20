link.bin: start.o main.o 
	arm-linux-ld -Tlink.lds -o link.elf $^
	arm-linux-objcopy -O binary link.elf link.bin
	arm-linux-objdump -D link.elf > link_elf.dis
	gcc mkv210_image.c -o mkmini210
	./mkmini210 link.bin 210.bin

%.o : %.S
	arm-linux-gcc -o $@ $< -c 

%.o : %.c
	arm-linux-gcc -o $@ $< -c 

clean:
	rm *.o *.elf *.bin *.dis mkmini210 -f
