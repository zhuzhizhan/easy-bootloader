SECTIONS
{
	. = 0x23E00000;
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
