#ifndef KPM_DEFINES_H           // check if this header file is already included
#define KPM_DEFINES_H           // define macro to prevent multiple inclusion

// Keypad row pin definitions (connected to Port1 pins)
#define R0 16                   // Row 0 connected to P1.16
#define R1 17                   // Row 1 connected to P1.17
#define R2 18                   // Row 2 connected to P1.18
#define R3 19                   // Row 3 connected to P1.19
// Keypad column pin definitions (connected to Port1 pins)
#define C0 20                   // Column 0 connected to P1.20
#define C1 21                   // Column 1 connected to P1.21
#define C2 22                   // Column 2 connected to P1.22
#define C3 23                   // Column 3 connected to P1.23

// function to initialize keypad hardware
void initKPM(void);             // configures row pins as output and column pins as input
// function to check if any key is pressed
unsigned int colscan(void);     // scans column pins and detects key press
// function to detect which row the key belongs to
unsigned int rowcheck(void);    // identifies row number where key is pressed
// function to detect which column the key belongs to
unsigned int colcheck(void);    // identifies column number where key is pressed
// function to scan keypad and return pressed key value
unsigned int keyscan(void);     // combines row and column detection
// function to read numeric input from keypad
int ReadNum(void);              // reads digits and converts them into integer value
#endif                          // end of header guard
