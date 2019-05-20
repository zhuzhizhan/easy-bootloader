void uart_init(void);

int main()
{
	char c;

	// 初始化串口 
	uart_init();	

	while (1)
	{
		// 开发板接收字符
		c = getc();
		
		// 开发板发送字符c+1
		putc(c+1);		
	}

	return 0;
}
