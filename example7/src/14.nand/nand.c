#include "nand.h"
#include "stdio.h"

#define NFCONF 			( *((volatile unsigned long *)0xB0E00000) )
#define NFCONT 			( *((volatile unsigned long *)0xB0E00004) )
#define NFCMMD 			( *((volatile unsigned long *)0xB0E00008) )
#define NFADDR 			( *((volatile unsigned long *)0xB0E0000C) )
#define NFDATA 			( *((volatile unsigned long *)0xB0E00010) )
#define NFSTAT 			( *((volatile unsigned long *)0xB0E00028) )

#define MP0_1CON 		( *((volatile unsigned long *)0xE02002E0) )
#define MP0_2CON 		( *((volatile unsigned long *)0xE0200300) )
#define MP0_3CON 		( *((volatile unsigned long *)0xE0200320) )

#define MAX_NAND_BLOCK  			  8192 			/*定义nand最大块数：8192块 	*/
#define NAND_PAGE_SIZE  			  2048 			/*定义一页的容量:2048 byte 	*/
#define NAND_BLOCK_SIZE 			  64  			/*定义block大小：64页		*/

#define TACLS    					  1				/* 时序相关的设置 			*/
#define TWRPH0   					  4
#define TWRPH1   					  1

#define NAND_CMD_READ_1st             0x00			/* 命令						*/
#define NAND_CMD_READ_2st             0x30
#define NAND_CMD_RANDOM_WRITE         0x85
#define NAND_CMD_RANDOM_READ_1st      0x05
#define NAND_CMD_RANDOM_READ_2st      0xe0
#define NAND_CMD_READ_CB_1st          0x00
#define NAND_CMD_READ_CB_2st          0x35
#define NAND_CMD_READ_ID              0x90
#define NAND_CMD_RES                  0xff
#define NAND_CMD_WRITE_PAGE_1st       0x80
#define NAND_CMD_WRITE_PAGE_2st       0x10
#define NAND_CMD_BLOCK_ERASE_1st      0x60
#define NAND_CMD_BLOCK_ERASE_2st      0xd0
#define NAND_CMD_READ_STATUS          0x70

#define BUSY 				  		  1
#define ECC_EN						  (1<<4)
#define CHIP_SELECTED   			  (~(1<<1))
#define CHIP_DESELECTED 			  (1<<1)
#define CONTROL_EN					  (1<<0)


static void nand_reset(void);
static void nand_wait_idle(void);
static void nand_select_chip(void);
static void nand_deselect_chip(void);
static void nand_send_cmd(unsigned long cmd);
static void nand_send_addr(unsigned long addr);
static unsigned char nand_read(void);
static void nand_write(unsigned char data);

typedef struct nand_id_info
{
	//marker code
	unsigned char IDm; 
	//device code
	unsigned char IDd; 
	unsigned char ID3rd;
	unsigned char ID4th;
	unsigned char ID5th;
}nand_id_info;

// 复位  
static void nand_reset(void)
{
	nand_select_chip();
	nand_send_cmd(NAND_CMD_RES);
	nand_wait_idle();
	nand_deselect_chip();
}

// 等待就绪  
static void nand_wait_idle(void)
{
	unsigned long i;
	while( !(NFSTAT & (BUSY<<4)) )
		for(i=0; i<10; i++);
}

// 发片选  
static void nand_select_chip(void)
{
	unsigned long i;
	NFCONT &= ~(1<<1);
	for(i=0; i<10; i++);
}

// 取消片选  
static void nand_deselect_chip(void)
{
	NFCONT |= (1<<1);
}

// 发命令  
static void nand_send_cmd(unsigned long cmd)
{
	NFCMMD = cmd;
}

// 发地址  
static void nand_send_addr(unsigned long addr)
{
	unsigned long i;
	unsigned long col, row;

	// 列地址，即页内地址  
	col = addr % NAND_PAGE_SIZE;		
	// 行地址，即页地址  			
	row = addr / NAND_PAGE_SIZE;

	// Column Address A0~A7  
	NFADDR = col & 0xff;			
	for(i=0; i<10; i++);		

	// Column Address A8~A11  
	NFADDR = (col >> 8) & 0x0f; 		
	for(i=0; i<10; i++);

	// Row Address A12~A19	
	NFADDR = row & 0xff;			
	for(i=0; i<10; i++);

	// Row Address A20~A27	
	NFADDR = (row >> 8) & 0xff;
	for(i=0; i<10; i++);

	// Row Address A28~A30	
	NFADDR = (row >> 16) & 0xff;
	for(i=0; i<10; i++);
}

