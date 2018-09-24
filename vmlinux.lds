OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")

OUTPUT_ARCH(i386)

ENTRY(start)

LOAD_OFFSET = 0xC0000000;
SECTIONS{
	. = 16M;
	.text.head : {
		*(.text.head)
		. = ALIGN(4K);
	}
		. += LOAD_OFFSET;
	.text : AT(ADDR(.text) - LOAD_OFFSET) {

	}
	.data : AT(ADDR(.data) - LOAD_OFFSET) {
		*(.data)
	}
	.bss : AT(ADDR(.bss) - LOAD_OFFSET) {
		*(.bss)
	}
	. = ALIGN(4K);
	__page_table_base = .;
	. += 1M;
}
