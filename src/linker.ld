ENTRY(entry)
OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 0x8000;

	.text :
	{
		*(.text)
	}

	.gdt :
	{
		*(.gdt)
	}

	.idt :
	{
		*(.idt)
	}

	.rodata :
	{
		*(.rodata)
	}

	.data :
	{
		*(.data)
	}

	.bss :
	{
		*(COMMON)
		*(.bss)
	}
}
