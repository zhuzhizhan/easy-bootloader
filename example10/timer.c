#include "lib.h"

#define	GPD0CON     (*(volatile unsigned int *)0xE02000A0) 

#define	TCFG0           (*(volatile unsigned int *)0xE2500000) 
#define	TCFG1           (*(volatile unsigned int *)0xE2500004) 
#define	TCON            (*(volatile unsigned int *)0xE2500008) 
#define	TCNTB0         (*(volatile unsigned int *)0xE250000C) 
#define	TCMPB0        (*(volatile unsigned int *)0xE2500010) 
#define	TCNTO0        (*(volatile unsigned int *)0xE2500014) 

void pwm_init(void)
{
	/* 配置为GPD0_0用于PWM输出 */
	GPD0CON |= (0x2 << 0);     /* TOUT_0 */
}

void timer0_init(void)
{
	/* 设置时钟源
	 * Timer0 input clock Frequency = 66700000 / ( {prescaler + 1} ) / {divider value} 
	 *      = 66700000 / (1+1) / 1
	 *      = 33350000( 即1s计数33350000次 )
	 */
	TCFG0 &= ~(0xff);
	TCFG0  |= 1;           /* Prescaler = 1 */
	TCFG1  &= ~0xf;     /* 0000 = 1/1 */

	/* 设置TCNTB0(即PWM的频率) */
	TCNTB0 = 33350;   /* PWM的频率为1KHz */							
	/* 设置TCMPB0(即PWM的占空比) */
	TCMPB0 = 16675;  /* 占空比为50% */

	TCON &= ~(1<<2);  /* 不进行电平反转(即引脚初始值为0) */
	TCON |= (1<<3);    /* auto-reload */
}

void pwm_start(void)
{
	TCON |= (1<<1);   /* set manual update */
	TCON |= (1<<0);   /* start timer 0 */
	TCON &= ~(1<<1); /* clean manual update */
}

void pwm_stop(void)
{
	TCON &= ~(1<<0);   /* stop timer 0 */
}

