#include <lpc214x.h>        // LPC2148 register definitions
#include "lcd_defines.h"    // LCD command and pin definitions
#include "lcd.h"            // LCD function declarations
#include "delay.h"          // Delay functions for timing control

// Function: Init_LCD
// Purpose : Initialize the LCD module
// Reason  : LCD requires a specific initialization sequence to work properly
//           This function configures pins and sets LCD to 8-bit 2-line mode
void Init_LCD(void)
{
        IODIR0 |= 0xFF << LCD_DATA;   // Set LCD data pins (D0-D7) as output
        IODIR0 |= 1 << RS;            // Set RS pin as output (Register Select)
        IODIR0 |= 1 << RW;            // Set RW pin as output (Read/Write control)
        IODIR0 |= 1 << EN;            // Set EN pin as output (Enable signal)
        delay_ms(15);                 // Wait for LCD power stabilization
        Cmd_LCD(MODE_8BIT_1LINE);     // Send command to set 8-bit mode and 1-line display
        delay_ms(5);                  // Delay for command processing
        Cmd_LCD(MODE_8BIT_1LINE);     // Repeat command as required by LCD initialization
        delay_us(100);                // Small delay
        Cmd_LCD(MODE_8BIT_1LINE);     // Repeat again to ensure proper initialization
        Cmd_LCD(MODE_8BIT_2LINE);     // Set LCD to 8-bit mode with 2 display lines
        //Cmd_LCD(DISP_ON_CUR_ON);    // Optional: turn display ON with cursor
        Cmd_LCD(DISP_ON);             // Turn display ON without cursor
        Cmd_LCD(CLEAR_LCD);           // Clear LCD screen
        Cmd_LCD(SHIFT_CUR_RIGHT);     // Cursor automatically moves right after each character
}

// Function: Cmd_LCD
// Purpose : Send command instruction to LCD
// Reason  : Commands control LCD operations like clear display, cursor move etc.
void Cmd_LCD(char cmd)
{
        IOCLR0 = 1 << RS;     // RS = 0 ? Select command register
        Write_LCD(cmd);       // Send command to LCD
}

// Function: Write_LCD
// Purpose : Write data or command to LCD
// Reason  : LCD requires an enable pulse to latch data or command
void Write_LCD(char data)
{
        IOCLR0 = 1 << RW;       // RW = 0 ? Write operation
        // Clear old data bits and place new data on LCD data lines
        IOPIN0 = IOPIN0 & ~(0xFF << LCD_DATA) | data << LCD_DATA;
        IOSET0 = 1 << EN;       // Set EN high to start enable pulse
        delay_us(1);            // Small delay so LCD can read data
        IOCLR0 = 1 << EN;       // Set EN low ? data latched by LCD
        delay_ms(2);            // Wait for LCD to complete operation
}


// Function: Char_LCD
// Purpose : Display a single character on LCD
// Reason  : LCD displays characters one at a time
void Char_LCD(char ch)
{
        IOSET0 = 1 << RS;      // RS = 1 ? Select data register

        Write_LCD(ch);         // Send character to LCD
}

// Function: Str_LCD
// Purpose : Display a string on LCD
// Reason  : Simplifies printing multiple characters
void Str_LCD(char * p)
{
        while(*p)              // Loop until null character '\0'
        {
                Char_LCD(*p++);   // Display character and move pointer to next
        }
}

// Function: Uint_LCD
// Purpose : Display unsigned integer on LCD
// Reason  : LCD can only display characters, so numbers must be converted to ASCII
void Uint_LCD(unsigned int n)
{
        char a[10];           // Array to store digits
        int i = 0;            // Index variable

        if( n == 0)           // If number is zero
        {
                Char_LCD('0'); // Directly display '0'
        }
        else
        {
                // Extract digits and store in reverse order
                while(n)
                {
                        a[i++] = (n%10) + 48;   // Convert digit to ASCII
                        n/=10;                  // Remove last digit
                }

                // Display digits in correct order
                for(--i; i >= 0; i--)
                        Char_LCD(a[i]);
        }
}

// Function: Sint_LCD
// Purpose : Display signed integer on LCD
// Reason  : Handles negative numbers
void Sint_LCD(int n)
{
        if(n<0)              // Check if number is negative
        {
                Char_LCD('-'); // Display minus sign
                n=-n;          // Convert negative number to positive
                Uint_LCD(n);   // Display number using unsigned function
        }
}
