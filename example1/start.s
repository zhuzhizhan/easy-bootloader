.text
.global _start

_start:
	ldr sp, =0x00007f00
	bl main

halt_loop:
	b halt_loop
