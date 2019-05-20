#include "stdio.h"
#include "lcd.h"

#define WIDTHEIGHT	480
#define HEIGHT	272

void uart_init(void);

int main(void)
{
	int c = 0;

	// ��ʼ������
	uart_init();

	// ��ʼ��LCD
	lcd_init();

	// ��ӡ�˵�
	while(1)
	{
		printf("\r\n###############LCD Test##############\r\n");
		printf("[1] lcd_clear_screen\r\n");
		printf("[2] lcd_draw_cross\r\n");
		printf("[3] lcd_draw_hline\r\n");
		printf("[4] lcd_draw_vline\r\n");
		printf("[5] lcd_draw_circle\r\n");
		printf("Enter your choice:");
		c = getc();
		printf("%c\r\n",c);
		switch(c)
		{
			case '1':
				// ����
				lcd_clear_screen(0x000000);									// ��
				break;
			case '2':
				// ��ʮ��
				lcd_draw_cross(50, 50, 20, 0x0000ff);						// ��
				break;
			case '3':
				// ������
				lcd_draw_hline(HEIGHT/2, 100, WIDTHEIGHT-100, 0xff0000);	// ��
				break;
			case '4':
				// ������
				lcd_draw_vline(WIDTHEIGHT/2, 50, HEIGHT-50, 0x00ff00);		// ��
				break;
			default:
				break;
		}
	}
	return 0;
}