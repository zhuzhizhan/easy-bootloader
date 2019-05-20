
int shell_parse(char * buf, char * argv[])
{
	int argc = 0;
	int state = 0;
	
	// ��������ַ�
	while (*buf)
	{
		// ���һ������
		if (*buf != ' ' && state == 0)
		{
			argv[argc++] = buf;
			state = 1;
		}
		
		// �����ո�
		if (*buf == ' ' && state == 1)
		{
			*buf = '\0';
			state = 0;
		}
		
		buf++;	
	}
	
	return argc;
}
