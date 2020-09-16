#define GPF0CON		(*(volatile unsigned int *)0xE0200120)
#define GPF1CON		(*(volatile unsigned int *)0xE0200140)
#define GPF2CON		(*(volatile unsigned int *)0xE0200160)
#define GPF3CON		(*(volatile unsigned int *)0xE0200180)

#define GPD0CON		(*(volatile unsigned int *)0xE02000A0)
#define GPD0DAT		(*(volatile unsigned int *)0xE02000A4)

#define CLK_SRC1	(*(volatile unsigned int *)0xe0100204)
#define CLK_DIV1	(*(volatile unsigned int *)0xe0100304)
#define DISPLAY_CONTROL	(*(volatile unsigned int *)0xe0107008)

#define VIDCON0		(*(volatile unsigned int *)0xF8000000)
#define VIDCON1		(*(volatile unsigned int *)0xF8000004)
#define VIDTCON2 	(*(volatile unsigned int *)0xF8000018)
#define VIDTCON3 	(*(volatile unsigned int *)0xF800001c)
#define WINCON0 	(*(volatile unsigned int *)0xF8000020)
#define WINCON2 	(*(volatile unsigned int *)0xF8000028)
#define SHADOWCON 	(*(volatile unsigned int *)0xF8000034)
#define VIDOSD0A 	(*(volatile unsigned int *)0xF8000040)
#define VIDOSD0B 	(*(volatile unsigned int *)0xF8000044)
#define VIDOSD0C 	(*(volatile unsigned int *)0xF8000048)

#define VIDW00ADD0B0 	(*(volatile unsigned int *)0xF80000A0)
#define VIDW00ADD1B0 	(*(volatile unsigned int *)0xF80000D0)
#define VIDW00ADD2   	(*(volatile unsigned int *)0xF8000100)

#define VIDTCON0 	(*(volatile unsigned int *)0xF8000010)
#define VIDTCON1 	(*(volatile unsigned int *)0xF8000014)

#define VSPW       9  
#define VBPD       13 
#define LINEVAL    479
#define VFPD       21 

#define HSPW       19 
#define HBPD       25 
#define HOZVAL     799
#define HFPD       209

#define LeftTopX     0
#define LeftTopY     0
#define RightBotX   799
#define RightBotY   479

#define FRAME_BUFFER   (0x3f000000)

void lcd_init(void)
{
	/* 1. 设置相关GPIO引脚用于LCD */
	GPF0CON = 0x22222222;		// GPF0[7:0]
	GPF1CON = 0x22222222;		// GPF1[7:0]
	GPF2CON = 0x22222222;		// GPF2[7:0]
	GPF3CON = 0x22222222;		// GPF3[7:0]

	/* 使能LCD本身 */
	GPD0CON |= 1<<4;
	GPD0DAT |= 1<<1;

	/* 该寄存器是时钟相关
	 * Display path selection 
	 * 10: RGB=FIMD I80=FIMD ITU=FIMD
	 */
	DISPLAY_CONTROL = 2<<0;

	/* 2. 初始化210的display controller 
	 * 2.1 hsync,vsync,vclk,vden的极性和时间参数
	 * 2.2 行数、列数(分辨率),象素颜色的格式
	 * 2.3 分配显存(frame buffer),写入display controller
	 */
	/* 
	 * CLKVAL_F[13:6]:该值需要根据LCD手册做相应的修改
	 * 			     HCLKD=166.75MHz，DCLK(min) = 20ns(50MHz)
	 *                VCLK = 166.75 / (5+1) = 28MHz
	 * CLKDIR  [4]:1 = Divided by CLKVAL_F
	 * ENVID   [1]:1 = Enable the video output and the Display control signal. 
	 * ENVID_F [0]:1 = Enable the video output and the Display control signal.  
	 */
	VIDCON0 &= ~((3<<26) | (1<<18) | (0xff<<6)  | (1<<2));     /* RGB I/F, RGB Parallel format,  */
	VIDCON0 |= ((5<<6) | (1<<4) );

	/* 设置极性(该值需要根据LCD手册做相应的修改)
	 * IVDEN [4]:0 = Normal
	 * IVSYNC[5]:1 = Inverted
	 * IHSYNC[6]:1 = Inverted
	 * IVCLK [7]:0 = Video data is fetched at VCLK falling edge
	 */
	VIDCON1 &= ~(1<<7);   /* 在vclk的下降沿获取数据 */
	VIDCON1 |= ((1<<6) | (1<<5));  /* HSYNC极性反转, VSYNC极性反转 */

	/* 设置时序(需要修改) */
	VIDTCON0 = (VBPD << 16) | (VFPD << 8) | (VSPW << 0);
	VIDTCON1 = (HBPD << 16) | (HFPD << 8) | (HSPW << 0);

	/* 设置屏幕的大小
	 * LINEVAL[21:11]:多少行   = 480
	 * HOZVAL [10:0] :水平大小 = 800
	 */
	VIDTCON2 = (LINEVAL << 11) | (HOZVAL << 0);

	/* WSWP_F   [15] :1    = Swap Enable(为什么要使能)，很关键的一位，能够解决掉重影问题
	 * BPPMODE_F[5:2]:1011 = unpacked 24 BPP (non-palletized R:8-G:8-B:8 )
	 * ENWIN_F  [0]:  1    = Enable the video output and the VIDEO control signal.
	 */
	WINCON0 &= ~(0xf << 2);
	WINCON0 |= (0xB<<2)|(1<<15);

	/* 窗口0，左上角的位置(0,0) */
	/* 窗口0，右下角的位置(800,480) */
	VIDOSD0A = (LeftTopX<<11) | (LeftTopY << 0);
	VIDOSD0B = (RightBotX<<11) | (RightBotY << 0);
	/* 大小 */
	VIDOSD0C = (LINEVAL + 1) * (HOZVAL + 1);

	VIDW00ADD0B0 = FRAME_BUFFER;
	/* VBASEL = VBASEU + (LINEWIDTH+OFFSIZE) x (LINEVAL+1) 
	 *        = 0 + (800*4 + 0) * 479
	 *        = 
	 */
	VIDW00ADD1B0 =  (((HOZVAL + 1)*4 + 0) * (LINEVAL + 1)) & (0xffffff);
	//VIDW00ADD1B0 = FRAME_BUFFER + HOZVAL * LINEVAL * 4; /* 新加的，是该这个吗? */

	SHADOWCON = 0x1; /* 使能通道0 */

	/* LCD控制器开启 */
	VIDCON0  |= 0x3; /* 开启总控制器 */
	WINCON0 |= 1;     /* 开启窗口0 */
}

void lcd_draw_pixel(int row, int col, int color)
{
	int * pixel = (int *)FRAME_BUFFER;

	*(pixel + row * (HOZVAL+1) + col) = color;	

	return;
} 

void lcd_clear_screen(int color)
{
	int i, j;
		
	for (i = 0; i < (LINEVAL+1); i++)
		for (j = 0; j < (HOZVAL+1); j++)
		{
			lcd_draw_pixel(i, j, color);
		}

	return;
}

void lcd_draw_bmp(int bmp_file_addr)
{
	int i, j;
	char * p = (char *)bmp_file_addr;
	int blue, green, red;
	int color;

	// read bmp file
	// bmp file header is 54 bytes
	p += 54;
	
	for (i = 0; i < 480; i++)
		for (j = 0; j < 800; j++)
		{
			blue = *p++;
			green = *p++;
			red = *p++;
		
			color = red << 16 | green << 8 | blue << 0;
			
			lcd_draw_pixel(480-i, j, color);
		}

	return;
}
