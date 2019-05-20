#include "stdio.h"
#include "int.h"


#define 	GPH2CON 	(*(volatile unsigned long *) 0xE0200C40)
#define 	GPH2DAT		(*(volatile unsigned long *) 0xE0200C44)

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


// 延时函数
void delay(unsigned long count)
{
	volatile unsigned long i = count;
	while (i--);
}

void isr_key(void)
{
	printf("we get company:EINT16_31\r\n");
	
	// clear VIC0ADDR
	intc_clearvectaddr();					
	// clear pending bit	
	EXT_INT_2_PEND |= 1<<0;					
}

int main(void)
{
	int c = 0;

	// 初始化串口
	uart_init();

	// 中断相关初始化
	system_initexception();						
	
	printf("**************Int test *************** \r\n");

	// 外部中断相关的设置
	// 1111 = EXT_INT[16] 
	GPH2CON |= 0xF;							
	// 010 = Falling edge triggered
	EXT_INT_2_CON |= 1<<1;			
	// unmasked
	EXT_INT_2_MASK &= ~(1<<0);
	
	// 设置中断EINT16_31的处理函数
	intc_setvectaddr(NUM_EINT16_31, isr_key);	
	// 使能中断EINT16_31
	intc_enable(NUM_EINT16_31);
	
	while (1)
	{   
		printf("%d\r\n",c++);
		delay(0x100000);
	}
}
