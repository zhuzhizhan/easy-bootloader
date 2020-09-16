#include "command.h"
#include "clock.h"
#include "led.h"
#include "uart.h"
#include "lib.h"
#include "nand.h"
#include "lcd.h"
#include "timer.h"
#include "setup.h"

#define UTRSTAT0  	(*(volatile unsigned int *)0xE2900010)

#define	CFG_PROMPT		"WY_BOOT # "	/* Monitor Command Prompt	*/
#define	CFG_CBSIZE		256		/* Console I/O Buffer Size	*/
#define	BOOTDELAY		5	        /* 倒计时 */


void (*fp)(int, int, int);

char *argv[10];

int readline (const char *const prompt)
{
	char console_buffer[CFG_CBSIZE];		/* console I/O buffer	*/
	char *buf = console_buffer;
	int argc = 0;
	int state = 0;

	//puts(prompt);
	wy_printf("%s",prompt);
	gets(console_buffer);
	
	while (*buf)
	{
		if (*buf != ' ' && state == 0)
		{
			argv[argc++] = buf;
			state = 1;
		}
		
		if (*buf == ' ' && state == 1)
		{
			*buf = '\0';
			state = 0;
		}
		
		buf++;	
	}
	
	return argc;
}

void message(void)
{
	wy_printf("\nThis bootloader support some command to test peripheral:\n");
	wy_printf("Such as: LCD, IIS, BUZZER \n");
	wy_printf("Try 'help' to learn them \n\n");	
}

const char cmd[] = "root=/dev/nfs nfsroot=192.168.1.104:/work/nfs_root/wy_fs ip=192.168.1.17 console=ttySAC0";
void init_tag(int addr)
{
	struct tag * p;
	int i;
	
	p = (struct tag*) addr;
	p->hdr.tag  =  ATAG_CORE;
	p->hdr.size = tag_size(tag_core);
	p->u.core.flags = 0;
	p->u.core.pagesize = 0;
	p->u.core.rootdev = 0;

	p = tag_next(p);
	p->hdr.tag = ATAG_CMDLINE;
	p->hdr.size =  (sizeof (cmd) + sizeof(struct tag_header) + 3) >>2;	
	for(i=0; i< sizeof (cmd); i++)	
		p->u.cmdline.cmdline[i] = cmd[i];

	p = tag_next(p);
	p->hdr.tag = ATAG_MEM;
	p->hdr.size = tag_size(tag_mem32);
	p->u.mem.size = 512*1024*1024;
	p->u.mem.start = 0x20000000;

	p = tag_next(p);
	p->hdr.tag = ATAG_NONE;
	p->hdr.size = 0;
}

void init_boot_parameter(void)
{
	int addr = 0x20008000;
	int taglist_mem_address = 0x20000100;
	
	fp = (void (*)(int, int, int))addr;
	
	init_tag(taglist_mem_address);	
}

int tstc (void)
{
	return UTRSTAT0 & 0x1;
}

void autoboot(void)
{
	int i;
	char bootdelay = BOOTDELAY;

	while(1)
	{
		wy_printf("Hit any key to stop autoboot: %d \n", bootdelay);	
		
		if(bootdelay == 0)
		{
			wy_printf("loading linux from 0x400000 to 0x20008000...\n");

			nand_read(0x20008000, 0x400000, 0x800000);

			wy_printf("boot linux ...\n");
			fp(0, 2456, 0x20000100);
			
			return;
		}

		for(i=0;i<=8;i++)
		{
			delay();
			if( tstc() )
				return;
		}

		bootdelay--;
	}

	return;
}

int main(void)
{
	char buf[6];
	int argc = 0;
	int i = 0;

	led_init(); /* 设置对应管脚为输出 */
	uart_init(); /* 初始化UART0 */
	lcd_init(); /* 初始化LCD */
	nand_read_id(buf);
	timer0_init(); /* 初始化定时器0，用于PWM输出 */
	init_boot_parameter(); /* 初始化启动内核需要的相关类容 */
		
	wy_printf("\nloading logo from NAND 0xc00000 to DDR 0x3fc00000...");
	nand_read((unsigned char *)0x3fc00000, 0xC00000, 0x300000);
	
	wy_printf("\n**********************************************************\n");
	wy_printf("                     wy_bootloader\n");
	wy_printf("                     vars: %d \n",2012);
	wy_printf("                     nand id:");
	putchar_hex(buf[0]);
	putchar_hex(buf[1]);
	putchar_hex(buf[2]);
	putchar_hex(buf[3]);
	putchar_hex(buf[4]);
	wy_printf("\n**********************************************************\n");

	lcd_draw_bmp(0x3fc00000); /*  显示LOGO */

	autoboot();  /* 延时自启动 */

	while (1)
	{
		argc = readline (CFG_PROMPT);
		if(argc == 0 && i ==0)
		{
			message();
			i=1;
		}
		run_command(argc, argv);
	}

	return 0;
}

