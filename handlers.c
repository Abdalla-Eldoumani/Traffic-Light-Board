// This file contains a C function to handle IRQ exceptions

// Header files
#include "uart.h"
#include "gpio.h"
#include "sysreg.h"
#include "gic.h"


// Reference to the global shared value
extern unsigned int sharedValue;
extern unsigned int redBool;



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       IRQ_handler
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function first prints out some basic information about
//                  the state of the interrupt controller, GPIO pending
//                  interrupts, and selected system registers. It then
//                  determines the particular kind of pending interrupt (which
//                  for the moment is a falling edge event on GPIO pin 1). The
//                  interrupt is cleared, and interrupt is handled in a simple-
//                  minded way by incrementing the shared global variable.
//
////////////////////////////////////////////////////////////////////////////////

void IRQ_handler()
{
    unsigned int r, ack, interruptID, CPUID;


    // Print out exception type
    uart_puts("\n\nInside IRQ exception handler:\n");

    // Print out the current exception level
    r = getCurrentEL();
    uart_puts("  Exception level:    0x");
    uart_puthex(r);
    uart_puts("\n");
    
    // Print out the value of the DAIF flags
    r = getDAIF();
    uart_puts("  DAIF flags:         0x");
    uart_puthex(r);
    uart_puts("\n");

    // Acknowledge the interrupt in the GIC. This also retrieves the Interrupt
    // ID and CPUID
    ack = *GIC_GICC_IAR;

    // Isolate the Interrupt ID from the raw acknowledge value
    interruptID = ack & 0x3ff;

    // Isolate the CPU ID from the raw acknowledge value
    CPUID = (ack & 0xc00) >> 10;

    // Print out raw acknowledge register value
    uart_puts("  Raw GICC_IAR:       0x");
    uart_puthex(ack);
    uart_puts("\n");

    // Print out Interrupt ID
    uart_puts("    Interrupt ID:     0x");
    uart_puthex(interruptID);
    uart_puts("\n");

    // Print out the CPU ID
    uart_puts("    CPU ID:           0x");
    uart_puthex(CPUID);
    uart_puts("\n");

    // Print out active interrupts after acknowledge
    uart_puts("  Active interrupts after acknowledge:\n");
	r = *(GIC_GICD_ISACTIVER + (0 * 4));
	uart_puts("    GICD_ISACTIVER0:  0x");
	uart_puthex(r);
	uart_puts("\n");
	r = *(GIC_GICD_ISACTIVER + (1 * 4));
	uart_puts("    GICD_ISACTIVER1:  0x");
	uart_puthex(r);
	uart_puts("\n");

    // Print out the GPIO event detect register. This tells us which particular
    // GPIO pin caused the interrupt.
    r = *GPEDS0;
    uart_puts("  GPIO GPEDS0:        0x");
    uart_puthex(r);
    uart_puts("\n");

    
    if (*GPEDS0 & (0x1)){
        *GPEDS0 &= (0x1);

        // if (redBool){
        // }
        sharedValue = 0;
    }
    	
    if (*GPEDS0 & (0x1 << 1)) {
        *GPEDS0 &= (0x1 << 1);

        // if (redBool){
        // }
        sharedValue = 1;
    }
    

    // Signal end of interrupt to the GIC. We do this by writing the ack value
    // to the EOI register.
    *GIC_GICC_EOIR = ack;

    // Print out end of handler
    uart_puts("Exiting IRQ exception handler\n\n");
      
    // Return to the IRQ exception handler stub
    return;
}
