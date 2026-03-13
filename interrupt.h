#ifndef INTERRUPT_H
#define INTERRUPT_H

void Interrupt_Init(void);

void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

#endif

