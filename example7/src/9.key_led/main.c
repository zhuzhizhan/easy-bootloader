
// LED
#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)

// KEY
#define 	GPH2CON 	(*(volatile unsigned long *) 0xE0200C40)
#define 	GPH2DAT		(*(volatile unsigned long *) 0xE0200C44)

#define 	GPJ2_0_OUTPUT 	(1<<(0*4))
#define 	GPJ2_1_OUTPUT 	(1<<(1*4))
#define 	GPJ2_2_OUTPUT 	(1<<(2*4))
#define 	GPJ2_3_OUTPUT 	(1<<(3*4))

#define 	GPH2_0_INTPUT 	~(0xf<<(0*4))
#define 	GPH2_1_INTPUT 	~(0xf<<(1*4))
#define 	GPH2_2_INTPUT 	~(0xf<<(2*4))
#define 	GPH2_3_INTPUT 	~(0xf<<(3*4))

void main()
{
	unsigned long dat;

	// LED:设置寄存器GPJ2CON0使GPJ2_0/1/2/3引脚为输出功能
	GPJ2CON = GPJ2_0_OUTPUT|GPJ2_1_OUTPUT|GPJ2_2_OUTPUT|GPJ2_3_OUTPUT;
	
	// KEY：设置寄存器GPH2CON0使GPH2_0/1/2/3引脚为输入功能
	GPH2CON = GPH2_0_INTPUT&GPH2_1_INTPUT&GPH2_2_INTPUT&GPH2_3_INTPUT;


	while(1)
	{
		// 读取KEY相关的引脚值，用于判断KEY是否被按下
		dat = GPH2DAT;

		// 判断KEY1:GPH2_0
		if(dat & (1<<0))				// KEY1被按下，则LED1亮，否则LED1灭
			GPJ2DAT |= 1<<0;			// OFF
		else
			GPJ2DAT &= ~(1<<0);			// ON

		// 判断KEY2:GPH2_1
		if(dat & (1<<1))				// KEY2被按下，则LED2亮，否则LED2灭
			GPJ2DAT |= 1<<1;			
		else
			GPJ2DAT &= ~(1<<1);

		// 判断KEY3:GPH2_2
		if(dat & (1<<2))				// KEY3被按下，则LED3亮，否则LED3灭
			GPJ2DAT |= (1<<2);
		else
			GPJ2DAT &= ~(1<<2);

		// 判断KEY4:GPH2_3
		if(dat & (1<<3))				// KEY4被按下，则LED4亮，否则LED4灭
			GPJ2DAT |= 1<<3;
		else
			GPJ2DAT &= ~(1<<3);

	}
}


