char getc(void);
void putc(char c);



int putchar(int c)
{
	if (c == '\n')
		putc('\r');
	
	if (c == '\b')
	{
		putc('\b');
		putc(' ');
	}
			
	putc((char)c);
	
	return c;	
}

int getchar(void)
{
	int c;
	
	c = (int)getc();

	if (c == '\r')
		return '\n';
	
	return c;
}

int puts(const char * s)
{
	while (*s)
		putchar(*s++);
		
	putchar('\n');

	return 0;
}

char * gets(char * s)
{
	char * p = s;
	
	while ((*p = getchar()) != '\n')
	{
		if (*p != '\b')
			putchar(*p++);
		else	
			if (p > s)
				putchar(*p--);	
	}
	
	*p = '\0';
	putchar('\n');
		
	return s;
}
