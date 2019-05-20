#define GPA0CON 		( *((volatile unsigned long *)0xE0200000) )		
#define GPA1CON 		( *((volatile unsigned long *)0xE0200020) )

// UART��ؼĴ���
#define ULCON0 			( *((volatile unsigned long *)0xE2900000) )		
#define UCON0 			( *((volatile unsigned long *)0xE2900004) )
#define UFCON0 			( *((volatile unsigned long *)0xE2900008) )
#define UMCON0 			( *((volatile unsigned long *)0xE290000C) )
#define UTRSTAT0 		( *((volatile unsigned long *)0xE2900010) )
#define UERSTAT0 		( *((volatile unsigned long *)0xE2900014) )
#define UFSTAT0 		( *((volatile unsigned long *)0xE2900018) )
#define UMSTAT0 		( *((volatile unsigned long *)0xE290001C) )
#define UTXH0 			( *((volatile unsigned long *)0xE2900020) )
#define URXH0 			( *((volatile unsigned long *)0xE2900024) )
#define UBRDIV0 		( *((volatile unsigned long *)0xE2900028) )
#define UDIVSLOT0 		( *((volatile unsigned long *)0xE290002C) )
#define UINTP 			( *((volatile unsigned long *)0xE2900030) )
#define UINTSP 			( *((volatile unsigned long *)0xE2900034) )
#define UINTM 			( *((volatile unsigned long *)0xE2900038) )

#define UART_UBRDIV_VAL		35
#define UART_UDIVSLOT_VAL	0x1

// ��ʼ������ 
void uart_init()
{
	// 1 ������������RX/TX����
	GPA0CON = 0x22222222;
	GPA1CON = 0x2222;

	// 2 �������ݸ�ʽ��
	// ʹ��fifo
	UFCON0 = 0x1;
	// ������
	UMCON0 = 0x0;
	// ����λ:8, ��У��, ֹͣλ: 1
	ULCON0 = 0x3;
	// ʱ�ӣ�PCLK����ֹ�жϣ�ʹ��UART���͡�����
	UCON0  = 0x5;  				

	// 3 ���ò�����
	UBRDIV0 = UART_UBRDIV_VAL;
	UDIVSLOT0 = UART_UDIVSLOT_VAL;

}

// ����һ���ַ� 
char getc(void)
{
	// ���RX FIFO�գ��ȴ�
	while (!(UTRSTAT0 & (1<<0)));
	// ȡ����
	return URXH0;                   	
}

// ����һ���ַ�
void putc(char c)
{
	// ���TX FIFO�����ȴ�
	while (!(UTRSTAT0 & (1<<2)));
	// д����
	UTXH0 = c;                      	
}
