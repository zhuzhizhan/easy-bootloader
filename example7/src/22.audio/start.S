.global _start
	
_start:
	mov r0, #0x53
	msr	CPSR_cxsf, r0

	b main