// 读一个字节的数据  
static unsigned char nand_read(void)
{
	return NFDATA;
}

// 写一个字节的数据  
static void nand_write(unsigned char data)
{
	NFDATA = data;
}

// nandflash 初始化  
void nand_init(void)
{

	// 1. 配置nandflash  
	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<3)|(0<<2)|(1<<1)|(0<<0);
	NFCONT = (0<<18)|(0<<17)|(0<<16)|(0<<10)|(0<<9)|(0<<8)|(0<<7)|(0<<6)|(0x3<<1)|(1<<0);

	// 2. 配置引脚  
	MP0_1CON = 0x22333322;
	MP0_2CON = 0x00002222;
	MP0_3CON = 0x22222222;

	// 3. 复位  
	nand_reset();
}

// 读芯片ID  
void nand_read_id(void)
{
	nand_id_info nand_id;
	// 1. 发片选  
	nand_select_chip();

	// 2. 读ID  
	nand_send_cmd(NAND_CMD_READ_ID);
	nand_send_addr(0x00);
	nand_wait_idle();
	nand_id.IDm = 	nand_read();
	nand_id.IDd = 	nand_read();
	nand_id.ID3rd = nand_read();
	nand_id.ID4th = nand_read();
	nand_id.ID5th = nand_read();

	printf("nandflash: makercode = %x,devicecode = %x\r\n",nand_id.IDm,nand_id.IDd);
	nand_deselect_chip();
}

// 擦除块  
unsigned char nand_erase(unsigned long block_num)
{
	unsigned long i = 0;

	// 获得row地址，即页地址  
	unsigned long row = block_num * NAND_BLOCK_SIZE;

	// 1. 发出片选信号  
	nand_select_chip();
	// 2. 擦除：第一个周期发命令0x60，第二个周期发块地址，第三个周期发命令0xd0  
	nand_send_cmd(NAND_CMD_BLOCK_ERASE_1st);
	for(i=0; i<10; i++);
	// Row Address A12~A19	
	NFADDR = row & 0xff;							
	for(i=0; i<10; i++);
	// Row Address A20~A27  
	NFADDR = (row >> 8) & 0xff;
	for(i=0; i<10; i++);
	// Row Address A28~A30  
	NFADDR = (row >> 16) & 0xff;			
	NFSTAT = (NFSTAT)|(1<<4);
	nand_send_cmd(NAND_CMD_BLOCK_ERASE_2st);
	for(i=0; i<10; i++);
	// 3. 等待就绪  
	nand_wait_idle();

	// 4. 读状态  
	unsigned char status = read_nand_status();
	if (status & 1 )
	{
		// 取消片选信号  
		nand_deselect_chip();						
		printf("masking bad block %d\r\n", block_num);
		return -1;
	}
	else
	{
		nand_deselect_chip();
		return 0;
	}
}

// 从nand中读数据到sdram  
int copy_nand_to_sdram(unsigned char *sdram_addr, unsigned long nand_addr, unsigned long length)
{
	unsigned long i = 0;

	// 1. 发出片选信号  
	nand_select_chip();

	// 2. 从nand读数据到sdram，第一周期发命令0x00，第二周期发地址nand_addr,第三个周期发命令0x30，可读一页(2k)的数据  
	while(length)
	{
		nand_send_cmd(NAND_CMD_READ_1st);
		nand_send_addr(nand_addr);
		NFSTAT = (NFSTAT)|(1<<4);
		nand_send_cmd(NAND_CMD_READ_2st);
		nand_wait_idle();

		// 列地址，即页内地址  
		unsigned long col = nand_addr % NAND_PAGE_SIZE;
		i = col;
		// 读一页数据，每次拷1byte，共拷2048次(2k)，直到长度为length的数据拷贝完毕 
		for(; i<NAND_PAGE_SIZE && length!=0; i++,length--)
		{
			*sdram_addr = nand_read();
			sdram_addr++;
			nand_addr++;
		}
	}

	// 3. 读状态  
	unsigned char status = read_nand_status();
	if (status & 1 )
	{
		// 取消片选信号  
		nand_deselect_chip();
		printf("copy nand to sdram fail\r\n");
		return -1;
	}
	else
	{
		nand_deselect_chip();
		return 0;
	}
}

