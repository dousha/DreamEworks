ENTRY(entry)
OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 0xd200;

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
		*(.boot_stack)
	}
}
