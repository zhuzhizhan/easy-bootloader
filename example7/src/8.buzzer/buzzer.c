
#define GPD0CON		(*(volatile unsigned long *)0xE02000A0)
#define GPD0DAT		(*(volatile unsigned long *)0xE02000A4)

// ≥ı ºªØbuzzer
void buzzer_init(void)
{	
	GPD0CON |= 1<<0;	

}

void buzzer_on(void)
{
	GPD0DAT |= 1<<0;
}

void buzzer_off(void)
{
	GPD0DAT &= ~(1<<0);	
}
