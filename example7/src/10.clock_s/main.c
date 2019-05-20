#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)

#define 	GPJ2_0_OUTPUT 	(1<<(0*4))
#define 	GPJ2_1_OUTPUT 	(1<<(1*4))
#define 	GPJ2_2_OUTPUT 	(1<<(2*4))
#define 	GPJ2_3_OUTPUT 	(1<<(3*4))

void delay(unsigned long count)
{
	volatile unsigned long i = count;
	while (i--);
}

void main()
{
	// 设置GPJ2CON0/1/2/3为输出引脚
	GPJ2CON = GPJ2_0_OUTPUT | GPJ2_1_OUTPUT | GPJ2_2_OUTPUT | GPJ2_3_OUTPUT;
	
	while (1)
	{
		// LED on
		GPJ2DAT = 0;			
		delay(0x100000);	
		
		// LED off
		GPJ2DAT = 0xf;			
		delay(0x100000);
	}
}

