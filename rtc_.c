#include <lpc214x.h>        // Header file for LPC2148 register definitions
#include "rtc.h"            // RTC function declarations
#include "lcd.h"            // LCD display functions
#include "lcd_defines.h"    // LCD command definitions
#include "kpm_defines.h"    // Keypad definitions
#include "delay.h"          // Delay functions

/* weekday names for display */
s8 week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"}; // array storing weekday names
// initialize RTC module
// reset RTC and configure prescaler
void RTC_Init(void)
{
    CCR = RTC_RESET;      // reset RTC counters and control register
#ifndef _LPC2148          // check if controller is not LPC2148
    PREINT = PREINT_VAL;  // load integer prescaler value
    PREFRAC = PREFRAC_VAL;// load fractional prescaler value
    CCR = RTC_ENABLE;     // enable RTC clock
#else
    CCR = RTC_ENABLE | RTC_CLKSRC;   // enable RTC and select external clock source
#endif
}
// read current time from RTC registers
void GetRTCTimeInfo(s32 *h,s32 *m,s32 *s)
{
    *h = HOUR;   // read hour register value
    *m = MIN;    // read minute register value
    *s = SEC;    // read second register value
}
// display time on LCD in HH:MM:SS format
void DisplayRTCTime(u32 h,u32 m,u32 s)
{
    Cmd_LCD(GOTO_LINE1_POS0);   // move LCD cursor to first line position 0

    Char_LCD(h/10 + 48);        // convert hour tens digit to ASCII and display
    Char_LCD(h%10 + 48);        // convert hour units digit to ASCII and display
    Char_LCD(':');              // display colon separator
    Char_LCD(m/10 + 48);        // display minute tens digit
    Char_LCD(m%10 + 48);        // display minute units digit
    Char_LCD(':');              // display colon separator
    Char_LCD(s/10 + 48);        // display seconds tens digit
    Char_LCD(s%10 + 48);        // display seconds units digit
}
// write time values into RTC registers
void SetRTCTimeInfo(u32 h,u32 m,u32 s)
{
    HOUR = h;   // write hour value into RTC HOUR register
    MIN  = m;   // write minute value into RTC MIN register
    SEC  = s;   // write second value into RTC SEC register
}
// read date from RTC registers
void GetRTCDateInfo(s32 *d,s32 *m,s32 *y)
{
    *d = DOM;       // read day of month register
    *m = MONTH;     // read month register
    *y = YEAR;      // read year register
}
// display date on LCD
void DisplayRTCDate(u32 d,u32 m,u32 y)
{
    Cmd_LCD(GOTO_LINE2_POS0);   // move LCD cursor to second line
    Char_LCD(d/10 + 48);        // display date tens digit
    Char_LCD(d%10 + 48);        // display date units digit
    Char_LCD('/');              // display slash separator
    Char_LCD(m/10 + 48);        // display month tens digit
    Char_LCD(m%10 + 48);        // display month units digit
    Char_LCD('/');              // display slash separator
    Char_LCD((y/1000)%10 + 48); // display year thousands digit
    Char_LCD((y/100)%10 + 48);  // display year hundreds digit
    Char_LCD((y/10)%10 + 48);   // display year tens digit
    Char_LCD((y%10) + 48);      // display year units digit

    Char_LCD(' ');              // print space for formatting
}
// write date values to RTC registers
void SetRTCDateInfo(u32 d,u32 m,u32 y)
{
    DOM   = d;      // update day of month register
    MONTH = m;      // update month register
    YEAR  = y;      // update year register
}
// read weekday register
void GetRTCDay(s32 *dow)
{
    *dow = DOW;   // read Day Of Week register
}
// display weekday next to time
void DisplayRTCDay(u32 dow)
{
    Cmd_LCD(GOTO_LINE1_POS0 + 8);   // move cursor to position after time display
    Char_LCD(' ');                  // display space
    Str_LCD(week[dow]);             // print weekday string from array
}
// write weekday value to RTC
void SetRTCDay(u32 dow)
{
    DOW = dow;   // update Day Of Week register
}
// Function to update RTC time using keypad input
void RTC_UpdateTime(void)
{
    int val;                       // variable used to store the number entered from keypad
    /* hour input */
    do{                            // start loop that continues until valid hour is entered
        Cmd_LCD(CLEAR_LCD);        // clear the LCD display
        Str_LCD("HOUR(0-23):");    // show message asking user to enter hour (0–23)
        val = ReadNum();           // read number entered using keypad
        if(val == -2)              // check if user pressed exit key (A)
            return;                // exit the function and go back to menu
        if(val == -3)              // check if user pressed skip key (C)
            break;                 // skip hour entry and go to minute input
        if(val == -1)              // check if user pressed '=' without entering digits
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor to second line of LCD
            Str_LCD("ENTER INPUT");   // display message asking user to enter input
            delay_ms(500);            // delay so user can read the message
        }
        else if(val < 0 || val > 23)  // check if entered hour is outside valid range
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor to second line
            Str_LCD("INVALID INPUT"); // display invalid input message
            delay_ms(500);            // small delay for readability
        }
        else                          // if entered hour is valid
        {
            hour = val;               // store entered hour into variable 'hour'
            break;                    // exit loop and move to minute input
        }
    }while(1);                        // repeat until valid hour is entered

    /* minute input */
    do{                               // start loop for minute entry
        Cmd_LCD(CLEAR_LCD);           // clear LCD display
        Str_LCD("MIN(0-59):");        // display prompt asking user to enter minute
        val = ReadNum();              // read number from keypad

        if(val == -2)                 // if exit key pressed
            return;                   // exit function
        if(val == -3)                 // if skip key pressed
            break;                    // skip minute entry
        if(val == -1)                 // if no digits entered
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor to second line
            Str_LCD("ENTER INPUT");   // show error message
            delay_ms(500);            // delay for message visibility
        }
        else if(val < 0 || val > 59)  // check valid minute range
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor
            Str_LCD("INVALID INPUT"); // display invalid message
            delay_ms(500);            // delay
        }
        else                          // valid minute entered
        {
            min = val;                // store minute value in variable 'min'
            break;                    // exit loop
        }
    }while(1);                        // repeat until valid minute is entered

    /* second input */
    do{                               // start loop for seconds input
        Cmd_LCD(CLEAR_LCD);           // clear LCD screen
        Str_LCD("SEC(0-59):");        // show message asking user to enter seconds
        val = ReadNum();              // read number from keypad

        if(val == -2)                 // if exit key pressed
            return;                   // exit function
        if(val == -3)                 // if skip key pressed
            break;                    // skip seconds entry
        if(val == -1)                 // if no digits entered
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor to second line
            Str_LCD("ENTER INPUT");   // show input error message
            delay_ms(500);            // delay for readability
        }
        else if(val < 0 || val > 59)  // check seconds range
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor
            Str_LCD("INVALID INPUT"); // display invalid input message
            delay_ms(500);            // delay
		}
        else                          // valid seconds entered
        {
            sec = val;                // store seconds value
            break;                    // exit loop
        }
    }while(1);                        // repeat until valid seconds entered
    SetRTCTimeInfo(hour,min,sec);     // write updated hour, minute, second values to RTC registers
}

