/* a list of files to link (others may be supplied on the command line) */
INPUT(libc.a libg.a libgcc.a libc.a libgcc.a)

/* output format (can be overridden on command line) */
OUTPUT_FORMAT("coff-sh")

/* output fileanme (can be overriden command line) */
OUTPUT_FILENAME("main.out")

/* our program's entry point; not useful for much except to
 * make sure the S7 record is proper, because the reset vector
 * actually defines the "entrypoint" in most embedded systems */
ENTRY(_start)

/* list of our memory sections */
MEMORY
{
	vect : o = 0, l = 1k
	rom : o = 0x400, l = 127k
	ram : o = 0x400000, l = 128k
	cache : o = 0xfffff000, l = 4k
}

/* how we're organizing memory sections defined in each module */
SECTIONS
{
	/* the interrupt vector table */
	.vect :
	{
		__vect_start = .;
		*(.vect);
		__vect_end = .;
	} > vect

	/* code and constants */
	.text :
	{
		__text_start = .;
		*(.text)
		*(.strings)
		__text_end = .;
	} > rom

	/* uninitialized data */
	.bss :
	{
		__bss_start = .;
		*(.bss)
		*(COMMON)
		__bss_end = .;
	} > ram

	/* initialized data */
	.init : AT (__text_end)
	{
		__data_start = .;
		*(.data)
		__data_end = .;
	} > ram

	/* application stack */
	.stack :
	{
		__stack_start = .;
		*(.stack)
		__stack_end = .;
	} > ram
}
