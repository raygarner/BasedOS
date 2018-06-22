/*the bootloader will look at this and start execution at the symbol
which is currently designated as the entry point*/
ENTRY(_start)

/*this dicates where the various sectins of the object files will be
put in the final kernel image*/
SECTIONS{
	/*begin adding sections at 1MiB*/
	. = 1M;

	/*the mutliboot header must be put be put early in the kernel
	image else the bootloader won't recognize it. next is the text section*/
	.text BLOCK(4K) : ALIGN(4K){
		*(.multiboot)
		*(.text)
	}

	/*read only data*/
	.rodata BLOCK(4K) : ALIGN(4K){
		*(.multiboot)
		*(.text)
	}

	/*read-write data (initialized)*/
	.data BLOCK(4K) : ALIGN(4K){
		*(.data)
	}

	/*read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K){
		*(COMMON)
		*(.bss)
	}

}	

