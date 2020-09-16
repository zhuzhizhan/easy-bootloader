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
	/* ����ΪGPD0_0����PWM��� */
	GPD0CON |= (0x2 << 0);     /* TOUT_0 */
}

void timer0_init(void)
{
	/* ����ʱ��Դ
	 * Timer0 input clock Frequency = 66700000 / ( {prescaler + 1} ) / {divider value} 
	 *      = 66700000 / (1+1) / 1
	 *      = 33350000( ��1s����33350000�� )
	 */
	TCFG0 &= ~(0xff);
	TCFG0  |= 1;           /* Prescaler = 1 */
	TCFG1  &= ~0xf;     /* 0000 = 1/1 */

	/* ����TCNTB0(��PWM��Ƶ��) */
	TCNTB0 = 33350;   /* PWM��Ƶ��Ϊ1KHz */							
	/* ����TCMPB0(��PWM��ռ�ձ�) */
	TCMPB0 = 16675;  /* ռ�ձ�Ϊ50% */

	TCON &= ~(1<<2);  /* �����е�ƽ��ת(�����ų�ʼֵΪ0) */
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

