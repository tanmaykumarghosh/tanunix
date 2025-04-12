OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386:i386)
ENTRY(start)
phys = 0x00100000;
SECTIONS
{
	/*http://wiki.osdev.org/Grub_Error_13 */
	. = phys;
	.__mbHeader : AT ( ADDR( .__mbHeader ) )
	{
	*(.__mbHeader)
	}
	.text : AT ( ADDR(.text) )
	{
	 code = .;
	 *(.text)
	  . = ALIGN(4096);
	}
	.data : AT ( ADDR(.text) + (data - code) )
	{
	 data = .;
	 *(.data)
	 . = ALIGN(4096);
	}
	.bss : AT( ADDR(.text) + (bss - code) )
	{
	 bss = .;
	 *(.bss)
	 . = ALIGN(4096);
	}
	end = .;
}