// 从sdram中写数据到nand  
int copy_sdram_to_nand(unsigned char *sdram_addr, unsigned long nand_addr, unsigned long length)
{
	unsigned long i = 0;

	// 1. 发出片选信号  
	nand_select_chip();

	// 2. 从sdram读数据到nand，第一周期发命令0x80，第二周期发地址nand_addr,第三个周期写一页(2k)数据，第四周期发0x10  
	while(length)
	{
		nand_send_cmd(NAND_CMD_WRITE_PAGE_1st);
		nand_send_addr(nand_addr);
		// 列地址，即页内地址  
		unsigned long col = nand_addr % NAND_PAGE_SIZE;
		i = col;
		// 写一页数据，每次拷1byte，共拷2048次(2k)，直到长度为length的数据拷贝完毕 
		for(; i<NAND_PAGE_SIZE && length!=0; i++,length--)
		{
			nand_write(*sdram_addr);
			sdram_addr++;
			nand_addr++;
		}
		NFSTAT = (NFSTAT)|(1<<4);
		nand_send_cmd(NAND_CMD_WRITE_PAGE_2st);
		nand_wait_idle();
	}
	// 3. 读状态  
	unsigned char status = read_nand_status();
	if (status & 1 )
	{
		// 取消片选信号  
		nand_deselect_chip();
		printf("copy sdram to nand fail\r\n");
		return -1;
	}
	else
	{
		nand_deselect_chip();
		return 0;
	}
}

// 随机读数据 paddr页地址,offset页内偏移地址  
unsigned char nand_random_read(unsigned long paddr,unsigned short offset)
{
	unsigned char readdata;

	// 1. 发出片选信号  
	nand_select_chip();

	// 2. 随机读页内某个地址的值  
	nand_send_cmd(NAND_CMD_READ_1st);
	//写入页地址
	NFADDR = 0;										
	NFADDR = 0;
	NFADDR = paddr&0xff;
	NFADDR = (paddr>>8)&0xff;
	NFADDR = (paddr>>16)&0xff;
	NFSTAT = (NFSTAT)|(1<<4);
	nand_send_cmd(NAND_CMD_READ_2st);
	nand_wait_idle();

	nand_send_cmd(NAND_CMD_RANDOM_READ_1st);
	//写入页内偏移地址
	NFADDR = offset&0xff; 							
	NFADDR = (offset>>8)&0xff;
	NFSTAT = (NFSTAT)|(1<<4);
	nand_send_cmd(NAND_CMD_RANDOM_READ_2st);

	readdata = nand_read();

	// 3. 读状态  
	unsigned char status = read_nand_status();
	if (status & 1 )
	{
		// 取消片选信号  
		nand_deselect_chip();
		printf("nand random read fail\r\n");
		return -1;
	}
	else
	{
		nand_deselect_chip();
		return readdata;
	}
}

// 随机写数据 paddr页地址,offset页内偏移地址  
unsigned char nand_random_write(unsigned long paddr,unsigned short offset,unsigned char wrdata)
{
	// 1. 发出片选信号  
	nand_select_chip();

	// 2. 随机写页内某个地址的值  
	nand_send_cmd(NAND_CMD_WRITE_PAGE_1st);
	NFADDR = 0;
	NFADDR = 0;
	NFADDR = paddr&0xff;
	NFADDR = (paddr>>8)&0xff;
	//NFADDR = (paddr>>16)&0xff;
	nand_send_cmd(NAND_CMD_RANDOM_WRITE);
	//写入页内偏移地址
	NFADDR = offset&0xff; 					
	NFADDR = (offset>>8)&0xff;
	nand_write(wrdata);
	NFSTAT = (NFSTAT)|(1<<4);
	nand_send_cmd(NAND_CMD_WRITE_PAGE_2st);
	nand_wait_idle();

	// 3. 读状态  
	unsigned char status = read_nand_status();
	if (status & 1 )
	{
		// 取消片选信号  
		nand_deselect_chip();
		printf("nand random write fail\r\n");
		return -1;
	}
	else
	{
		nand_deselect_chip();
		return 0;
	}
}

unsigned char read_nand_status(void)
{
	unsigned char ch;
	int i;

	// 1. 发出片选信号  
	nand_select_chip();

	// 2. 读状态  
	nand_send_cmd(NAND_CMD_READ_STATUS);
	for(i=0; i<10; i++);
	ch = nand_read();

	// 3. 取消片选  
	nand_deselect_chip();
	return ch;
}
