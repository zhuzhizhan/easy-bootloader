#include "stdio.h"

#define		ADCTS_PRSCVL		65
#define 	ADCTS_BASE			0xE1700000
#define 	TSADCCON0			( *((volatile unsigned long *)(ADCTS_BASE+0x0)) )
#define    	TSCON0				( *((volatile unsigned long *)(ADCTS_BASE+0x4)) )
#define    	TSDLY0				( *((volatile unsigned long *)(ADCTS_BASE+0x8)) )
#define    	TSDATX0				( *((volatile unsigned long *)(ADCTS_BASE+0xc)) )
#define    	TSDATY0				( *((volatile unsigned long *)(ADCTS_BASE+0x10)) )
#define    	TSPENSTAT0			( *((volatile unsigned long *)(ADCTS_BASE+0x14)) )
#define    	CLRINTADC0			( *((volatile unsigned long *)(ADCTS_BASE+0x18)) )
#define  	ADCMUX				( *((volatile unsigned long *)(ADCTS_BASE+0x1c)) )
#define		CLRINTPEN0			( *((volatile unsigned long *)(ADCTS_BASE+0x20)) )

// 延时函数
void delay(unsigned long count)
{
	volatile unsigned long i = count;
	while (i--)
		;
}

// 使用查询方式读取A/D转换值
int read_adc(int ch)
{
    // 使能预分频功能，设置A/D转换器的时钟 = PCLK/(65+1)
	TSADCCON0 = (1<<16)|(1 << 14) | (65 << 6);

    // 清除位[2]，设为普通转换模式,禁止read start
	TSADCCON0 &= ~((1<<2)|(1<<1));

	// 选择通道
	ADCMUX = 0;

    // 设置位[0]为1，启动A/D转换
    TSADCCON0 |= (1 << 0);

    // 当A/D转换真正开始时，位[0]会自动清0
    while (TSADCCON0 & (1 << 0));

    // 检测位[15]，当它为1时表示转换结束
    while (!(TSADCCON0 &  (1 << 15)) );

    // 读取数据
    return (TSDATX0 & 0xfff);
}

void adc_test(void)
{
	printf("\r\n##################adc test################\r\n");
	while(1)
	{
		printf("adc = %d\r\n",read_adc(0));
		delay(0x100000);
	}
}