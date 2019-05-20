CC      = arm-linux-gcc
LD      = arm-linux-ld
AR      = arm-linux-ar
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

INCLUDEDIR 	:= $(shell pwd)/include 
CFLAGS 		:= -Wall -O2 -fno-builtin
CPPFLAGS   	:= -nostdinc -I$(INCLUDEDIR) 

export 	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

objs := stdio.o start.o main.o uart.o shell.o command.o lib.o clock.o lib/libc.a

shell.bin: $(objs)
	${LD} -Tshell.lds -o shell.elf $^
	${OBJCOPY} -O binary -S shell.elf $@
	${OBJDUMP} -D shell.elf > shell.dis
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.PHONY : lig/libc.a
lib/libc.a:
	cd lib; make; cd ..
	
clean:
	make -C ./lib
	rm -f *.bin *.elf *.dis *.o *~
	
