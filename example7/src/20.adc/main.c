#include "stdio.h"

#define W	480
#define H	272

#define POS	50
#define RADIUS	20

void uart_init(void);
void adc_test(void);

int main(void)
{
	// ³õÊ¼»¯´®¿Ú
	uart_init();				

	// ²âÊÔADC
	adc_test();

	return 0;
}
