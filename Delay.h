#ifndef DELAY_H              // check if DELAY_H is already defined
#define DELAY_H              // define DELAY_H to prevent multiple inclusion
// function prototype for microsecond delay
void delay_us(unsigned int us);   // generates delay in microseconds
// function prototype for millisecond delay
void delay_ms(unsigned int ms);   // generates delay in milliseconds
// function prototype for second delay
void delay_s(unsigned int s);     // generates delay in seconds
#endif                       // end of header guard

