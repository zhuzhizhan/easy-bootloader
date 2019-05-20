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

	// ���ÿ��Ź��жϵ��жϴ�����
	intc_setvectaddr(NUM_WDT,isr_wtd);
	// ʹ�ܿ��Ź��ж�
	intc_enable(NUM_WDT);

	// ���Կ��Ź��Ķ�ʱ����
	wtd_operate(0,1,0,1,100,100000000,100000000);
}

//���Ź��жϴ�����
void isr_wtd()
{
	//��¼�жϷ�������
	static int wtdcounter=0;
	printf("%d\r\n",++wtdcounter);

	// ���Ź�����ж����
	WTCLRINT = 1;   		
	// VIC����ж����
	intc_clearvectaddr();
	if(wtdcounter==5)
	{
		// ���Ź�reset
		printf("waiting system reset\r\n");
		wtd_operate(1,1,0,1,100,100000000,100000000);
	}
}

void wtd_operate(unsigned long uenreset, unsigned long uenint, unsigned long uselectclk, unsigned long uenwtd, unsigned long uprescaler,unsigned long uwtdat, unsigned long uwtcnt)
{
	WTDAT = uwtdat;
	WTCNT = uwtcnt;
	
	/*uenreset: �Ƿ�ʹ��reset
	 *uenint: �Ƿ�ʹ���ж�
	 *uselectclk: ��Ƶϵ��
	 *uenwtd:�Ƿ�������ʱ��
	 *uprescaler:Ԥ��Ƶϵ��
	 */
	WTCON = (uenreset<<0)|(uenint<<2)|(uselectclk<<3)|(uenwtd<<5)|((uprescaler)<<8);
}
