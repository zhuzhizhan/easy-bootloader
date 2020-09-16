#define APLL_CON  	(*(volatile unsigned int *)0xe0100100) 
#define CLK_SRC0  	(*(volatile unsigned int *)0xe0100200) 
#define CLK_DIV0  	(*(volatile unsigned int *)0xe0100300) 
#define MPLL_CON  	(*(volatile unsigned int *)0xe0100108)  

void clock_init(void)
{
	/* 设置时钟为:
	 * ARMCLK=1000MHz, HCLKM=200MHz, HCLKD=166.75MHz
	 * HCLKP =133.44MHz, PCLKM=100MHz, PCLKD=83.375MHz, 
	 * PCLKP =66.7MHz
	 */

	/* SDIV[2:0]  : S = 1
	 * PDIV[13:8] : P = 0x3
	 * MDIV[25:16]: M = 0x7d
	 * LOCKED [29]: 1 = 使能锁
	 * ENABLE [31]: 1 = 使能APLL控制器
	 * 得出FoutAPLL = 500MHz
	 */
	APLL_CON = (1<<31)|(1<<29)|(0x7d<<16)|(0x3<<8)|(1<<0);
	/* 时钟源的设置
	 * APLL_SEL[0] :1 = FOUTAPLL
	 * MPLL_SEL[4] :1 = FOUTMPLL
	 * EPLL_SEL[8] :1 = FOUTEPLL
	 * VPLL_SEL[12]:1 = FOUTVPLL
	 * MUX_MSYS_SEL[16]:0 = SCLKAPLL
	 * MUX_DSYS_SEL[20]:0 = SCLKMPLL
	 * MUX_PSYS_SEL[24]:0 = SCLKMPLL
	 * ONENAND_SEL [28]:1 = HCLK_DSYS
	 */
	CLK_SRC0 = (1<<28)|(1<<12)|(1<<8)|(1<<4)|(1<<0);
	/* 设置分频系数
	 * APLL_RATIO[2:0]: APLL_RATIO = 0x0
	 * A2M_RATIO [6:4]: A2M_RATIO  = 0x4
	 * HCLK_MSYS_RATIO[10:8]: HCLK_MSYS_RATIO = 0x4
	 * PCLK_MSYS_RATIO[14:12]:PCLK_MSYS_RATIO = 0x1
	 * HCLK_DSYS_RATIO[19:16]:HCLK_DSYS_RATIO = 0x3
	 * PCLK_DSYS_RATIO[22:20]:PCLK_DSYS_RATIO = 0x1
	 * HCLK_PSYS_RATIO[27:24]:HCLK_PSYS_RATIO = 0x4
	 * PCLK_PSYS_RATIO[30:28]:PCLK_PSYS_RATIO = 0x1
	 */
	 CLK_DIV0 = (0x1<<28)|(0x4<<24)|(0x1<<20)|(0x3<<16)|(0x1<<12)|(0x4<<8)|(0x4<<4);
	/* SDIV[2:0]  : S = 1
	 * PDIV[13:8] : P = 0xc
	 * MDIV[25:16]: M = 0x29b
	 * VSEL   [27]: 0
	 * LOCKED [29]: 1 = 使能锁
	 * ENABLE [31]: 1 = 使能MPLL控制器
	 * 得出FoutAPLL = 667MHz
	 */
	APLL_CON = (1<<31)|(1<<29)|(0x29d<<16)|(0xc<<8)|(1<<0);
}
