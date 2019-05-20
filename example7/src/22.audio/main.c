#include "audio.h"
#include "stdio.h"

#define IISTXD		(*(volatile unsigned int *)0xEEE30010)
#define IISCON  	(*(volatile unsigned int *)0xEEE30000)

void uart_init(void);

void main(void)
{
	printf("Audio Test\r\n");
	
	int offset = 0x2E;						// 音频数据开始的地方
	short * p = (short *)0x22000000;		// 音频文件应该位于的位置
	
	iic_init();								// 初始化i2c

	wm8960_init();							// 初始化wm8960
	
	iis_init();								// 初始化iis
	
	// 循环播放音频文件
	while (1)
	{
		// polling  Primary Tx FIFO0 full status indication. 
		while((IISCON & (1<<8)) == (1<<8));
		
		IISTXD = *(p+offset);				// 每次发送2byte
		
		offset++;
		if (offset > (882046-0x2e) /2)		// 有多少个2byte = (文件大小-偏移)/2
			offset = 0x2E;
	}
}
