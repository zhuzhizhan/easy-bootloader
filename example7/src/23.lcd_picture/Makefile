CC      = arm-linux-gcc
LD      = arm-linux-ld
AR      = arm-linux-ar
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

INCLUDEDIR 	:= $(shell pwd)/include 
CFLAGS 		:= -Wall -O2 -fno-builtin
CPPFLAGS   	:= -nostdinc -I$(INCLUDEDIR) 

export 	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

objs := start.o main.o uart.o lcd.o lib/libc.a

lcd.bin: $(objs)
	${LD} -Tlcd.lds -o lcd.elf $^
	${OBJCOPY} -O binary -S lcd.elf $@
	${OBJDUMP} -D lcd.elf > lcd.dis
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.PHONY : lig/libc.a
lib/libc.a:
	cd lib; make; cd ..
	
clean:
	make clean -C ./lib 
	rm -f *.bin *.elf *.dis *.o *~
	
