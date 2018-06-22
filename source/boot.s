/*declares constants for the multiboot header*/

.set ALIGN,		1<<0 /* align loaded modules on page boundaries */
.set MEMINFO, 1<<1 /* provide memory map */
.set FLAGS,		ALIGN | MEMINFO /* this is the Multiboot 'flag' field */
.set MAGIC,		0x1BADB002 /* 'magic number' lets bootloader find the header */
.set CHECKSUM,-(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

/*declares a mutliboot header to indicate that the program is a kernel
this is done by using 'magic' values which are documented by the mutliboot
standard*/

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*it is up to the kernel to provide a stack and the multiboot standard does
not define the value of the stack pointer register. the following allocates 
room for a stack by creating a symbol where the bottom of the stack is intened
to be, allocating 16384 bytes for it, and then setting another symbol at the
top of where the stack is intened to be. the reason that the stack is 16-bytes
is because of x86 standard conventions*/

.section .bss
.align 16
stack_bottom:
.skip 16834 # 16 KiB
stack_top:

/*'_start' is specified as the entry point to the kernel by the linker so that
the bootloader will jump to this position once the kernel has been loaded. there
is no need to return from this function because the bootloader would have gone*/

.section .text
.global _start
.type _start, @function
_start:

				/*at this point the bootloader has loaded us into an environment where we are in
				32-bit protected mode, interrupts are disabled and paging is disabled. here, 
				the kernel lhas full control of the CPU however, the kernel can only make use of
				hardware features. this means that there's no printf function unless the kernel
				provides its own implementation. it also means that there's no security
				restrictions, safeguards or debugging mechanisms- there is only what the kernel
				provides its-self*/

				/*because on x86 systems stacks travel downwards, the esp (stack pointer)
				register must point to the top of our stack*/

				mov $stack_top, %esp

				/*at this point the processor still isnt fully initialized yet and that its
				best to spend a minimal amount of time without the processor initialized
				yet because features such as floating point instructions and instruction
				set extentions are not available.*/

				/*this is where the high level kernel will enter. at the time of the call
				instruction, the ABI (application binary interface) requires that the stack is
				16-byte aligned which is fine because this was done earlier.*/

				call kernel_main

				/*since the systm has nothing more to do it must be put into an infinite loop.
				this is done by disabling interups with cli (clear interrupt enbale in eflags),
				waiting for the next interrupt to arrive (which will never come because
				they are disabled) by using the halt instruction and adding a jump to the halt
				just in case a non-maskable interrupt occurs.*/

				cli
1:			hlt
				jmp 1b

/*to aid in debugging the size of the start symbol is set to to the current location '.'
minus its start.*/

.size _start, . - _start



