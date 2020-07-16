MEMORY
{
	flash(rx) : ORIGIN  = 0x08000000,LENGTH = 64K
	sram (rwx): ORIGIN = 0x20000000, LENGTH = 20K  
}

SECTIONS
{
	/*Holds Constants - Code - Interrupt Vectors  and are aligned at 4 bytes*/
	.text :
	{
		. = ALIGN(4);
		KEEP(*(.interruptsvector))		/* KEEP command skips optimizing this section */
		*(.text)
		*(.const)
		*(.rodata)
	} > flash
	
	/* Holds Static variables*/ 
	.data :
	{
		. = ALIGN(4);
		_data_start = . ;				/* Get the Virtual MemoAddr load address*/
		*(.data)
		. = ALIGN(4);
		_data_end = . ;
	} > sram AT > flash		

	_data_load= LOADADDR(.data);	/*Fetch load address from LoadMemoAddr([physical])*/


	/* Holds uninitialized Data */
	.bss :
	{
		. = ALIGN(4);
		_bss_start = . ;
		*(.bss)
		*(.BSS)
		*(COMMON)
		. = ALIGN(4);
		_bss_end = . ;

	} > sram

} 