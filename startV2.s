// startV2:  This version demonstrates how to set up an environment that
//           supports interrupt handling.
//
// This routine is used to establish an environment in which a C program can
// run. We create this environment only on CPU Core 0. The other cores simply
// run an infinite loop.
//
// The stack pointer register is initialized to point just below the text
// section of the program. It grows backwards (toward 0), so it uses memory
// addresses below that of the _start routine.
//
// We also zero out all bytes in the .bss section, and then branch to the main()
// routine. The main() routine should never return to this code (it should be in
// an infinite loop), but if it does, we then put the CPU Core 0 into an
// infinite loop.
//
// This version of the start routine also changes the exception level from EL2
// to EL1 (in the aarch64 execution state). The exception vector table is also
// set up, and vector stubs are provided. Only the IRQ handler is implemented,
// and is called from the IRQ stub.


	// Put the machine code for this routine into the .text.boot section
	.section ".text.boot"

	// The _start symbol needs to be visible to the linker since this is
        // where execution starts for bare metal code
	.global _start
_start:
	// Copy the contents of the multiprocessor affinity register into the x1
        // register. The rightmost 2 bits gives us the CPU Core number that this
        // code is running on. We will only continue running the rest of the
        // program if we are on CPU Core 0. We will put all other cores into an
        // infinite loop.
	mrs     x1, mpidr_el1	// Read the MP affinity system register
	tst	x1, 0x3		// Bitwise AND rightmost 2 bits
	b.eq	core_zero	// Skip forward if both bits are 0

	//  If here, the CPU Core number is not 0, so loop forever
loop:  	wfe			// Wait for event puts the core to sleep
	b	loop		// Infinite loop

  	// If here, the CPU Core is 0, and we continue with the rest of the
  	// setup. We are running in EL2 currently, and will change to EL1 below.
core_zero:

	// Set the stack pointer to point to where the _start routine begins.
        // The stack grows backwards (towards 0), so it uses memory that has
        // lower addresses than the _start routine. We need to set this properly
        // so that C functions and assembly routines can allocate stack frames.
	adrp	x1, _start	// Put the _start address into x1
	add	x1, x1, :lo12:_start
	msr	sp_el1, x1	// Copy the address into the EL1 SP register

	// Enable AArch64 in EL1 by setting bits RW and SWIC to 1 in the
	// Hypervisor Configuration Register (see p. D10-2492 and D10-2503 in
	// the ARM Architecture Reference Manual). Since all other bits are 0,
	// most instructions are not trapped, and the Physical SError, IRQ, and
	// FIQ routings are set so that these exceptions are not taken to EL2,
	// but are instead handled at EL1.
	mov	x0, (1 << 31)		// Enable AArch64
	orr	x0, x0, (1 << 1)	// SWIO is hardwired on the Pi
	msr	hcr_el2, x0

	// Set the Vector Base Address Register (EL1) to the address of the
	// vectors defined below
	adrp	x2, _vectors
	add	x2, x2, :lo12:_vectors
	msr     vbar_el1, x2

	// Change execution level to EL1:
	//
	// Set the Saved Program Status Register so that when entering EL1, the
	// DAIF bits are set to 1111 (exceptions are masked) and the M[3:2] bits
	// are set to 01 (EL1) and the M[0] bit is set to 0 (SP is always SP0)
	// (see p. C5-386-387 in the ARM Architecture Reference Manual).
	mov	x2, 0x3C4
	msr	spsr_el2, x2

	// Set the Exception Link Register EL2 to the address of the instruction
	// labelled AtEL1 (a few lines down). We will jump to this instruction
	// when executing the exception return instruction.
	adr	x2, AtEL1
	msr	elr_el2, x2

	// Executing a return from exception forces the processor to change to
	// EL1. We then jump to the instruction at the label below.
	eret


	// Set the current SP to the _start address, as described above. This
	// will be sp_el0.
AtEL1:	mov	sp, x1

	// Clear the .bss section using a loop. The __bss_start symbol indicates
        // where in RAM the .bss section starts. The __bss_size symbol is also
        // provided by the linker, and gives the size (in doublewords) of the
        // .bss section.
	adrp	x1, __bss_start		// Put address of .bss into x1
	add	x1, x1, :lo12:__bss_start
	ldr     w2, =__bss_size		// Put the size of the .bss section
					// into w2, using a literal pool.
					// w2 is our counter.

top:	cbz     w2, endloop		// Exit loop if counter == 0
	str     xzr, [x1], 8		// Write zeroes to RAM, x1 += 8
	sub     w2, w2, 1		// Decrement counter (w2)
	cbnz    w2, top			// Keep looping while counter != 0
endloop:

	// Branch to the main() routine, which should never return
  	bl      main

	// We should never arrive here, but if we do we branch to the infinite
        // loop above
	b       loop



	// Exception handler stubs that are used by the vectors below.

	// A stub that does nothing
_synch_handler:
	eret


_IRQ_handler:
	// Save that state of all general purpose registers. We do this so that
	// any C code that we call from here can use any of the general purpose
	// registers.
	stp	x0, x1, [sp, -16]!
	stp	x2, x3, [sp, -16]!
	stp	x4, x5, [sp, -16]!
	stp	x6, x7, [sp, -16]!
	stp	x8, x9, [sp, -16]!
	stp	x10, x11, [sp, -16]!
	stp	x12, x13, [sp, -16]!
	stp	x14, x15, [sp, -16]!
	stp	x16, x17, [sp, -16]!
	stp	x18, x19, [sp, -16]!
	stp	x20, x21, [sp, -16]!
	stp	x22, x23, [sp, -16]!
	stp	x24, x25, [sp, -16]!
	stp	x26, x27, [sp, -16]!
	stp	x28, x29, [sp, -16]!
	str	x30, [sp, -16]!

	// Call the IRQ handler written in C. You must provide your own handler
	// code, packaged as a C function.
	bl	IRQ_handler

	// Restore state of all general purpose registers
	ldr	x30, [sp], 16
	ldp	x28, x29, [sp], 16
	ldp	x26, x27, [sp], 16
	ldp	x24, x25, [sp], 16
	ldp	x22, x23, [sp], 16
	ldp	x20, x21, [sp], 16
	ldp	x18, x19, [sp], 16
	ldp	x16, x17, [sp], 16
	ldp	x14, x15, [sp], 16
	ldp	x12, x13, [sp], 16
	ldp	x10, x11, [sp], 16
	ldp	x8, x9, [sp], 16
	ldp	x6, x7, [sp], 16
	ldp	x4, x5, [sp], 16
	ldp	x2, x3, [sp], 16
	ldp	x0, x1, [sp], 16

	// Return from exception
	eret


	// A stub that does nothing
_FIQ_handler:
	eret

	// A stub that does nothing
_SError_handler:
	eret




	// Exception Vector Table:
	//
	// The start of the table must be aligned to an address evenly divisible
	// by 2048 (i.e. it must end with 11 zeroes). Furthermore, each entry
	// must also be aligned to an address evenly divisible by 128 (i.e. must
	// end with 7 zeroes), and entries must follow each other consecutively
	// in memory. Each vector can be as long as 32 instructions. Note that
	// only the first 4 entries are supplied, since the other 12 are not
	// used in this code.
	.align 11
_vectors:
	// Synchronous
	.align  7
	b	_synch_handler	// Branch to handler stub defined above

	// IRQ
	.align  7
	b	_IRQ_handler	// Branch to handler stub defined above

	// FIQ
	.align  7
	b	_FIQ_handler	// Branch to handler stub defined above

	// SError
	.align  7
	b	_SError_handler	// Branch to handler stub defined above
