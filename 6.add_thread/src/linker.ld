SECTIONS
{
	. = 0xffff000000000000;
	.text.boot : { *(.text.boot) }
	. = ALIGN(0x00001000);
	
        testc_begin = .;
	.text.test : { build/testcode* (.text) }
	.rodata.test : { build/testcode* (.rodata) }
	.data.test : { build/testcode* (.data) }
	.bss.test : { build/testcode* (.bss) }
	testc_end = .;	

	. = ALIGN(0x00001000);
	user_begin = .;
	.text.user : { build/user* (.text) }
	.rodata.user : { build/user* (.rodata) }
	.data.user : { build/user* (.data) }
	.bss.user : { build/user* (.bss) }
	user_end = .;
	
	.text :  { *(.text) }
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	. = ALIGN(0x8);
	bss_begin = .;
	.bss : { *(.bss*) } 
	bss_end = .;
	. = ALIGN(0x00001000);
	pg_dir = .;
	.data.pgd : { . += (3 * (1 << 12)); }	
	_end = .;
	
}

