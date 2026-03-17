#include "delay.h"          // include delay function declarations

// function to generate delay in microseconds
void delay_us(unsigned int us)
{
    for(us *= 12; us > 0; us--);   // multiply input by 12 (approx cycles per microsecond)
                                   // loop runs until us becomes 0 to create small delay
}
// function to generate delay in milliseconds
void delay_ms(unsigned int ms)
{
    for(ms *= 12000; ms > 0; ms--);   // multiply ms by 12000 to approximate 1 millisecond delay
                                      // loop keeps decrementing until value reaches 0
}
// function to generate delay in seconds
void delay_s(unsigned int s)
{
    for(s *= 12000000; s > 0; s--);   // multiply seconds by 12,000,000 to approximate 1 second delay
                                      // loop decreases until counter reaches 0
}
