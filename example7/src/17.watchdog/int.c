#include "int.h"
#include "stdio.h"

// Interrupt
#define VIC0_BASE					(0xF2000000)
#define VIC1_BASE					(0xF2100000)
#define VIC2_BASE					(0xF2200000)
#define VIC3_BASE					(0xF2300000)

// VIC0
#define		VIC0IRQSTATUS			( *((volatile unsigned long *)(VIC0_BASE + 0x00)) )
#define		VIC0FIQSTATUS			( *((volatile unsigned long *)(VIC0_BASE + 0x04)) )
#define		VIC0RAWINTR				( *((volatile unsigned long *)(VIC0_BASE + 0x08)) )
#define		VIC0INTSELECT			( *((volatile unsigned long *)(VIC0_BASE + 0x0c)) )
#define		VIC0INTENABLE			( *((volatile unsigned long *)(VIC0_BASE + 0x10)) )
#define		VIC0INTENCLEAR			( *((volatile unsigned long *)(VIC0_BASE + 0x14)) )
#define		VIC0SOFTINT				( *((volatile unsigned long *)(VIC0_BASE + 0x18)) )
#define		VIC0SOFTINTCLEAR		( *((volatile unsigned long *)(VIC0_BASE + 0x1c)) )
#define		VIC0PROTECTION			( *((volatile unsigned long *)(VIC0_BASE + 0x20)) )
#define		VIC0SWPRIORITYMASK		( *((volatile unsigned long *)(VIC0_BASE + 0x24)) )
#define		VIC0PRIORITYDAISY		( *((volatile unsigned long *)(VIC0_BASE + 0x28)) )
#define 		VIC0VECTADDR			(VIC0_BASE + 0x100)
#define 		VIC0VECPRIORITY			( *((volatile unsigned long *)(VIC0_BASE + 0x200)) )
#define 		VIC0ADDR				( *((volatile unsigned long *)(VIC0_BASE + 0xf00)) )
#define 		VIC0PERID0				( *((volatile unsigned long *)(VIC0_BASE + 0xfe0)) )
#define 		VIC0PERID1				( *((volatile unsigned long *)(VIC0_BASE + 0xfe4)) )
#define 		VIC0PERID2				( *((volatile unsigned long *)(VIC0_BASE + 0xfe8)) )
#define 		VIC0PERID3				( *((volatile unsigned long *)(VIC0_BASE + 0xfec)) )
#define 		VIC0PCELLID0				( *((volatile unsigned long *)(VIC0_BASE + 0xff0)) )
#define 		VIC0PCELLID1				( *((volatile unsigned long *)(VIC0_BASE + 0xff4)) )
#define 		VIC0PCELLID2				( *((volatile unsigned long *)(VIC0_BASE + 0xff8)) )
#define 		VIC0PCELLID3				( *((volatile unsigned long *)(VIC0_BASE + 0xffc)) )

// VIC1
#define		VIC1IRQSTATUS			( *((volatile unsigned long *)(VIC1_BASE + 0x00)) )
#define		VIC1FIQSTATUS			( *((volatile unsigned long *)(VIC1_BASE + 0x04)) )
#define		VIC1RAWINTR				( *((volatile unsigned long *)(VIC1_BASE + 0x08)) )
#define		VIC1INTSELECT			( *((volatile unsigned long *)(VIC1_BASE + 0x0c)) )
#define		VIC1INTENABLE			( *((volatile unsigned long *)(VIC1_BASE + 0x10)) )
#define		VIC1INTENCLEAR			( *((volatile unsigned long *)(VIC1_BASE + 0x14)) )
#define		VIC1SOFTINT				( *((volatile unsigned long *)(VIC1_BASE + 0x18)) )
#define		VIC1SOFTINTCLEAR		( *((volatile unsigned long *)(VIC1_BASE + 0x1c)) )
#define		VIC1PROTECTION			( *((volatile unsigned long *)(VIC1_BASE + 0x20)) )
#define		VIC1SWPRIORITYMASK		( *((volatile unsigned long *)(VIC1_BASE + 0x24)) )
#define		VIC1PRIORITYDAISY		( *((volatile unsigned long *)(VIC1_BASE + 0x28)) )
#define 		VIC1VECTADDR			(VIC1_BASE + 0x100)
#define 		VIC1VECPRIORITY			( *((volatile unsigned long *)(VIC1_BASE + 0x200)) )
#define 		VIC1ADDR				( *((volatile unsigned long *)(VIC1_BASE + 0xf00)) )
#define 		VIC1PERID0				( *((volatile unsigned long *)(VIC1_BASE + 0xfe0)) )
#define 		VIC1PERID1				( *((volatile unsigned long *)(VIC1_BASE + 0xfe4)) )
#define 		VIC1PERID2				( *((volatile unsigned long *)(VIC1_BASE + 0xfe8)) )
#define 		VIC1PERID3				( *((volatile unsigned long *)(VIC1_BASE + 0xfec)) )
#define 		VIC1PCELLID0				( *((volatile unsigned long *)(VIC1_BASE + 0xff0)) )
#define 		VIC1PCELLID1				( *((volatile unsigned long *)(VIC1_BASE + 0xff4)) )
#define 		VIC1PCELLID2				( *((volatile unsigned long *)(VIC1_BASE + 0xff8)) )
#define 		VIC1PCELLID3				( *((volatile unsigned long *)(VIC1_BASE + 0xffc)) )

