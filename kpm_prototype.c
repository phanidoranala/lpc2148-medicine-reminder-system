#include <lpc21xx.h>        // LPC2148 register definitions
#include "lcd_defines.h"    // LCD command definitions
#include "lcd.h"            // LCD functions
#include "kpm_defines.h"    // keypad definitions
#include "delay.h"          // delay functions

// lookup table for keypad characters
// rows × columns mapping of keypad
unsigned int kpmLUT[4][4]=
{
 {'7','8','9','A'},
 {'4','5','6','B'},
 {'1','2','3','C'},
 {'*','0','=','D'}
};

// Function : initKPM
// Purpose  : Initialize keypad pins
// Rows are outputs and columns are inputs
void initKPM(void)
{
        IODIR1 |=  0xF<<R0;   // set row pins (R0-R3) as output

        IOSET1 |= 0xF<<C0;    // set column pins (C0-C3) HIGH (pull-up)
}


// Function : colscan
// Purpose  : Check whether any key is pressed
// Returns 0 if key pressed, otherwise 1
unsigned int colscan(void)
{
        // read column bits and check if any column becomes LOW
        if(((IOPIN1>>C0)&0xF)<15)
                return 0;     // key pressed
        else
                return 1;     // no key pressed
}


// Function : rowcheck
// Purpose  : Identify which row contains the pressed key
unsigned int rowcheck(void)
{
        unsigned int  rn;    // variable to store row number

        for(rn=0;rn<4;rn++)  // check each row
        {
                // make one row LOW and others HIGH
                IOPIN1 = ((IOPIN1 &~(0xF<<R0))|(~(1<<rn)<<R0));
                if(colscan()==0)  // if column detects key press
                        break;    // row found
        }
        // reset rows back to LOW
        IOPIN1 = ((IOPIN1 &~(0xF<<R0))|(0<<R0));
        return rn;  // return detected row number
}



// Function : colcheck
// Purpose  : Identify which column contains the pressed key
unsigned int colcheck(void)
{
        unsigned int  cn;   // variable to store column number
        for(cn=0;cn<4;cn++) // check each column
        {
                // if column pin becomes LOW ? key pressed
                if(((IOPIN1>>(C0+cn))&1)==0)
                        break;
        }
        return cn;  // return detected column number
}

// Function : keyscan
// Purpose  : Scan keypad and return pressed key character
unsigned int  keyscan(void)
{
        unsigned int  rn,cn,keyV;
        while(colscan());   // wait until a key is pressed
        rn=rowcheck();      // detect which row key belongs to
        cn=colcheck();      // detect which column key belongs to
        keyV=kpmLUT[rn][cn]; // get key value from lookup table
        while(!colscan());  // wait until key is released
        return keyV;        // return detected key
}



// Function : ReadNum
// Purpose  : Read numeric value from keypad
// Supports :
// 0-9 digits entry
// B = backspace
// A = exit
// C = skip
// = = enter
int ReadNum(void)
{
    char key;        // store pressed key
    int sum = 0;     // store final number
    int digits = 0;  // count number of digits entered
    while(1)         // continuously check keypad
    {
        key = keyscan();   // get key from keypad
        // check if key is numeric digit
        if(key >= '0' && key <= '9')
        {
            Char_LCD(key);             // display digit on LCD
            sum = (sum * 10) + (key - '0'); // convert ASCII to number
            digits++;                  // increase digit count
        }
        // if backspace key pressed
        else if(key == 'B')
        {
            if(digits > 0)      // check if digits exist
            {
                sum = sum / 10; // remove last digit
                digits--;       // decrease digit count
                Cmd_LCD(0x10);  // move cursor left
                Char_LCD(' ');  // erase character
                Cmd_LCD(0x10);  // move cursor left again
            }
        }
        // exit option
        else if(key == 'A')
        {
            return -2;          // return exit code
        }
        // skip option
        else if(key == 'C')
        {
            return -3;          // skip current input
        }
        // enter key pressed
        else if(key == '=')
        {
            if(digits == 0)     // if no number entered
                return -1;      // invalid input
            break;              // finish reading number
        }
    }
    return sum;   // return entered number
}
