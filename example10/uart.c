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
	/* 设置对应GPIO用于UART0 */
	GPA0CON |= 0x22;
			
	/* 设置UART0寄存器 */
	/* bit[1:0]:0x3 = 8位数据位
	 * 其他位默认,即1位停止位，无校验，正常模式
	 */
	ULCON0 |= (0x3<<0);
	/*
	 * Receive Mode [1:0]:1 = 接收采用查询或者中断模式
	 * Transmit Mode[3:2]:1 = 发送采用查询或者中断模式
	 * bit[6]:1 = 产生错误中断
	 * bit[10]:0 = 时钟源为PCLK
	 */
	UCON0 = (1<<6)|(1<<2)|(1<<0);
	/* 设置波特率(详细信息请参考手册或者学习日记)
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
	/* 查询状态寄存器，直到有有效数据 */
	while (!(UTRSTAT0 & (1<<0)));
	
	c = URXH0; /* 读取接收寄存器的值 */
		
	return c;
}

void uart_putchar(char c)
{
	/* 查询状态寄存器，直到发送缓存为空 */
	while (!(UTRSTAT0 & (1<<2)));
	
	UTXH0 = c; /* 写入发送寄存器 */
	
	return;
}