// VIC2
#define		VIC2IRQSTATUS			( *((volatile unsigned long *)(VIC2_BASE + 0x00)) )
#define		VIC2FIQSTATUS			( *((volatile unsigned long *)(VIC2_BASE + 0x04)) )
#define		VIC2RAWINTR				( *((volatile unsigned long *)(VIC2_BASE + 0x08)) )
#define		VIC2INTSELECT			( *((volatile unsigned long *)(VIC2_BASE + 0x0c)) )
#define		VIC2INTENABLE			( *((volatile unsigned long *)(VIC2_BASE + 0x10)) )
#define		VIC2INTENCLEAR			( *((volatile unsigned long *)(VIC2_BASE + 0x14)) )
#define		VIC2SOFTINT				( *((volatile unsigned long *)(VIC2_BASE + 0x18)) )
#define		VIC2SOFTINTCLEAR		( *((volatile unsigned long *)(VIC2_BASE + 0x1c)) )
#define		VIC2PROTECTION			( *((volatile unsigned long *)(VIC2_BASE + 0x20)) )
#define		VIC2SWPRIORITYMASK		( *((volatile unsigned long *)(VIC2_BASE + 0x24)) )
#define		VIC2PRIORITYDAISY		( *((volatile unsigned long *)(VIC2_BASE + 0x28)) )
#define 		VIC2VECTADDR			(VIC2_BASE + 0x100)
#define 		VIC2VECPRIORITY			( *((volatile unsigned long *)(VIC2_BASE + 0x200)) )
#define 		VIC2ADDR				( *((volatile unsigned long *)(VIC2_BASE + 0xf00)) )
#define 		VIC2PERID0				( *((volatile unsigned long *)(VIC2_BASE + 0xfe0)) )
#define 		VIC2PERID1				( *((volatile unsigned long *)(VIC2_BASE + 0xfe4)) )
#define 		VIC2PERID2				( *((volatile unsigned long *)(VIC2_BASE + 0xfe8)) )
#define 		VIC2PERID3				( *((volatile unsigned long *)(VIC2_BASE + 0xfec)) )
#define 		VIC2PCELLID0				( *((volatile unsigned long *)(VIC2_BASE + 0xff0)) )
#define 		VIC2PCELLID1				( *((volatile unsigned long *)(VIC2_BASE + 0xff4)) )
#define 		VIC2PCELLID2				( *((volatile unsigned long *)(VIC2_BASE + 0xff8)) )
#define 		VIC2PCELLID3				( *((volatile unsigned long *)(VIC2_BASE + 0xffc)) )

