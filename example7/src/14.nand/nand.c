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

#define MAX_NAND_BLOCK  			  8192 			/*����nand��������8192�� 	*/
#define NAND_PAGE_SIZE  			  2048 			/*����һҳ������:2048 byte 	*/
#define NAND_BLOCK_SIZE 			  64  			/*����block��С��64ҳ		*/

#define TACLS    					  1				/* ʱ����ص����� 			*/
#define TWRPH0   					  4
#define TWRPH1   					  1

#define NAND_CMD_READ_1st             0x00			/* ����						*/
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

// ��λ  
static void nand_reset(void)
{
	nand_select_chip();
	nand_send_cmd(NAND_CMD_RES);
	nand_wait_idle();
	nand_deselect_chip();
}

// �ȴ�����  
static void nand_wait_idle(void)
{
	unsigned long i;
	while( !(NFSTAT & (BUSY<<4)) )
		for(i=0; i<10; i++);
}

// ��Ƭѡ  
static void nand_select_chip(void)
{
	unsigned long i;
	NFCONT &= ~(1<<1);
	for(i=0; i<10; i++);
}

// ȡ��Ƭѡ  
static void nand_deselect_chip(void)
{
	NFCONT |= (1<<1);
}

// ������  
static void nand_send_cmd(unsigned long cmd)
{
	NFCMMD = cmd;
}

// ����ַ  
static void nand_send_addr(unsigned long addr)
{
	unsigned long i;
	unsigned long col, row;

	// �е�ַ����ҳ�ڵ�ַ  
	col = addr % NAND_PAGE_SIZE;		
	// �е�ַ����ҳ��ַ  			
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

// ��һ���ֽڵ�����  
static unsigned char nand_read(void)
{
	return NFDATA;
}

// дһ���ֽڵ�����  
static void nand_write(unsigned char data)
{
	NFDATA = data;
}

// nandflash ��ʼ��  
void nand_init(void)
{

	// 1. ����nandflash  
	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<3)|(0<<2)|(1<<1)|(0<<0);
	NFCONT = (0<<18)|(0<<17)|(0<<16)|(0<<10)|(0<<9)|(0<<8)|(0<<7)|(0<<6)|(0x3<<1)|(1<<0);

	// 2. ��������  
	MP0_1CON = 0x22333322;
	MP0_2CON = 0x00002222;
	MP0_3CON = 0x22222222;

	// 3. ��λ  
	nand_reset();
}

