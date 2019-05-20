SECTIONS
{
	. = 0x20000000;
	.text : {
		start.o
		* (.text)
	}
	.bss : {
		* (.bss)
	}
	.data : {
		* (.data)
	}	
}