// update RTC date using keypad
// Function to update RTC date using keypad input
void RTC_UpdateDate(void)
{
    int val;                          // variable to store the number entered from keypad
    /* date input */
    do{                               // start loop until valid date is entered
        Cmd_LCD(CLEAR_LCD);           // clear the LCD display
        Str_LCD("DATE(1-31):");       // show prompt asking user to enter date (1 to 31)
        val = ReadNum();              // read number entered through keypad
        if(val == -2)                 // if user presses exit key (A)
            return;                   // exit this function and return to menu
        if(val == -3)                 // if user presses skip key (C)
            break;                    // skip date entry and move to next step
        if(val == -1)                 // if user presses '=' without entering digits
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move LCD cursor to second line
            Str_LCD("ENTER INPUT");   // display message asking user to enter input
            delay_ms(500);            // delay so user can read message
        }
        else if(val < 1 || val > 31)  // check if entered date is outside valid range
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor to second line
            Str_LCD("INVALID INPUT"); // display invalid input message
            delay_ms(500);            // delay for readability
        }
        else
        {
            date = val;               // store valid date in variable 'date'
            break;                    // exit loop once valid date is entered
        }
    }while(1);                        // repeat until valid input is given

    /* month input */
    do{                               // start loop for month input
        Cmd_LCD(CLEAR_LCD);           // clear LCD screen
        Str_LCD("MONTH(1-12):");      // display month prompt
        val = ReadNum();              // read month from keypad
        if(val == -2)                 // exit condition
            return;                   // exit function
        if(val == -3)                 // skip condition
            break;                    // skip month input
        if(val == -1)                 // if no digits entered
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor to second line
            Str_LCD("ENTER INPUT");   // display input required message
            delay_ms(500);            // delay to show message
        }
        else if(val < 1 || val > 12)  // check if month is outside valid range
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor
            Str_LCD("INVALID INPUT"); // show invalid input message
            delay_ms(500);            // delay for readability
        }
        else
        {
            month = val;              // store valid month value
            break;                    // exit loop
        }
    }while(1);                        // repeat until valid month is entered

    /* year input */
    do{                               // start loop for year input
        Cmd_LCD(CLEAR_LCD);           // clear LCD display
        Str_LCD("YEAR:");             // prompt user to enter year
        val = ReadNum();              // read year from keypad
        if(val == -2)                 // exit key pressed
            return;                   // return to menu
        if(val == -3)                 // skip key pressed
            break;                    // skip year entry
        if(val == -1)                 // no digits entered
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor to second line
            Str_LCD("ENTER INPUT");   // show message
            delay_ms(500);            // wait for user to read
        }
        else if(val < 2000 || val > 2099)  // check valid year range
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor
            Str_LCD("INVALID INPUT"); // display error
            delay_ms(500);            // delay
        }
        else
        {
            year = val;               // store valid year
            break;                    // exit loop
        }
    }while(1);                        // repeat until valid year entered
    SetRTCDateInfo(date,month,year);  // update RTC registers with new date
}

