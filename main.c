#include "uart.h"
#include "sysreg.h"
#include "gpio.h"
#include "gic.h"
#include "systimer.h"

// Function prototypes
void init_GPIO0_to_risingEdgeInterrupt();
void init_GPIO1_to_fallingEdgeInterrupt();


void init_GPIO4_to_output();
void set_GPIO4();
void clear_GPIO4();

void init_GPIO12_to_output();
void set_GPIO12();
void clear_GPIO12();

void init_GPIO16_to_output();
void set_GPIO16();
void clear_GPIO16();

// Global shared variable to track state
unsigned int sharedValue;
unsigned int redBool;

void main()
{
    unsigned int value, el, i;

    
    // Set up the UART serial port
    uart_init();

    // Print out initial values before setting GIC, etc.
    uart_puts("Initial Values:\n");
    
    // Query the current exception level
    el = getCurrentEL();
    
    // Print out the exception level
    uart_puts("  Exception level:    0x");
    uart_puthex(el);
    uart_puts("\n");
    
    // Get the SPSel value
    value = getSPSel();
    
    // Print out the SPSel value
    uart_puts("  SPSel:              0x");
    uart_puthex(value);
    uart_puts("\n");
        
    // Query the current DAIF flag values
    value = getDAIF();
    
    // Print out the DAIF flag values
    uart_puts("  DAIF flags:         0x");
    uart_puthex(value);
    uart_puts("\n");

    // Print out initial values the GPREN0 register (rising-edge interrupt
    // enable register)
    value = *GPREN0;
    uart_puts("  GPREN0:             0x");
    uart_puthex(value);
    uart_puts("\n");
    
    // Print out initial values the GPFEN0 register (falling-edge interrupt
    // enable register)
    value = *GPFEN0;
    uart_puts("  GPFEN0:             0x");
    uart_puthex(value);
    uart_puts("\n");
    
    sharedValue = 0;
    redBool = 1;
    

    // Set and print out new values
    uart_puts("\nResetting to new values:\n");
    
    // Set up GPIO pins
    init_GPIO0_to_risingEdgeInterrupt();
    init_GPIO1_to_fallingEdgeInterrupt();
    init_GPIO4_to_output();
    init_GPIO12_to_output();
    init_GPIO16_to_output();
    
    // Enable IRQ Exceptions on the CPU core
    enableIRQ();
    
    // Query the current DAIF flag values
    value = getDAIF();
    
    // Print out the DAIF flag values
    uart_puts("  DAIF flags:         0x");
    uart_puthex(value);
    uart_puts("\n");

    uart_puts("  GPREN0:             0x");
    value = *GPREN0;
    uart_puthex(value);
    uart_puts("\n");

    uart_puts("  GPFEN0:             0x");
    value = *GPFEN0;
    uart_puthex(value);
    uart_puts("\n\n");

    
    // Set up the Generic Interrupt Controller

    for (i = 0; i < 16; i++) {
		*(GIC_GICD_IPRIORITYR + (i * 4)) = 0x00000000;
    }

    for (i = 8; i < 16; i++) {
		*(GIC_GICD_ITARGETSR + (i * 4)) = 0x01010101;
    }

    for (i = 1; i < 4; i++) {
		*(GIC_GICD_ICFGR + (i * 4)) = 0xFFFFFFFF;
    }

    uart_puts("Enabling Bank 0 GPIO interrupts (pins 0 - 27) in GIC:\n");
    *(GIC_GICD_ISENABLER + (1 * 4)) = 0x00020000;

    // Print out enabled interrupts
    value = *(GIC_GICD_ISENABLER + (0 * 4));
    uart_puts("  GICD_ISENABLER0:    0x");
    uart_puthex(value);
    uart_puts("\n");
    value = *(GIC_GICD_ISENABLER + (1 * 4));
    uart_puts("  GICD_ISENABLER1:    0x");
    uart_puthex(value);
    uart_puts("\n\n");

    if (el == 0x3) {
		*GIC_GICD_CTLR = 0x3;
		uart_puts("Enabling GIC forwarding of Group 0 and 1 interrupts.\n");
    } else {
		*GIC_GICD_CTLR = 0x1;
		uart_puts("Enabling GIC forwarding of Group 1 interrupts.\n");
    }
    value = *GIC_GICD_CTLR;
    uart_puts("  GICD_CTLR:          0x");
    uart_puthex(value);
    uart_puts("\n\n");

    // Main loop
    uart_puts("\nStarting main loop...\n");

    while (1) {
        if (sharedValue == 0) {
            uart_puts("\nState 1\n");
            // redBool = 1;
            set_GPIO16(); clear_GPIO12(); clear_GPIO4();
            microsecond_delay(500000);

            // redBool = 1;
            set_GPIO12(); clear_GPIO16(); clear_GPIO4();
            microsecond_delay(500000);

            // redBool = 0;
            set_GPIO4(); clear_GPIO12(); clear_GPIO16();
            microsecond_delay(500000);
        } else if (sharedValue == 1){
            uart_puts("\nState 2\n");
            // redBool = 0;
            set_GPIO4(); clear_GPIO12(); clear_GPIO16();
            microsecond_delay(250000);

            // redBool = 1;
            set_GPIO12(); clear_GPIO4(); clear_GPIO16();
            microsecond_delay(250000);

            // redBool = 1;
            set_GPIO16(); clear_GPIO4(); clear_GPIO12();
            microsecond_delay(250000);
        }

        value = 0x0000FFFF;
        while (value--) {
            asm volatile("nop");
        }
    }
}

