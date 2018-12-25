
SECTIONS {
	/* start at a page-aligned address */
	.vmfunc.call ALIGN(0x1000): {
		/* I don't know what address this gives */
		__vmfunccall_load_addr = .;
		KEEP(*(.vmfunc.call))
		/* align to a page boundary */
		. = ALIGN(0x1000);
	}
	__vmfunccall_page_size = SIZEOF(.vmfunc.call);

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
