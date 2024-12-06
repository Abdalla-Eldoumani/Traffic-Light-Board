//  This file provides functions to query and set various system registers. It
//  is written in assembly code, since the system registers must be written to
//  or read from using the msr and mrs instructions.


		.text
		.balign 4

		.global getCurrentEL
getCurrentEL:	mrs	x0, CurrentEL
		lsr	x0, x0, 2
		and	x0, x0, 0x3
		ret


		.global getSPSel
getSPSel:	mrs	x0, SPSel
		ret


		.global getNZCV
getNZCV:	mrs	x0, NZCV
		lsr	x0, x0, 28
		and	x0, x0, 0xF
		ret


		.global getDAIF
getDAIF:	mrs	x0, DAIF
		lsr	x0, x0, 6
		and	x0, x0, 0xF
		ret


		.global enableDAIF
enableDAIF:	msr	DAIFClr, 0b1111
		ret


		.global disableDAIF
disableDAIF:	msr	DAIFSet, 0b1111
		ret


		.global enableIRQ
enableIRQ:	msr	DAIFClr, 0b0010
		ret


		.global disableIRQ
disableIRQ:	msr	DAIFSet, 0b0010
		ret


		.global enableFIQ
enableFIQ:	msr	DAIFClr, 0b0001
		ret


		.global disableFIQ
disableFIQ:	msr	DAIFSet, 0b0001
		ret




