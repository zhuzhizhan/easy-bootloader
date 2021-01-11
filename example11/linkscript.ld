ENTRY(_Reset)

SECTIONS
{
	. = 0x0;
	.text : { startup1.o (.vector_table) *(.text) }
	. = ALIGN(8);
}
