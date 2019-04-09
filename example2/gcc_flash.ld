MEMORY
{
	CODE(rx) : ORIGIN = 0x00100000, LENGTH = 0x00010000
	DATA(rw) : ORIGIN = 0x00200000, LENGTH = 0x00004000
	STACK(rw) : ORIGIN = 0x00204000, LENGTH = 0x00000000
}

SECTIONS
{
	. = 0x00000000;
	.text : 
	{
		*csstartup.o(.text)
	} > CODE = 0

	.text :
	{
		*(.text)        /* remaining code */
		*(.glue_7t) * (.glue_7)
	} > CODE = 0

	. = ALIGN(4);

	/* .rodata section whitch is used for read-only data (constants) */

	.rodata :
	{
		*(.rodata)
	} > CODE

	. = ALIGN(4);

	_etext = .;

	PROVIDE(etext = .);

	/* .data section which is used for initialized data */

	.data : AT(_etext)
	{
		_data = .;
		*(.data)
		SORT(CONSTRUCTORS)
	} > DATA

	. = ALIGN(4);

	_edata = .;

	PROVIDE(edata = .);

	/* .bss section which is used for uninitialized data */

	.bss :
	{
		__bss_start = .;
		*(.bss)
		*(COMMON)
	}

	. = ALIGN(4);

	.init_data :
	{
		*(.internal_ram_top)
	} > STACK

	PROVIDE(end = .);

	/* Stabs debugging sections. */
	.stab 0 : {*(.stab)}
	.stabstr 0 : {*(.stabstr)}
	.stab.excl 0 : {*(.stab.excl)}
	.stab.exclstr 0 : {*(stab.exclstr)}
	.stab.index 0 : {*(stab.index)}
	.stab.indexstr 0 : {*(stab.indexstr)}
	.comment 0 {*(.comment)}
	.debug 0 : {*(.debug)}
	.line 0 : {*(.line)}
	.debug_srcinfo 0 : {*(.debug_scrinfo)}
	.debug_sfnames 0 : {*(.debug_sfnames)}
	.debug_aranges 0 : {*(.debug_aranges)}
	.debug_pubnames 0 : {*(.debug_pubnames)}
	.debug_info 0 : {*(.debug_info.gnu.linkonce.wi.*)}
	.debug_abbrev 0 : {*(.debug_abbrev)}
	.debug_line 0 : {*(.debug_line)}
	.debug_frame 0 : {*(.debug_frame)}
	.debug_str 0 : {*(.debug_str)}
	.debug_loc 0 : {*(.debug_loc)}
	.debug_macinfo 0 : {*(.debug_macinfo)}
	.debug_weaknames 0 : {*(.debug_weaknames)}
	.debug_funcnames 0 : {*(.debug_funcnames)}
	.debug_typenames 0 : {*(.debug_typenames)}
	.debug_varnames 0 : {*(.debug_varnames)}
}
