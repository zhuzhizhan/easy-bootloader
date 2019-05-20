#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)

void delay(int r0)					// —” ±
{
    volatile int count = r0;
    while (count--)
        ;
}

void led_blink()					// LED…¡À∏
{
	GPJ2CON = 0x00001111;			// ≈‰÷√“˝Ω≈
	while(1)							
	{
		GPJ2DAT = 0;				// LED on
		delay(0x100000);
		GPJ2DAT = 0xf;				// LED off
		delay(0x100000);
	}
}

