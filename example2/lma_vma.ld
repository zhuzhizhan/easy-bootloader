SECTIONS
{
	.text 0x1000 :
	{
		*(.text)
		_etext = .;
	}

	.mdata 0x2000 : AT(ADDR(.text) + SIZEOF(.text))
	{
		_data = .;
		*(.data)
		_edata = .;
	}

	.bss 0x3000 :
	{
		_bstart = .;
		*(.bss)
		*(COMMON)
		_bend = .;
	}
}
