#include "int.h"
#include "stdio.h"

#define WDT_BASE			(0xE2700000)
#define WTCON			( *((volatile unsigned long *)(WDT_BASE+0x00)) )
#define WTDAT			( *((volatile unsigned long *)(WDT_BASE+0x04)) )
#define WTCNT			( *((volatile unsigned long *)(WDT_BASE+0x08)) )
#define WTCLRINT			( *((volatile unsigned long *)(WDT_BASE+0x0C)) )

void isr_wtd(void);
void wtd_operate(unsigned long uenreset, unsigned long uenint, unsigned long uselectclk, unsigned long uenwtd, unsigned long uprescaler,unsigned long uwtdat, unsigned long uwtcnt);

void wtd_test(void)
{
	printf("\r\n\r\n#############watchdog test#############\r\n");

	// 设置看门狗中断的中断处理函数
	intc_setvectaddr(NUM_WDT,isr_wtd);
	// 使能看门狗中断
	intc_enable(NUM_WDT);

	// 测试看门狗的定时功能
	wtd_operate(0,1,0,1,100,100000000,100000000);
}

//看门狗中断处理函数
void isr_wtd()
{
	//记录中断发生次数
	static int wtdcounter=0;
	printf("%d\r\n",++wtdcounter);

	// 看门狗相关中断清除
	WTCLRINT = 1;   		
	// VIC相关中断清除
	intc_clearvectaddr();
	if(wtdcounter==5)
	{
		// 看门狗reset
		printf("waiting system reset\r\n");
		wtd_operate(1,1,0,1,100,100000000,100000000);
	}
}

void wtd_operate(unsigned long uenreset, unsigned long uenint, unsigned long uselectclk, unsigned long uenwtd, unsigned long uprescaler,unsigned long uwtdat, unsigned long uwtcnt)
{
	WTDAT = uwtdat;
	WTCNT = uwtcnt;
	
	/*uenreset: 是否使能reset
	 *uenint: 是否使能中断
	 *uselectclk: 分频系数
	 *uenwtd:是否启动定时器
	 *uprescaler:预分频系数
	 */
	WTCON = (uenreset<<0)|(uenint<<2)|(uselectclk<<3)|(uenwtd<<5)|((uprescaler)<<8);
}
