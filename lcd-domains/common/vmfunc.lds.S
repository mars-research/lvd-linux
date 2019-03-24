
SECTIONS {
	/* start at a page-aligned address */
	.vmfunc.trampoline.text ALIGN(0x1000): {
		/* I don't know what address this gives */
		__vmfunc_trampoline_load_addr = .;
		KEEP(*(.vmfunc.trampoline.text))
		/* align to a page boundary */
		. = ALIGN(0x1000);
	}
	__vmfunc_trampoline_page_size = SIZEOF(.vmfunc.trampoline.text);

	/* start at a page-aligned address */
	.vmfunc.springboard.text ALIGN(0x1000): {
		/* I don't know what address this gives */
		__vmfunc_sboard_load_addr = .;
		KEEP(*(.vmfunc.springboard.text))
		/* align to a page boundary */
		. = ALIGN(0x1000);
	}
	__vmfunc_sboard_page_size = SIZEOF(.vmfunc.springboard.text);

	/* start at a page-aligned address */
	.vmfuncwrapper.text ALIGN(0x1000): {
		/* I don't know what address this gives */
		__vmfuncwrapper_load_addr = .;
		KEEP(*(.vmfuncwrapper.text))
		/* align to a page boundary */
		. = ALIGN(0x1000);
	}
	__vmfuncwrapper_page_size = SIZEOF(.vmfuncwrapper.text);
}
/* XXX: ld fails to respect the alignment if I insert it AFTER .text */
INSERT BEFORE .text;
