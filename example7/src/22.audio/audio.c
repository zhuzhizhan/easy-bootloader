
// GPIO
#define GPICON  	(*(volatile unsigned int *)0xE0200220)	//IIS Signals

// IIS
#define IISCON  	(*(volatile unsigned int *)0xEEE30000)	//IIS Control
#define IISMOD  	(*(volatile unsigned int *)0xEEE30004)	//IIS Mode
#define IISFIC  	(*(volatile unsigned int *)0xEEE30008)	//IIS FIFO Control
#define IISPSR  	(*(volatile unsigned int *)0xEEE3000C)	//IIS Prescaler
#define IISTXD		(*(volatile unsigned int *)0xEEE30010)	//IIS TXD DATA
#define IISRXD 		(*(volatile unsigned int *)0xEEE30014)	//IIS RXD DATA
#define IISFICS  	(*(volatile unsigned int *)0xEEE30018)	//IIS FIFO Control

void iis_init(void)
{
    int N;
    // 配置引脚用于i2s功能
    GPICON = 0x22222222;

    // 设置i2s相关时钟
    // step 1: EPLL output 67.7Mhz (see p361 of s5pv210.pdf)
    // EPLL_CON0/ EPLL_CON1, R/W, Address = 0xE010_0110/0xE010_0114)
    // FOUT = (MDIV+K/65536) X FIN / (PDIV X 2SDIV)
    // Fout = (0x43+0.7)*24M / (3*2^3) = 80*24M/24 = 67.7Mhz
#define EPLL_CON0  	(*(volatile unsigned int *)0xe0100110)
#define EPLL_CON1  	(*(volatile unsigned int *)0xe0100114)
    EPLL_CON0 = 0xa8430303; 	// MPLL_FOUT = 67.7Mhz
    EPLL_CON1 = 0xbcee; 	// from linux kernel setting

    // step 2: MUX_EPLL = 1 -> SCLK_EPLL = 67.7Mhz (see p361 of s5pv210.pdf)
    // CLK_SRC0, R/W, Address = 0xE010_0200
    // 0xe0100200: 10001111
    // EPLL_SEL  [8]  Control MUXEPLL (0:FINPLL, 1:FOUTEPLL)
#define CLK_SRC0  	(*(volatile unsigned int *)0xE0100200)
    CLK_SRC0 = 0x10001111;

    // step 3: Mux_I2S	AUDIO subsystem clock selection (see P1868 P1875 of s5pv210.pdf)
#define CLK_CON  	(*(volatile unsigned int *)0xEEE10000)
    CLK_CON = 0x1;		// 1 = FOUT_EPLL		MUXI2S_A 00 = Main CLK


    // 设置i2s控制器
    // step 4:	Divider of IIS (67.7 -> 11.289Mhz)
    // N + 1 = (67.7Mhz) / (256 * 44.1Khz) = 5.99
    // IISCDCLK  11.289Mhz = 44.1K * 256fs
    // IISSCLK	  1.4112Mhz = 44.1K * 32fs
    // IISLRCLK   44.1Khz
    N = 5;
    IISPSR = 1<<15 | N<<8;

    // IIS interface active (start operation).	1 = Active
    IISCON |= 1<<0 | (unsigned)1<<31;

    // [9:8] 10 = Transmit and receive simultaneous mode
    // 1 = Using I2SCLK 	(use EPLL)
    IISMOD = 1<<9 | 0<<8 | 1<<10;

}

// IIC GPIO
#define GPD1CON    	(*(volatile unsigned *)0xE02000C0) //Port D1 control
#define GPD1DAT    	(*(volatile unsigned *)0xE02000C4) //Port D1 data
#define GPD1PUD    	(*(volatile unsigned *)0xE02000C8) //Pull-up control D

// IIC SFR
#define I2CCON0  	(*(volatile unsigned *)0xE1800000) //IIC control
#define I2CSTAT0 	(*(volatile unsigned *)0xE1800004) //IIC status
#define I2CADD0  	(*(volatile unsigned *)0xE1800008) //IIC address
#define I2CDS0   	(*(volatile unsigned *)0xE180000C) //IIC data shift
#define I2CLC0   	(*(volatile unsigned *)0xE1800010) //IIC multi-master line control 

