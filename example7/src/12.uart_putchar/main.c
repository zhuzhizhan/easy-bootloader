void uart_init(void);

int main()
{
	char c;

	// ��ʼ������ 
	uart_init();	

	while (1)
	{
		// ����������ַ�
		c = getc();
		
		// �����巢���ַ�c+1
		putc(c+1);		
	}

	return 0;
}
