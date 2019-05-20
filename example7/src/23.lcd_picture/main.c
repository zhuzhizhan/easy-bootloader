#include "stdio.h"
#include "lcd.h"
#include "bmp.h"

#define WIDTHEIGHT	480
#define HEIGHT	272

void uart_init(void);

int main(void)
{
	printf("Draw picture\r\n");
	
	// ��ʼ������
	uart_init();

	// ��ʼ��LCD
	lcd_init();

	// ��ͼ
	lcd_clear_screen(0x0);
	lcd_draw_bmp(gImage_bmp);

	// ���ַ�
	printf("FriendlyARM\r\n");
	
	return 0;
}