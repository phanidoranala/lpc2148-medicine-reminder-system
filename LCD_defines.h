// LCD_defines.h
#ifndef LCD_DEFINES_H          // check if this header file is already included
#define LCD_DEFINES_H          // define the macro to prevent multiple inclusion
// LCD pin connections with LPC2148 GPIO pins
#define LCD_DATA 8             // LCD data pins (D0–D7) connected starting from P0.8
#define RS 16                  // RS (Register Select) pin connected to P0.16
#define RW 17                  // RW (Read/Write) pin connected to P0.17
#define EN 18                  // EN (Enable) pin connected to P0.18
// ---------------- LCD Commands ----------------
// command to clear the entire LCD display
#define CLEAR_LCD 0X01
// command to return cursor to home position (line1, position0)
#define RET_CUR_HOME 0x02
// command to turn LCD display OFF
#define DISP_OFF 0x08
// command to turn LCD display ON without cursor
#define DISP_ON 0x0C
// display ON with cursor visible
#define DISP_ON_CUR_ON 0x0E
// display ON with blinking cursor
#define DISP_ON_CUR_BLINK 0x0F
// LCD operating modes
#define MODE_8BIT_1LINE 0X30   // configure LCD in 8-bit mode with 1 display line
#define MODE_8BIT_2LINE 0X38   // configure LCD in 8-bit mode with 2 display lines
#define MODE_4BIT_1LINE 0X20   // configure LCD in 4-bit mode with 1 display line
#define MODE_4BIT_2LINE 0X28   // configure LCD in 4-bit mode with 2 display lines
// LCD cursor positions
#define GOTO_LINE1_POS0 0X80   // move cursor to line 1 position 0
#define GOTO_LINE2_POS0 0XC0   // move cursor to line 2 position 0
#define GOTO_LINE3_POS0 0X94   // move cursor to line 3 position 0
#define GOTO_LINE4_POS0 0XD4   // move cursor to line 4 position 0
// display shift commands
#define SHIFT_CUR_RIGHT 0X06   // move cursor to the right after each character
#define SHIFT_DISP_LEFT 0x10   // shift entire display to the left
#define SHIFT_DISP_RIGHT 0X14  // shift entire display to the right
// command to access CGRAM (used to create custom characters)
#define GOTO_CGRAM 0x40
#endif                        // end of header guard
