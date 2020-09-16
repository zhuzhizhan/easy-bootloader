#define GPA0CON  	(*(volatile unsigned int *)0xE0200000) 

#define ULCON0  	(*(volatile unsigned int *)0xE2900000) 
#define UCON0  		(*(volatile unsigned int *)0xE2900004) 
#define UTRSTAT0  	(*(volatile unsigned int *)0xE2900010)
#define UTXH0  		(*(volatile unsigned char *)0xE2900020) 
#define URXH0  		(*(volatile unsigned char *)0xE2900024) 
#define UBRDIV0 	(*(volatile unsigned int *)0xE2900028) 
#define UDIVSLOT0  	(*(volatile unsigned int *)0xE290002C)

void uart_init(void)
{
	/* ���ö�ӦGPIO����UART0 */
	GPA0CON |= 0x22;
			
	/* ����UART0�Ĵ��� */
	/* bit[1:0]:0x3 = 8λ����λ
	 * ����λĬ��,��1λֹͣλ����У�飬����ģʽ
	 */
	ULCON0 |= (0x3<<0);
	/*
	 * Receive Mode [1:0]:1 = ���ղ��ò�ѯ�����ж�ģʽ
	 * Transmit Mode[3:2]:1 = ���Ͳ��ò�ѯ�����ж�ģʽ
	 * bit[6]:1 = ���������ж�
	 * bit[10]:0 = ʱ��ԴΪPCLK
	 */
	UCON0 = (1<<6)|(1<<2)|(1<<0);
	/* ���ò�����(��ϸ��Ϣ��ο��ֲ����ѧϰ�ռ�)
	 * DIV_VAL = UBRDIVn + (num of 1's in UDIVSLOTn)/16
	 * DIV_VAL = (PCLK / (bps x 16)) - 1
	 */
	UBRDIV0 = 0x23;
	UDIVSLOT0 = 0x808;

	return;
}

char uart_getchar(void)
{
	char c;
	/* ��ѯ״̬�Ĵ�����ֱ������Ч���� */
	while (!(UTRSTAT0 & (1<<0)));
	
	c = URXH0; /* ��ȡ���ռĴ�����ֵ */
		
	return c;
}

void uart_putchar(char c)
{
	/* ��ѯ״̬�Ĵ�����ֱ�����ͻ���Ϊ�� */
	while (!(UTRSTAT0 & (1<<2)));
	
	UTXH0 = c; /* д�뷢�ͼĴ��� */
	
	return;
}

