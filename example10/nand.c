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
    write_cmd(0xff);  // ��λ����
    wait_idle();
    nand_deselect_chip();
}

void nand_init(void)
{
	/* 
	 * ����ʱ�����(HCLK_PSYS = 667MHz/5 = 133MHz)
	 * TACLS[15:12]: TACLS  = 1 	1/133Mhz  = 7.5ns
	 * TWRPH0[11:8]: TWRPH0 = 1 	7.5ns * 2 = 15ns
	 * TWRPH1 [7:4]: TWRPH1 = 1	7.5ns * 2 = 15ns
	 * AddrCycle[1]: 1 = ָ����ַ����Ϊ5�Σ�����Ǻ�2440������
	 */
	NFCONF |= 1<<12 | 1<<8 | 1<<4;
	NFCONF |= 1<<1;

	/*
	 * ʹ��NAND������
	 * �ر�Ƭѡ�ź�
	 */
	NFCONT |= (1<<0)|(1<<1); 

	/*
	 * ������Ӧ�ܽ�����Nand Flash������
	 */
	MP0_3CON = 0x22222222;

	/* ��λNAND Flash */
	nand_reset();

	return;
}

/* ��ID */
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

/* ��һҳ�ĺ��� */
void nand_read(unsigned char *buf, unsigned long start_addr, int size)
{
	int i, j;

	/* ѡ��оƬ */
	nand_select_chip();

	for(i=start_addr; i < (start_addr + size);) {
		/* ����READ0���� */
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

	/* ȡ��Ƭѡ�ź� */
	nand_deselect_chip();
}

void nand_write(int sdram_addr, int nand_addr, int size)
{

}

