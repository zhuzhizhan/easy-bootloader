
.global _start
	
_start:
	ldr sp, =0xD0030000  /* ��ʼ��ջ����Ϊ����Ҫ����C���� */
	bl clock_init              /* ��ʼ��ʱ�� */
	bl ddr_init                   /* ��ʼ���ڴ� */
	bl nand_init               /* ��ʼ��NAND */


	ldr r0, =0x36000000   /* Ҫ������DDR�е�λ�� */
	ldr r1, =0x0                 /* ��NAND��0��ַ��ʼ���� */
	ldr r2, =bss_start         /* BSS�εĿ�ʼ��ַ */
	sub r2,r2,r0                  /* Ҫ�����Ĵ�С */
	bl nand_read              /* �������� */

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
	ldr sp, =0x33000000    /* ���³�ʼ��ջ��ָ���ڴ� */
	ldr pc, =main

