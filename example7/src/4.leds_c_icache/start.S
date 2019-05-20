//#define CONFIG_SYS_ICACHE_OFF  

.global _start

_start:
	// 关闭看门狗
	ldr	r0, =0xE2700000
    mov	r1, #0
    str	r1, [r0]

    // 打开icache可提高运行速度
#ifdef CONFIG_SYS_ICACHE_OFF
    // clear bit 12 (I) I-cache
    bic	r0, r0, #0x00001000
#else
    // set bit 12 (I) I-cache
    orr	r0, r0, #0x00001000
#endif
    mcr	p15, 0, r0, c1, c0, 0

	// 设置栈，以便调用c函数
	ldr	sp, =0xD0037D80

	// 调用c函数,LED闪烁
	bl	led_blink

halt:
	b halt

