#include <lpc214x.h>
#include "interrupt.h"
#include "config.h"

/* variables defined in main.c */
extern int menu_flag;
extern int med_ack;

// initialize external interrupts for switches
// SW1 -> menu trigger, SW2 -> medicine acknowledge
void Interrupt_Init(void)
{
    /* configure SW1 pin as EINT0 function */
    PINSEL0 &= ~(3<<2);     // clear pin function bits
    PINSEL0 |= (3<<2);      // select EINT0 function
    /* configure SW2 pin as EINT1 function */
    PINSEL0 &= ~(3<<6);     // clear pin function bits
    PINSEL0 |= (3<<6);      // select EINT1 function
    /* clear any pending interrupt flags */
    EXTINT = (1<<0) | (1<<1);
    /* configure interrupts as edge triggered */
    EXTMODE |= (1<<0) | (1<<1);
    /* select falling edge trigger */
    EXTPOLAR &= ~((1<<0)|(1<<1));
    /* select IRQ mode instead of FIQ */
    VICIntSelect &= ~((1<<14)|(1<<15));
    /* configure vector slot for EINT0 */
    VICVectCntl0 = (1<<5) | 14;           // enable slot and assign interrupt number
    VICVectAddr0 = (unsigned int)eint0_isr; // address of ISR
    /* configure vector slot for EINT1 */
    VICVectCntl1 = (1<<5) | 15;           // enable slot and assign interrupt number
    VICVectAddr1 = (unsigned int)eint1_isr;
    /* enable EINT0 and EINT1 interrupts */
    VICIntEnable |= (1<<14) | (1<<15);
}

/* SW1 interrupt service routine
   used to open menu */
void eint0_isr(void) __irq
{
    menu_flag = 1;      // inform main program to open menu
    EXTINT = (1<<0);    // clear EINT0 interrupt flag
    VICVectAddr = 0;    // signal end of interrupt to VIC

}

/* SW2 interrupt service routine
   used to acknowledge medicine alert */
void eint1_isr(void) __irq
{
    med_ack = 1;        // indicate medicine acknowledged
#ifndef SIMULATION
		IOSET0 = (1<<BUZZER_ALERT);   // stop buzzer alert
#else
    IOSET0 = (1<<LED_ALERT);      // turn off LED alert
#endif
    EXTINT = (1<<1);   // clear EINT1 interrupt flag
    VICVectAddr = 0;   // signal end of interrupt
}

