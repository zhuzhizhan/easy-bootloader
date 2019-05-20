#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)

// ��ʱ����
void delay(unsigned long count)
{
	volatile unsigned long i = count;
	while (i--)
		;
}

void main()				//LED ��˸
{
	GPJ2CON = 0x00001111;		// ��������
	while(1)					// ��˸
	{
		GPJ2DAT = 0;			// LED on
		delay(0x100000);
		GPJ2DAT = 0xf;			// LED off
		delay(0x100000);
	}
}