// for portability of IIC driver
#define IICCON	I2CCON0
#define IICSTAT	I2CSTAT0
#define IICADD	I2CADD0
#define IICDS	I2CDS0


void iic_write(int slave_addr, int addr, int data)
{
	I2CDS0 = slave_addr;

	// bit[7:6]: 主机发送器
	// bit[5]:发出s信号和I2CDS0里的从机地址
	// bit[4]:使能tx/rx
	I2CSTAT0 = 0xf0;

	
	while ((I2CCON0 & 0x10) == 0);					// 等待数据发送
	while ((I2CSTAT0 & 0x1));						// 等待从机发来ACK


	// 发7bit地址和9bit数据
	I2CDS0 = addr<<1 | ((data>>8) & 0x0001);
	I2CCON0 &= ~(1<<4);								// 清中断
	while ((I2CCON0 & 0x10) == 0);					// 等待数据发送
	while ((I2CSTAT0 & 0x1));						// 等待从机发来ACK
	I2CDS0 = (data & 0x00FF);
	
	I2CCON0 &= ~(1<<4);								// 清中断
	while ((I2CCON0 & 0x10) == 0);					// 等待数据发送
	while ((I2CSTAT0 & 0x1));						// 等待从机发来ACK
	


	// bit[7:6]: 主机发送器
	// bit[5]: 发出p信号
	// bit[4]: 使能tx/rx
	I2CSTAT0 = 0xd0;
	I2CCON0 &= ~(1<<4);								// 清中断
	
	// 延时等待
	int i=0;
	for(i=0; i<50; i++);
	return;

}

void iic_init(void)
{
    // 配置引脚用于i2c功能
    GPD1CON |= 0x22;
    GPD1PUD |= 0x5;

    // bit[7] = 1: Enable ACK
    // bit[6] = 0: Prescaler IICCLK=PCLK/16
    // bit[5] = 1: Enable interrupt
    // bit[0:3] = 0xf:Transmit clock value = IICCLK/(15+1)
    I2CCON0  = (1<<7) | (0<<6) | (1<<5) | (0xf);

    // bit[4] = 1: 使能接收和发送功能
    I2CSTAT0 = 0x10;

}

void wm8960_init(void)
{
    // bit[7:1]: 0x1a
    // bit[0]:0: write
#define WM8960_DEVICE_ADDR		0x34

    // 重置
    iic_write(WM8960_DEVICE_ADDR, 0xf, 0x0);

    // 设置电源
	iic_write(WM8960_DEVICE_ADDR, 0x19, 1<<8 | 1<<7 | 1<<6);
	iic_write(WM8960_DEVICE_ADDR, 0x1a, 1<<8 | 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3);
	iic_write(WM8960_DEVICE_ADDR, 0x2F, 1<<3 | 1<<2);


    // 设置时钟
    iic_write(WM8960_DEVICE_ADDR, 0x4, 0x0);

    // 设置ADC-DAC
    iic_write(WM8960_DEVICE_ADDR, 0x5, 0x0);

    // 设置audio interface
    iic_write(WM8960_DEVICE_ADDR, 0x7, 0x2);

    // 设置OUTPUTS
    iic_write(WM8960_DEVICE_ADDR, 0x2, 0xFF | 0x100);
    iic_write(WM8960_DEVICE_ADDR, 0x3, 0xFF | 0x100);

    // 设置DAC VOLUME
    iic_write(WM8960_DEVICE_ADDR, 0xa, 0xFF | 0x100);
    iic_write(WM8960_DEVICE_ADDR, 0xb, 0xFF | 0x100);

    // 设置mixer
    iic_write(WM8960_DEVICE_ADDR, 0x22, 1<<8 | 1<<7);
    iic_write(WM8960_DEVICE_ADDR, 0x25, 1<<8 | 1<<7);

    return;

}

