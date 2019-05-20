BL2.bin: start.o main.o
	arm-linux-ld -Tsdram.lds -o BL2.elf $^
	arm-linux-objcopy -O binary BL2.elf BL2.bin
	arm-linux-objdump -D BL2.elf > BL2_elf.dis
	
%.o : %.S
	arm-linux-gcc -o $@ $< -c  

%.o : %.c
	arm-linux-gcc -o $@ $< -c   

clean:
	rm *.o *.elf *.bin *.dis -f
