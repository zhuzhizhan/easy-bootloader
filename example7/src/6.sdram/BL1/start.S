.global _start

_start:
	// 关闭看门狗
	ldr	r0, =0xE2700000			
	mov	r1, #0
	str	r1, [r0]

	// 设置栈，以便调用c函数
	ldr	sp, =0xD0037D80
				
	// 初始化内存
	bl mem_init						

	// 重定位，并跳到DRAM中运行				
	bl copy_code_to_dram

halt:
	b halt
