#ifndef INTERRUPT_H            // check if INTERRUPT_H is already defined
#define INTERRUPT_H            // define INTERRUPT_H to prevent multiple inclusion

// function prototype to initialize external interrupts
void Interrupt_Init(void);    // configures SW1 and SW2 interrupt pins and VIC
// interrupt service routine for EINT0
void eint0_isr(void) __irq;   // executed when SW1 (P0.1) interrupt occurs
// interrupt service routine for EINT1
void eint1_isr(void) __irq;   // executed when SW2 (P0.3) interrupt occurs
#endif                        // end of header guard
