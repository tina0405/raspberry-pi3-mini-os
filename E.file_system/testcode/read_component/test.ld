ENTRY(oper_compt)
SECTIONS
{
	
	.text :  { *(.text) }
	__init_array_start = .;	
	.rodata : { *(.rodata) }
	__init_array_end =.;
	.data : { *(.data) }
	.bss : { *(.bss*) } 
	
}

