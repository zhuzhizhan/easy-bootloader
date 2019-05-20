#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)

// —” ±∫Ø ˝
void delay(unsigned long count)
{
	volatile unsigned long i = count;
	while (i--)
		;
}

void led_blink()				//LED …¡À∏
{
	GPJ2CON = 0x00001111;		// ≈‰÷√“˝Ω≈
	while(1)					// …¡À∏
	{
		GPJ2DAT = 0;			// LED on
		delay(0x1000000);
		GPJ2DAT = 0xf;			// LED off
		delay(0x1000000);
	}
}


