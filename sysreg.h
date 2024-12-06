//  C language function prototypes for the functions in sysreg.s, which are
//  written in assembly

unsigned int getCurrentEL();
unsigned int getSPSel();
unsigned int getNZCV();
unsigned int getDAIF();

void enableDAIF();
void disableDAIF();
void enableIRQ();
void disableIRQ();
void enableFIQ();
void disableFIQ();
