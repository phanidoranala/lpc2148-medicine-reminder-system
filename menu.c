#include "lcd.h"            // LCD driver functions
#include "lcd_defines.h"    // LCD command definitions
#include "kpm_defines.h"    // keypad definitions
#include "rtc.h"            // RTC related functions
#include "medicine.h"       // medicine schedule functions
#include "delay.h"          // delay functions


// function to display menu and handle user selection
// menu exits when user selects exit or when timeout occurs
void Menu_Setup(void)
{
    char key;                 // variable to store pressed keypad key
    int start_sec,cur_sec;    // variables used to track menu start and current seconds
    int diff;                 // variable used to calculate time difference

    // read current RTC time when menu is opened
    GetRTCTimeInfo(&hour,&min,&sec);
    start_sec = sec;          // store the second value when menu started (for timeout calculation)
    Cmd_LCD(CLEAR_LCD);       // clear LCD before displaying menu

    while(1)                  // infinite loop to keep menu active
    {
        // display first line of menu
        Cmd_LCD(GOTO_LINE1_POS0);   // move LCD cursor to line1 position0
        Str_LCD("1.TIME 2.DAY");    // show menu options for time and date/day
        // display second line of menu
        Cmd_LCD(GOTO_LINE2_POS0);   // move cursor to second line
        Str_LCD("3.MED 4.EXIT  ");  // show medicine setup and exit option
        // check if keypad key is pressed
        if(colscan()==0)            // colscan returns 0 when a key press is detected
        {
            key = keyscan();        // read which key was pressed
            if(key=='1')            // if user presses key '1'
            {
                RTC_UpdateTime();   // open time configuration screen
                // reset timeout counter after returning from submenu
                GetRTCTimeInfo(&hour,&min,&sec);
                start_sec = sec;    // restart timeout timer
                continue;           // return to menu display
            }

            else if(key=='2')       // if user presses key '2'
            {
                RTC_UpdateDate();   // open date configuration screen
                RTC_UpdateDay();    // open weekday configuration screen
                // reset timeout counter
                GetRTCTimeInfo(&hour,&min,&sec);
                start_sec = sec;    // restart timer
                continue;           // return to menu
            }
            else if(key=='3')       // if user presses key '3'
            {
                SetMedicineSchedule();   // open medicine schedule setup
                // restart timeout counter
                GetRTCTimeInfo(&hour,&min,&sec);
                start_sec = sec;
                continue;           // return to menu
            }
            else if(key=='4')       // if user presses key '4'
            {
                break;              // exit menu loop
            }
        }
        // read current RTC time again
        GetRTCTimeInfo(&hour,&min,&sec);
        cur_sec = sec;              // store current second value
        /* calculate difference between menu start time and current time
           also handle second rollover from 59 -> 0 */
        if(cur_sec >= start_sec)
            diff = cur_sec - start_sec;        // normal subtraction if no rollover
        else
            diff = 60 - start_sec + cur_sec;   // handle rollover case

        // check if menu is idle for 30 seconds
        if(diff >= 30)
        {
            Cmd_LCD(CLEAR_LCD);     // clear LCD
            Str_LCD("MENU TIMEOUT"); // display timeout message
            delay_ms(500);          // wait for half second
            break;                  // exit menu loop
        }
    }
}

