#include "stdio.h"

void uart_init(void);
int command_do(int argc, char * argv[]);
int shell_parse(char * buf, char * argv[]);
char * gets(char * s);


int main(void)
{
	char buf[64];
	int argc = 0;
	char * argv[10];

	//��ʼ������
	uart_init();

	while (1)
	{
		printf("mini210s: ");
		gets(buf);								// �ȴ��û���������

		argc = shell_parse(buf, argv);			// ��������

		command_do(argc, argv);					// ��������
	}
}
