#include "audio.h"
#include "stdio.h"

#define IISTXD		(*(volatile unsigned int *)0xEEE30010)
#define IISCON  	(*(volatile unsigned int *)0xEEE30000)

void uart_init(void);

void main(void)
{
	printf("Audio Test\r\n");
	
	int offset = 0x2E;						// ��Ƶ���ݿ�ʼ�ĵط�
	short * p = (short *)0x22000000;		// ��Ƶ�ļ�Ӧ��λ�ڵ�λ��
	
	iic_init();								// ��ʼ��i2c

	wm8960_init();							// ��ʼ��wm8960
	
	iis_init();								// ��ʼ��iis
	
	// ѭ��������Ƶ�ļ�
	while (1)
	{
		// polling  Primary Tx FIFO0 full status indication. 
		while((IISCON & (1<<8)) == (1<<8));
		
		IISTXD = *(p+offset);				// ÿ�η���2byte
		
		offset++;
		if (offset > (882046-0x2e) /2)		// �ж��ٸ�2byte = (�ļ���С-ƫ��)/2
			offset = 0x2E;
	}
}
