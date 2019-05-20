#include "int.h"
#include "stdio.h"

#define		PWMTIMER_BASE			(0xE2500000)

#define		TCFG0    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x00)) )
#define		TCFG1    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x04)) )
#define		TCON      	( *((volatile unsigned long *)(PWMTIMER_BASE+0x08)) )
#define		TCNTB0    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x0C)) )
#define		TCMPB0    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x10)) )
#define		TCNTO0    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x14)) )
#define		TCNTB1    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x18)) )
#define		TCMPB1    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x1C)) )
#define		TCNTO1    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x20)) )
#define		TCNTB2    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x24)) )
#define		TCMPB2    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x28)) )
#define		TCNTO2    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x2C)) )
#define		TCNTB3    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x30)) )
#define		TCMPB3    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x34)) )
#define		TCNTO3    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x38)) )
#define		TCNTB4    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x3C)) )
#define		TCNTO4    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x40)) )
#define		TINT_CSTAT 	( *((volatile unsigned long *)(PWMTIMER_BASE+0x44)) )

void pwm_stopall(void);
void timer_request(void);
void irq_handler(void);
void timer_init(unsigned long utimer,unsigned long uprescaler,unsigned long udivider,unsigned long utcntb,unsigned long utcmpb);
void irs_timer();


// ���ڼ�¼�жϷ����Ĵ���
int counter;


void timer_request(void)
{
	printf("\r\n#############Timer test############\r\n");

	// ��ֹ����timer
	pwm_stopall();								

	counter = 0;

	// ����timer0�жϵ��жϴ�����
	intc_setvectaddr(NUM_TIMER0,irs_timer);	
	
	// ʹ��timer0�ж�
	intc_enable(NUM_TIMER0);
	
	// ����timer0
	timer_init(0,65,4,62500,0);		
}

// ֹͣ����timer
void pwm_stopall(void)
{
	TCON = 0;
}


// timer0�жϵ��жϴ�����
void irs_timer()
{
	unsigned long uTmp;


	//��timer0���ж�״̬�Ĵ���
	uTmp = TINT_CSTAT;
	TINT_CSTAT = uTmp;      
	
	// ��ӡ�жϷ�������
	printf("Timer0IntCounter = %d \r\n",counter++);
	
	// vic��ص��ж����
	intc_clearvectaddr();
}

void timer_init(unsigned long utimer,unsigned long uprescaler,unsigned long udivider,unsigned long utcntb,unsigned long utcmpb)
{
	unsigned long temp0;

	// ��ʱ��������ʱ�� = PCLK / ( {prescaler value + 1} ) / {divider value} = PCLK/(65+1)/16=62500hz

	//����Ԥ��Ƶϵ��Ϊ66
	temp0 = TCFG0;
	temp0 = (temp0 & (~(0xff00ff))) | ((uprescaler-1)<<0);
	TCFG0 = temp0;

	// 16��Ƶ
	temp0 = TCFG1;
	temp0 = (temp0 & (~(0xf<<4*utimer))& (~(1<<20))) |(udivider<<4*utimer);
	TCFG1 = temp0;

	// 1s = 62500hz
	TCNTB0 = utcntb;
	TCMPB0 = utcmpb;

	// �ֶ�����
	TCON |= 1<<1;

	// ���ֶ�����λ
	TCON &= ~(1<<1);

	// �Զ����غ�����timer0
	TCON |= (1<<0)|(1<<3);

	// ʹ��timer0�ж�
	temp0 = TINT_CSTAT;
	temp0 = (temp0 & (~(1<<utimer)))|(1<<(utimer));
	TINT_CSTAT = temp0;
}