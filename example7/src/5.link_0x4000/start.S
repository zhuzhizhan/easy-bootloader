.global _start

_start:
	// �رտ��Ź�
	ldr	r0, =0xE2700000			
	mov	r1, #0
	str	r1, [r0]

	// ����ջ���Ա����c����
	ldr	sp, =0xD0037D80					
			
										
	// �ض�λ						
	// _start��ǰ��λ�ڵĵ�ַ��0xd0022000
	adr r0, _start  		
	// _start�����ӵ�ַ:0xd0024000
	ldr r1, =_start 	
	// bss�ε���ʼ��ַ
	ldr r2, =bss_start
	cmp r0, r1
	beq clean_bss
	
copy_loop:
	ldr r3, [r0], #4    // Դ
	str r3, [r1], #4	// Ŀ��
	cmp r1, r2
	bne copy_loop


	// ��bss��
clean_bss:
	ldr r0, =bss_start					
	ldr r1, =bss_end
	cmp r0, r1
	beq run_on_dram
	mov r2, #0
clear_loop:
	str r2, [r0], #4
	cmp r0, r1
	bne clear_loop


	// ��ת
run_on_dram:			 
	ldr pc, =main						


halt:
	b halt
