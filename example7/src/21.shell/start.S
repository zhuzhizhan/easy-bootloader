.global _start
.global IRQ_handle

_start:
	// 关闭看门狗
	ldr	r0, =0xE2700000			
	mov	r1, #0
	str	r1, [r0]

	// 设置栈，以便调用c函数
	ldr	sp, =0x40000000		

	// 汇编初始化时钟		
	bl clock_init	

	// 调用main函数
	bl main	

