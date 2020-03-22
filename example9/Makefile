objs := head.o init.o leds.o

mmu.bin : $(objs)
	arm-linux-gnueabi-ld -Tmmu.lds -o mmu_elf $^
	arm-linux-gnueabi-objcopy -O binary -S mmu_elf $@
	arm-linux-gnueabi-objdump -D -m arm mmu_elf > mmu.dis
    
%.o : %.c
	arm-linux-gnueabi-gcc -Wall -O2 -c -o $@ $<

%.o : %.S
	arm-linux-gnueabi-gcc -Wall -O2 -c -o $@ $<

clean:
	rm -f mmu.bin mmu_elf mmu.dis *.o

