
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

	// LED:���üĴ���GPJ2CON0ʹGPJ2_0/1/2/3����Ϊ�������
	GPJ2CON = GPJ2_0_OUTPUT|GPJ2_1_OUTPUT|GPJ2_2_OUTPUT|GPJ2_3_OUTPUT;
	
	// KEY�����üĴ���GPH2CON0ʹGPH2_0/1/2/3����Ϊ���빦��
	GPH2CON = GPH2_0_INTPUT&GPH2_1_INTPUT&GPH2_2_INTPUT&GPH2_3_INTPUT;


	while(1)
	{
		// ��ȡKEY��ص�����ֵ�������ж�KEY�Ƿ񱻰���
		dat = GPH2DAT;

		// �ж�KEY1:GPH2_0
		if(dat & (1<<0))				// KEY1�����£���LED1��������LED1��
			GPJ2DAT |= 1<<0;			// OFF
		else
			GPJ2DAT &= ~(1<<0);			// ON

		// �ж�KEY2:GPH2_1
		if(dat & (1<<1))				// KEY2�����£���LED2��������LED2��
			GPJ2DAT |= 1<<1;			
		else
			GPJ2DAT &= ~(1<<1);

		// �ж�KEY3:GPH2_2
		if(dat & (1<<2))				// KEY3�����£���LED3��������LED3��
			GPJ2DAT |= (1<<2);
		else
			GPJ2DAT &= ~(1<<2);

		// �ж�KEY4:GPH2_3
		if(dat & (1<<3))				// KEY4�����£���LED4��������LED4��
			GPJ2DAT |= 1<<3;
		else
			GPJ2DAT &= ~(1<<3);

	}
}


