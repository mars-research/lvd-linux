
SECTIONS {
	/* start at a page-aligned address */
	.vmfunc.text ALIGN(0x1000): {
		/* I don't know what address this gives */
		__vmfunc_load_addr = .;
		*(.vmfunc.text)
		/* align to a page boundary */
		. = ALIGN(0x1000);
	}
	__vmfunc_page_size = SIZEOF(.vmfunc.text);
}
/* XXX: ld fails to respect the alignment if I insert it AFTER .text */
INSERT BEFORE .text;