// VIC3
#define		VIC3IRQSTATUS			( *((volatile unsigned long *)(VIC3_BASE + 0x00)) )
#define	 	VIC3FIQSTATUS			( *((volatile unsigned long *)(VIC3_BASE + 0x04)) )
#define	 	VIC3RAWINTR				( *((volatile unsigned long *)(VIC3_BASE + 0x08)) )
#define	 	VIC3INTSELECT			( *((volatile unsigned long *)(VIC3_BASE + 0x0c)) )
#define	 	VIC3INTENABLE			( *((volatile unsigned long *)(VIC3_BASE + 0x10)) )
#define	 	VIC3INTENCLEAR			( *((volatile unsigned long *)(VIC3_BASE + 0x14)) )
#define	 	VIC3SOFTINT				( *((volatile unsigned long *)(VIC3_BASE + 0x18)) )
#define	 	VIC3SOFTINTCLEAR		( *((volatile unsigned long *)(VIC3_BASE + 0x1c)) )
#define	 	VIC3PROTECTION			( *((volatile unsigned long *)(VIC3_BASE + 0x20)) )
#define	 	VIC3SWPRIORITYMASK		( *((volatile unsigned long *)(VIC3_BASE + 0x24)) )
#define	 	VIC3PRIORITYDAISY		( *((volatile unsigned long *)(VIC3_BASE + 0x28)) )
#define 		VIC3VECTADDR			(VIC3_BASE + 0x100)
#define 		VIC3VECPRIORITY			( *((volatile unsigned long *)(VIC3_BASE + 0x200)) )
#define 		VIC3ADDR				( *((volatile unsigned long *)(VIC3_BASE + 0xf00)) )
#define 		VIC3PERID0				( *((volatile unsigned long *)(VIC3_BASE + 0xfe0)) )
#define 		VIC3PERID1				( *((volatile unsigned long *)(VIC3_BASE + 0xfe4)) )
#define 		VIC3PERID2				( *((volatile unsigned long *)(VIC3_BASE + 0xfe8)) )
#define 		VIC3PERID3				( *((volatile unsigned long *)(VIC3_BASE + 0xfec)) )
#define 		VIC3PCELLID0				( *((volatile unsigned long *)(VIC3_BASE + 0xff0)) )
#define 		VIC3PCELLID1				( *((volatile unsigned long *)(VIC3_BASE + 0xff4)) )
#define 		VIC3PCELLID2				( *((volatile unsigned long *)(VIC3_BASE + 0xff8)) )
#define 		VIC3PCELLID3				( *((volatile unsigned long *)(VIC3_BASE + 0xffc)) )

#define		_Exception_Vector			0xD0037400
#define		 pExceptionRESET			( *((volatile unsigned long *)(_Exception_Vector + 0x0)) )
#define		 pExceptionUNDEF			( *((volatile unsigned long *)(_Exception_Vector + 0x4)) )
#define		 pExceptionSWI			( *((volatile unsigned long *)(_Exception_Vector + 0x8)) )
#define 		pExceptionPABORT			( *((volatile unsigned long *)(_Exception_Vector + 0xc)) )
#define 		pExceptionDABORT			( *((volatile unsigned long *)(_Exception_Vector + 0x10)) )
#define		 pExceptionRESERVED		( *((volatile unsigned long *)(_Exception_Vector + 0x14)) )
#define 		pExceptionIRQ				( *((volatile unsigned long *)(_Exception_Vector + 0x18)) )
#define 		pExceptionFIQ				( *((volatile unsigned long *)(_Exception_Vector + 0x1c)) )


void exceptionundef(void)
{
	printf("undefined instruction exception.\n");
	while(1);
}

void exceptionswi(void)
{
	printf("swi exception.\n");
	while(1);
}

void exceptionpabort(void)
{
	printf("pabort exception.\n");
	while(1);
}

void exceptiondabort(void)
{

	printf("dabort exception.\n");
	while(1);
}

// 中断相关初始化
void system_initexception( void)
{
	// 设置中断向量表 
	pExceptionUNDEF	  =	(unsigned long)exceptionundef;
	pExceptionSWI  	  =	(unsigned long)exceptionswi;
	pExceptionPABORT  =	(unsigned long)exceptionpabort;
	pExceptionDABORT  =	(unsigned long)exceptiondabort;
	pExceptionIRQ	  =	(unsigned long)IRQ_handle;
	pExceptionFIQ	  =	(unsigned long)IRQ_handle;

	// 初始化中断控制器
	intc_init();

}



// 初始化中断控制器  
void intc_init(void)
{
	// 禁止所有中断
	VIC0INTENCLEAR = 0xffffffff;
	VIC1INTENCLEAR = 0xffffffff;
	VIC2INTENCLEAR = 0xffffffff;
	VIC3INTENCLEAR = 0xffffffff;

	// 选择中断类型为IRQ
	VIC0INTSELECT = 0x0;
	VIC1INTSELECT = 0x0;
	VIC2INTSELECT = 0x0;
	VIC3INTSELECT = 0x0;

	// 清VICxADDR
	intc_clearvectaddr();

}