void init_GPIO0_to_risingEdgeInterrupt()
{
    register unsigned int r;
    r = *GPFSEL0;
    r &= ~(0x7); // Set GPIO 0 as input
    *GPFSEL0 = r;

    r = 150;
    while(r--){
        asm volatile("nop");
    }

    r = *GPPUPPDN0;
    r &= ~(0x3); // Disable pull-up/down
    r |= (0x1 << 1);
    *GPPUPPDN0 = r;

    r = 150;
    while(r--){
        asm volatile("nop");
    }

    *GPREN0 = (0x1 << 0); // Enable rising edge detect for GPIO 0
}

void init_GPIO1_to_fallingEdgeInterrupt()
{
    register unsigned int r;

    r = 150;
    while(r--){
        asm volatile("nop");
    }

    r = *GPFSEL0;
    r &= ~(0x7 << 3); // Set GPIO 1 as input
    *GPFSEL0 = r;

    r = 150;
    while(r--){
        asm volatile("nop");
    }

    r = *GPPUPPDN0;
    r &= ~(0x3 << 2); // Disable pull-up/down
    *GPPUPPDN0 = r;

    r = 150;
    while(r--){
        asm volatile("nop");
    }
    
    *GPFEN0 = (0x1 << 1);
}

void init_GPIO4_to_output()
{
    register unsigned int r = *GPFSEL0;
    r &= ~(0x7 << 12); // Clear FSEL bits for GPIO 4
    r |= (0x1 << 12);  // Set GPIO 4 as output
    *GPFSEL0 = r;
}

void set_GPIO4() {
	*GPREN0 &= ~(0x1 << 0);
	*GPSET0 = (0x1 << 4);
	*GPREN0 |= (0x1 << 0);
}

void clear_GPIO4() { *GPCLR0 = (0x1 << 4); }

void init_GPIO12_to_output()
{
    register unsigned int r = *GPFSEL1;
    r &= ~(0x7 << 6); // Clear FSEL bits for GPIO 12
    r |= (0x1 << 6);  // Set GPIO 12 as output
    *GPFSEL1 = r;
}

void set_GPIO12() { *GPSET0 = (0x1 << 12); }
void clear_GPIO12() { *GPCLR0 = (0x1 << 12); }

void init_GPIO16_to_output()
{
    register unsigned int r = *GPFSEL1;
    r &= ~(0x7 << 18); // Clear FSEL bits for GPIO 16
    r |= (0x1 << 18);  // Set GPIO 16 as output
    *GPFSEL1 = r;
}

void set_GPIO16() { *GPSET0 = (0x1 << 16); }
void clear_GPIO16() { *GPCLR0 = (0x1 << 16); }