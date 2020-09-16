
.global _start
	
_start:
	ldr sp, =0xD0030000  /* 初始化栈，因为后面要调用C函数 */
	bl clock_init              /* 初始化时钟 */
	bl ddr_init                   /* 初始化内存 */
	bl nand_init               /* 初始化NAND */


	ldr r0, =0x36000000   /* 要拷贝到DDR中的位置 */
	ldr r1, =0x0                 /* 从NAND的0地址开始拷贝 */
	ldr r2, =bss_start         /* BSS段的开始地址 */
	sub r2,r2,r0                  /* 要拷贝的大小 */
	bl nand_read              /* 拷贝数据 */

clean_bss:
	ldr r0, =bss_start
	ldr r1, =bss_end
	mov r3, #0
	cmp r0, r1
	ldreq pc, =on_ddr
clean_loop:
	str r3, [r0], #4
	cmp r0, r1	
	bne clean_loop		
	ldr pc, =on_ddr

on_ddr:
	ldr sp, =0x33000000    /* 重新初始化栈，指向内存 */
	ldr pc, =main

