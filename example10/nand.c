#define	NFCONF  (*(volatile unsigned int *)0xB0E00000) 
#define	NFCONT  (*(volatile unsigned int *)0xB0E00004) 	
#define	NFCMMD  (*(volatile unsigned char *)0xB0E00008) 
#define	NFADDR  (*(volatile unsigned char *)0xB0E0000C)
#define	NFDATA  (*(volatile unsigned char *)0xB0E00010)
#define	NFSTAT  (*(volatile unsigned int *)0xB0E00028)

#define	MP0_3CON  (*(volatile unsigned int *)0xE0200320)
#define   MP0_1CON  (*(volatile unsigned int *)0xE02002E0)
		
#define PAGE_SIZE	2048
#define NAND_SECTOR_SIZE_LP    2048

void wait_idle(void)
{
	int i;
	while(!(NFSTAT&(1<<0)));
	for(i=0; i<10; i++);
}

void nand_select_chip(void)
{
	int i;
	NFCONT &= ~(1<<1);
	for(i=0; i<10; i++);
}

void nand_deselect_chip(void)
{
	NFCONT |= (1<<1);
}

void write_cmd(int cmd)
{
	NFCMMD = cmd;
}

void write_addr(unsigned int addr)
{
	int i;
	NFADDR = (addr>>0) & 0xFF;
	wait_idle();
	NFADDR = (addr>>8) & 0x7;
	wait_idle();
	NFADDR = (addr>>11) & 0xFF;
	wait_idle();
	NFADDR = (addr>>19) & 0xFF;
	wait_idle();
	NFADDR = (addr>>27) & 0x1;
	wait_idle();
}

unsigned char read_data(void)
{
	return NFDATA;
}

static void nand_reset(void)
{
    nand_select_chip();
    write_cmd(0xff);  // 复位命令
    wait_idle();
    nand_deselect_chip();
}

void nand_init(void)
{
	/* 
	 * 设置时间参数(HCLK_PSYS = 667MHz/5 = 133MHz)
	 * TACLS[15:12]: TACLS  = 1 	1/133Mhz  = 7.5ns
	 * TWRPH0[11:8]: TWRPH0 = 1 	7.5ns * 2 = 15ns
	 * TWRPH1 [7:4]: TWRPH1 = 1	7.5ns * 2 = 15ns
	 * AddrCycle[1]: 1 = 指明地址周期为5次，这个是和2440的区别
	 */
	NFCONF |= 1<<12 | 1<<8 | 1<<4;
	NFCONF |= 1<<1;

	/*
	 * 使能NAND控制器
	 * 关闭片选信号
	 */
	NFCONT |= (1<<0)|(1<<1); 

	/*
	 * 设置相应管脚用于Nand Flash控制器
	 */
	MP0_3CON = 0x22222222;

	/* 复位NAND Flash */
	nand_reset();

	return;
}

/* 读ID */
void nand_read_id(char id[])
{
	int i;
	
	nand_select_chip();
	write_cmd(0x90);
	write_addr(0x00);

	for (i = 0; i < 5; i++)
		id[i] = read_data();

	nand_deselect_chip();
}

/* 读一页的函数 */
void nand_read(unsigned char *buf, unsigned long start_addr, int size)
{
	int i, j;

	/* 选中芯片 */
	nand_select_chip();

	for(i=start_addr; i < (start_addr + size);) {
		/* 发出READ0命令 */
		write_cmd(0);

		/* Write Address */
		write_addr(i);
		write_cmd(0x30);		
		wait_idle();

		for(j=0; j < NAND_SECTOR_SIZE_LP; j++, i++) {
			*buf = read_data();
			buf++;
		}
	}

	/* 取消片选信号 */
	nand_deselect_chip();
}

void nand_write(int sdram_addr, int nand_addr, int size)
{

}

