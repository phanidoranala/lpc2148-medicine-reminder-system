#ifndef LCD_H                 // check if LCD_H is already defined
#define LCD_H                 // define LCD_H to avoid multiple inclusion
// function to write raw data or command to LCD
void Write_LCD(char data);   // sends data to LCD data pins and generates enable pulse
// function to send command instruction to LCD
void Cmd_LCD(char cmd);      // used to control LCD operations like clear, cursor move
// function to display a single character on LCD
void Char_LCD(char ch);      // sends one ASCII character to LCD
// function to initialize LCD module
void Init_LCD(void);         // configures LCD pins and sets display mode
// function to display a string on LCD
void Str_LCD(char* p);       // prints characters one by one until NULL terminator
// function to display unsigned integer on LCD
void Uint_LCD(unsigned int n);   // converts integer to ASCII and displays
// function to display signed integer on LCD
void Sint_LCD(int n);        // handles negative numbers before printing
// function to display floating point number on LCD
void Float_LCD(float num, char nDP); // prints float value with given decimal places
// function to display hexadecimal number
void Hex_LCD(unsigned int n);  // converts number into hexadecimal format
// function to display binary number
void Bin_LCD(unsigned int n, char nBB); // prints binary representation of number
// function to build custom characters in LCD CGRAM
void Build_CGRAM(char* p, char nb); // used to create user-defined characters
#endif                         // end of header guard




