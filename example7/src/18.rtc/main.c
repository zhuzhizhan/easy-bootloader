#include "int.h"
#include "stdio.h"
#include "rtc.h"

void uart_init(void);

int main(void)
{
	int c = 0;

	// ��ʼ������
	uart_init();

	// �ж���س�ʼ��
	system_initexception();						

	// ��ӡ�˵�
	while(1)
	{
		printf("\r\n###############rtc test##############\r\n");
		printf("[d] Display rtc realtime(hour:min:sec:weekday date/month/year)\r\n");
		printf("[s] Reset rtc realtime(12:0:0:Tuesday 1/1/2012) \r\n");
		printf("Enter your choice:");
		c = getc();
		printf("%c\r\n",c);
		switch(c)
		{
			case 'd':
				// ��ӡʱ��
				rtc_realtime_display();
				break;
			case 's':
				// ����ʱ��
				rtc_settime();
				break;
		}
	}
	return 0;
}
