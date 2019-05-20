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

	//初始化串口
	uart_init();

	while (1)
	{
		printf("mini210s: ");
		gets(buf);								// 等待用户输入命令

		argc = shell_parse(buf, argv);			// 解析命令

		command_do(argc, argv);					// 运行命令
	}
}
