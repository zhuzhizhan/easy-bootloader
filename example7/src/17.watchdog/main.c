#include "int.h"
#include "stdio.h"

#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)
#define 	GPH2CON 	(*(volatile unsigned long *) 0xE0200C40)
#define 	GPH2DAT		(*(volatile unsigned long *) 0xE0200C44)

#define 	GPJ2_0_OUTPUT 	(1<<(0*4))
#define 	GPJ2_1_OUTPUT 	(1<<(1*4))
#define 	GPJ2_2_OUTPUT 	(1<<(2*4))
#define 	GPJ2_3_OUTPUT 	(1<<(3*4))

#define 	GPH2_0_EINT16 	(0xf<<(0*4))
#define 	GPH2_1_EINT17 	(0xf<<(1*4))
#define 	GPH2_2_EINT18 	(0xf<<(2*4))
#define 	GPH2_3_EINT19 	(0xf<<(3*4))

#define		EXT_INT_0_CON  			( *((volatile unsigned long *)0xE0200E00) )
#define		EXT_INT_1_CON  			( *((volatile unsigned long *)0xE0200E04) )
#define		EXT_INT_2_CON  			( *((volatile unsigned long *)0xE0200E08) )
#define		EXT_INT_3_CON  			( *((volatile unsigned long *)0xE0200E0C) )

#define		EXT_INT_0_MASK   		( *((volatile unsigned long *)0xE0200F00) )
#define		EXT_INT_1_MASK   		( *((volatile unsigned long *)0xE0200F04) )
#define		EXT_INT_2_MASK   		( *((volatile unsigned long *)0xE0200F08) )
#define		EXT_INT_3_MASK   		( *((volatile unsigned long *)0xE0200F0C) )

#define		EXT_INT_0_PEND   		( *((volatile unsigned long *)0xE0200F40) )
#define		EXT_INT_1_PEND   		( *((volatile unsigned long *)0xE0200F44) )
#define		EXT_INT_2_PEND   		( *((volatile unsigned long *)0xE0200F48) )
#define		EXT_INT_3_PEND   		( *((volatile unsigned long *)0xE0200F4C) )

void uart_init();
void wtd_test();

int main(void)
{
	// 初始化串口
	uart_init();

	//中断相关初始化
	system_initexception();						

	// 测试看门狗
	wtd_test();
	
	while(1);
}