// 保存需要处理的中断的中断处理函数的地址  
void intc_setvectaddr(unsigned long intnum, void (*handler)(void))
{
	//VIC0
	if(intnum<32)			
	{
		*( (volatile unsigned long *)(VIC0VECTADDR + 4*intnum) ) = (unsigned)handler;
	}
	//VIC1
	else if(intnum<64) 		
	{
		*( (volatile unsigned long *)(VIC1VECTADDR + 4*(intnum-32)) ) = (unsigned)handler;
	}
	//VIC2
	else if(intnum<96) 			
	{
		*( (volatile unsigned long *)(VIC2VECTADDR + 4*(intnum-64)) ) = (unsigned)handler;
	}
	//VIC3
	else	
	{
		*( (volatile unsigned long *)(VIC3VECTADDR + 4*(intnum-96)) ) = (unsigned)handler;
	}
	return;
}

// 清除需要处理的中断的中断处理函数的地址  
void intc_clearvectaddr(void)
{
	// VICxADDR:当前正在处理的中断的中断处理函数的地址
	VIC0ADDR = 0;
	VIC1ADDR = 0;
	VIC2ADDR = 0;
	VIC3ADDR = 0;
}

// 使能中断  
void intc_enable(unsigned long intnum)
{
	unsigned long temp;
	if(intnum<32)
	{
		temp = VIC0INTENABLE;
		temp |= (1<<intnum);
		VIC0INTENABLE = temp;
	}
	else if(intnum<64)
	{
		temp = VIC1INTENABLE;
		temp |= (1<<(intnum-32));
		VIC1INTENABLE = temp;
	}
	else if(intnum<96)
	{
		temp = VIC2INTENABLE;
		temp |= (1<<(intnum-64));
		VIC2INTENABLE = temp;
	}
	else if(intnum<NUM_ALL)
	{
		temp = VIC3INTENABLE;
		temp |= (1<<(intnum-96));
		VIC3INTENABLE = temp;
	}
	// NUM_ALL : enable all interrupt
	else 
	{
		VIC0INTENABLE = 0xFFFFFFFF;
		VIC1INTENABLE = 0xFFFFFFFF;
		VIC2INTENABLE = 0xFFFFFFFF;
		VIC3INTENABLE = 0xFFFFFFFF;
	}

}

// 禁止中断  
void intc_disable(unsigned long intnum)
{
	unsigned long temp;

	if(intnum<32)
	{
		temp = VIC0INTENCLEAR;
		temp |= (1<<intnum);
		VIC0INTENCLEAR = temp;
	}
	else if(intnum<64)
	{
		temp = VIC1INTENCLEAR;
		temp |= (1<<(intnum-32));
		VIC1INTENCLEAR = temp;
	}
	else if(intnum<96)
	{
		temp = VIC2INTENCLEAR;
		temp |= (1<<(intnum-64));
		VIC2INTENCLEAR = temp;
	}
	else if(intnum<NUM_ALL)
	{
		temp = VIC3INTENCLEAR;
		temp |= (1<<(intnum-96));
		VIC3INTENCLEAR = temp;
	}
	// NUM_ALL : disable all interrupt
	else 
	{
		VIC0INTENCLEAR = 0xFFFFFFFF;
		VIC1INTENCLEAR = 0xFFFFFFFF;
		VIC2INTENCLEAR = 0xFFFFFFFF;
		VIC3INTENCLEAR = 0xFFFFFFFF;
	}

	return;
}


// 读中断状态  
unsigned long intc_getvicirqstatus(unsigned long ucontroller)
{
	if(ucontroller == 0)
		return	VIC0IRQSTATUS;
	else if(ucontroller == 1)
		return 	VIC1IRQSTATUS;
	else if(ucontroller == 2)
		return 	VIC2IRQSTATUS;
	else if(ucontroller == 3)
		return 	VIC3IRQSTATUS;
	else
	{}
	return 0;
}

// 通用中断处理函数
void irq_handler(void)
{
	unsigned long vicaddr[4] = {VIC0ADDR,VIC1ADDR,VIC2ADDR,VIC3ADDR};
	int i=0;
	void (*isr)(void) = NULL;

	for(; i<4; i++)
	{
		if(intc_getvicirqstatus(i) != 0)
		{
			isr = (void (*)(void)) vicaddr[i];
			break;
		}
	}
	(*isr)();
}


