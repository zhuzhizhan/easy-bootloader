#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)

// ÑÓÊ±º¯Êý
void delay(unsigned long count)
{
	volatile unsigned long i = count;
	while (i--)
		;
}


void main()
{
	buzzer_init();
	
	while(1)				
	{
		buzzer_on();		
		delay(0x50000);
		buzzer_off();		
		delay(0x50000);
	}
}