// ��оƬID  
void nand_read_id(void)
{
	nand_id_info nand_id;
	// 1. ��Ƭѡ  
	nand_select_chip();

	// 2. ��ID  
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

// ������  
unsigned char nand_erase(unsigned long block_num)
{
	unsigned long i = 0;

	// ���row��ַ����ҳ��ַ  
	unsigned long row = block_num * NAND_BLOCK_SIZE;

	// 1. ����Ƭѡ�ź�  
	nand_select_chip();
	// 2. ��������һ�����ڷ�����0x60���ڶ������ڷ����ַ�����������ڷ�����0xd0  
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
	// 3. �ȴ�����  
	nand_wait_idle();

	// 4. ��״̬  
	unsigned char status = read_nand_status();
	if (status & 1 )
	{
		// ȡ��Ƭѡ�ź�  
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

// ��nand�ж����ݵ�sdram  
int copy_nand_to_sdram(unsigned char *sdram_addr, unsigned long nand_addr, unsigned long length)
{
	unsigned long i = 0;

	// 1. ����Ƭѡ�ź�  
	nand_select_chip();

	// 2. ��nand�����ݵ�sdram����һ���ڷ�����0x00���ڶ����ڷ���ַnand_addr,���������ڷ�����0x30���ɶ�һҳ(2k)������  
	while(length)
	{
		nand_send_cmd(NAND_CMD_READ_1st);
		nand_send_addr(nand_addr);
		NFSTAT = (NFSTAT)|(1<<4);
		nand_send_cmd(NAND_CMD_READ_2st);
		nand_wait_idle();

		// �е�ַ����ҳ�ڵ�ַ  
		unsigned long col = nand_addr % NAND_PAGE_SIZE;
		i = col;
		// ��һҳ���ݣ�ÿ�ο�1byte������2048��(2k)��ֱ������Ϊlength�����ݿ������ 
		for(; i<NAND_PAGE_SIZE && length!=0; i++,length--)
		{
			*sdram_addr = nand_read();
			sdram_addr++;
			nand_addr++;
		}
	}

	// 3. ��״̬  
	unsigned char status = read_nand_status();
	if (status & 1 )
	{
		// ȡ��Ƭѡ�ź�  
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

// ��sdram��д���ݵ�nand  
int copy_sdram_to_nand(unsigned char *sdram_addr, unsigned long nand_addr, unsigned long length)
{
	unsigned long i = 0;

	// 1. ����Ƭѡ�ź�  
	nand_select_chip();

	// 2. ��sdram�����ݵ�nand����һ���ڷ�����0x80���ڶ����ڷ���ַnand_addr,����������дһҳ(2k)���ݣ��������ڷ�0x10  
	while(length)
	{
		nand_send_cmd(NAND_CMD_WRITE_PAGE_1st);
		nand_send_addr(nand_addr);
		// �е�ַ����ҳ�ڵ�ַ  
		unsigned long col = nand_addr % NAND_PAGE_SIZE;
		i = col;
		// дһҳ���ݣ�ÿ�ο�1byte������2048��(2k)��ֱ������Ϊlength�����ݿ������ 
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
	// 3. ��״̬  
	unsigned char status = read_nand_status();
	if (status & 1 )
	{
		// ȡ��Ƭѡ�ź�  
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

// ��������� paddrҳ��ַ,offsetҳ��ƫ�Ƶ�ַ  
unsigned char nand_random_read(unsigned long paddr,unsigned short offset)
{
	unsigned char readdata;

	// 1. ����Ƭѡ�ź�  
	nand_select_chip();

	// 2. �����ҳ��ĳ����ַ��ֵ  
	nand_send_cmd(NAND_CMD_READ_1st);
	//д��ҳ��ַ
	NFADDR = 0;										
	NFADDR = 0;
	NFADDR = paddr&0xff;
	NFADDR = (paddr>>8)&0xff;
	NFADDR = (paddr>>16)&0xff;
	NFSTAT = (NFSTAT)|(1<<4);
	nand_send_cmd(NAND_CMD_READ_2st);
	nand_wait_idle();

	nand_send_cmd(NAND_CMD_RANDOM_READ_1st);
	//д��ҳ��ƫ�Ƶ�ַ
	NFADDR = offset&0xff; 							
	NFADDR = (offset>>8)&0xff;
	NFSTAT = (NFSTAT)|(1<<4);
	nand_send_cmd(NAND_CMD_RANDOM_READ_2st);

	readdata = nand_read();

	// 3. ��״̬  
	unsigned char status = read_nand_status();
	if (status & 1 )
	{
		// ȡ��Ƭѡ�ź�  
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

// ���д���� paddrҳ��ַ,offsetҳ��ƫ�Ƶ�ַ  
unsigned char nand_random_write(unsigned long paddr,unsigned short offset,unsigned char wrdata)
{
	// 1. ����Ƭѡ�ź�  
	nand_select_chip();

	// 2. ���дҳ��ĳ����ַ��ֵ  
	nand_send_cmd(NAND_CMD_WRITE_PAGE_1st);
	NFADDR = 0;
	NFADDR = 0;
	NFADDR = paddr&0xff;
	NFADDR = (paddr>>8)&0xff;
	//NFADDR = (paddr>>16)&0xff;
	nand_send_cmd(NAND_CMD_RANDOM_WRITE);
	//д��ҳ��ƫ�Ƶ�ַ
	NFADDR = offset&0xff; 					
	NFADDR = (offset>>8)&0xff;
	nand_write(wrdata);
	NFSTAT = (NFSTAT)|(1<<4);
	nand_send_cmd(NAND_CMD_WRITE_PAGE_2st);
	nand_wait_idle();

	// 3. ��״̬  
	unsigned char status = read_nand_status();
	if (status & 1 )
	{
		// ȡ��Ƭѡ�ź�  
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

	// 1. ����Ƭѡ�ź�  
	nand_select_chip();

	// 2. ��״̬  
	nand_send_cmd(NAND_CMD_READ_STATUS);
	for(i=0; i<10; i++);
	ch = nand_read();

	// 3. ȡ��Ƭѡ  
	nand_deselect_chip();
	return ch;
}