// Function to update weekday in RTC
void RTC_UpdateDay(void)
{
    int val;                          // variable to store weekday entered
    do{                               // loop until valid weekday entered
        Cmd_LCD(CLEAR_LCD);           // clear LCD screen
        Str_LCD("DAY(0-6): S0 M1");   // show weekday mapping (Sunday=0 Monday=1)
        Cmd_LCD(GOTO_LINE2_POS0);     // move cursor to second line
        Str_LCD("T2 W3 T4 F5 S6");    // display remaining weekdays
        val = ReadNum();              // read number from keypad
        if(val == -2)                 // exit key pressed
            return;                   // exit function
        if(val == -3)                 // skip key pressed
            break;                    // skip weekday entry
        if(val == -1)                 // no digits entered
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor
            Str_LCD("ENTER INPUT");   // display message
            delay_ms(500);            // wait
        }
        else if(val < 0 || val > 6)   // check if weekday value is invalid
        {
            Cmd_LCD(GOTO_LINE2_POS0); // move cursor
            Str_LCD("INVALID INPUT"); // show error message
            delay_ms(500);            // delay
        }
        else
        {
            day = val;                // store valid weekday
            break;                    // exit loop
        }
    }while(1);                        // repeat until valid weekday entered
    SetRTCDay(day);                   // update RTC Day-of-Week register
}

