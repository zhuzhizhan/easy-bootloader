INCLUDE linkcmds.memory

SECTIONS
{
	.text :
	{
		*(.text)
	} > REGION_TEXT

	.rodata :
	{
		*(.rodata)
		rodata_end = .;
	} > REGION_RODATA

	.data : AT(rodata_end)
	{
		data_start = .;
		*(.data)
	} > REGION_DATA

	data_size = SIZEOF(.data);
	data_load_start = LOADADDR(.data);

	.bss :
	{
		*(.bss)
	} > REGION_BSS
}
