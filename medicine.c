#include <lpc214x.h>      // LPC2148 register definitions
#include "medicine.h"     // medicine reminder function declarations
#include "lcd.h"          // LCD driver functions
#include "lcd_defines.h"  // LCD command macros
#include "kpm_defines.h"  // keypad definitions
#include "delay.h"        // delay functions
#include "config.h"       // project configuration macros
#include "types.h"        // custom data types like s32
#include "rtc.h"          // RTC related functions

#define MED_COUNT 3       // maximum number of medicines supporte
// store medicine hours in array
s32 med_hour[MED_COUNT] = {-1,-1,-1};   // initialize all medicine hours to -1 
// store medicine minutes in array
s32 med_min[MED_COUNT]  = {-1,-1,-1};   // initialize medicine minutes to -1

// medicine states
// 0 = waiting
// 1 = alert active
// 2 = handled
int med_state[MED_COUNT] = {0,0,0};     // initialize medicine states

// total medicines configured by user
int med_total = 3;                      // default medicine count

// function to set medicine schedule using keypad
void SetMedicineSchedule(void)
{
    int i,j;                            // loop variables
    // read number of medicines
    do{
        Cmd_LCD(CLEAR_LCD);             // clear LCD display
        Str_LCD("NO OF MED(1-3)");      // ask user to enter number of medicines
        med_total = ReadNum();          // read number from keypad
        if(med_total == -2)             // if A key pressed
            return;                     // exit schedule setup
        if(med_total == -1)             // if '=' pressed without digits
        {
            Cmd_LCD(GOTO_LINE2_POS0);   // move cursor to second line
            Str_LCD("ENTER INPUT");     // show message
            delay_ms(500);              // delay for readability
        }
        else if(med_total < 1 || med_total > 3)   // check valid range
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("INVALID INPUT");   // show invalid message
            delay_ms(500);
        }
    }while(med_total < 1 || med_total > 3);  // repeat until valid value
    
    // configure each medicine schedule
    for(i=0;i<med_total;i++)
    {
        // read medicine hour
        do{
            Cmd_LCD(CLEAR_LCD);        // clear LCD
            Str_LCD("MED");            // display MED text
            Uint_LCD(i+1);             // show medicine number
            Str_LCD(" HR(0-23)");      // prompt hour input
            med_hour[i] = ReadNum();   // read hour from keypad
            if(med_hour[i] == -2)      // exit if A pressed
                return;
            if(med_hour[i] == -1)      // if no digits entered
            {
                Cmd_LCD(GOTO_LINE2_POS0);
                Str_LCD("ENTER INPUT");
                delay_ms(500);
            }
            else if(med_hour[i] < 0 || med_hour[i] > 23) // invalid hour
            {
                Cmd_LCD(GOTO_LINE2_POS0);
                Str_LCD("INVALID INPUT");
                delay_ms(500);
            }
        }while(med_hour[i] < 0 || med_hour[i] > 23); // repeat until valid hour

        // read medicine minute
        do{
            Cmd_LCD(CLEAR_LCD);        // clear LCD
            Str_LCD("MED");
            Uint_LCD(i+1);             // show medicine number
            Str_LCD(" MIN(0-59)");     // prompt minute input
            med_min[i] = ReadNum();    // read minute from keypad
            if(med_min[i] == -2)       // exit
                return;
            if(med_min[i] == -1)       // no digits
            {
                Cmd_LCD(GOTO_LINE2_POS0);
                Str_LCD("ENTER INPUT");
                delay_ms(500);
            }
            else if(med_min[i] < 0 || med_min[i] > 59) // invalid minute
            {
                Cmd_LCD(GOTO_LINE2_POS0);
                Str_LCD("INVALID INPUT");
                delay_ms(500);
            }
            else
            {
                // check duplicate medicine time
                for(j=0;j<i;j++)
                {
                    if(med_hour[i] == med_hour[j] &&
                       med_min[i]  == med_min[j])   // same hour and minute
                    {
                        Cmd_LCD(GOTO_LINE2_POS0);
                        Str_LCD("TIME ALREADY SET"); // show duplicate message
                        delay_ms(800);
                        i--;        // repeat same medicine entry
                        break;
                    }
                }
                if(j != i)          // if duplicate found
                    continue;       // repeat input
            }
        }while(med_min[i] < 0 || med_min[i] > 59 || med_hour[i] == -1);
        med_state[i] = 0;           // set medicine state to waiting
    }

    // disable unused medicine slots
    for(i=med_total;i<MED_COUNT;i++)
    {
        med_hour[i] = -1;           // mark unused
        med_min[i]  = -1;
        med_state[i] = 0;
    }
    Cmd_LCD(CLEAR_LCD);             // clear display
    Str_LCD("MED SET DONE");        // show success message
    delay_ms(800);                  // wait for user to read
}

// Function to check if medicine reminder should activate
int CheckMedicineReminder(int hour,int min,int *med_ack)
{
    int i;                         // loop variable for checking medicines
    for(i=0;i<med_total;i++)       // loop through all configured medicines
    {
        // check if current time matches medicine schedule
        if((med_hour[i] != -1) &&                     // ensure medicine slot is valid
           (hour == med_hour[i] && min == med_min[i]) // check if RTC time matches medicine time
           && med_state[i] == 0)                      // ensure alert not already triggered
        {
            Cmd_LCD(CLEAR_LCD);                       // clear LCD display
            Str_LCD("TAKE MED ");                     // show medicine alert message
            Uint_LCD(i+1);                            // display medicine number (MED 1, MED 2...)
#ifdef SIMULATION
            IOCLR0 = (1<<LED_ALERT);                  // turn ON LED alert in simulation
#else
            IOSET0 = (1<<BUZZER_ALERT);               // start buzzer alert in real hardware
#endif
            med_state[i] = 1;                         // mark medicine alert as active
        }
        // check if alert is currently active
        if(med_state[i] == 1)
        {
            if(*med_ack)                              // check if user acknowledged medicine
            {
#ifdef SIMULATION
                IOSET0 = (1<<LED_ALERT);              // turn OFF LED
#else
                IOCLR0 = (1<<BUZZER_ALERT);           // stop buzzer
#endif
                Cmd_LCD(CLEAR_LCD);                   // clear LCD
                Str_LCD("MED ");                      // display MED text
                Uint_LCD(i+1);                        // show medicine number
                Str_LCD(" TAKEN");                    // display taken confirmation
                delay_ms(800);                        // wait so user can read message
                Cmd_LCD(CLEAR_LCD);                   // clear LCD again
                Str_LCD("NEXT MED AT");               // show next medicine info
                Cmd_LCD(GOTO_LINE2_POS0);             // move cursor to second line
                if(i+1 < med_total)                   // check if next medicine exists
                {
                    Char_LCD(med_hour[i+1]/10 + 48);  // display next medicine hour tens digit
                    Char_LCD(med_hour[i+1]%10 + 48);  // display hour units digit
                    Char_LCD(':');                    // display colon separator
                    Char_LCD(med_min[i+1]/10 + 48);   // display minute tens digit
                    Char_LCD(med_min[i+1]%10 + 48);   // display minute units digit
                }
                else
                {
                    Str_LCD("NO MORE");               // show message if no medicines left
                }
                
                delay_ms(800);                        // delay to show next medicine message
                med_state[i] = 2;                     // mark medicine as handled
                *med_ack = 0;                         // reset acknowledgement flag
                Cmd_LCD(CLEAR_LCD);                   // clear display
                return 0;                             // return 0 (no active alert now)
            }

            // check if medicine time passed without acknowledgement
            if(min != med_min[i])                     // if minute changed
            {
#ifdef SIMULATION
                IOSET0 = (1<<LED_ALERT);              // turn OFF LED
#else
                IOCLR0 = (1<<BUZZER_ALERT);           // stop buzzer
#endif
                Cmd_LCD(CLEAR_LCD);                   // clear LCD
                Str_LCD("MISSED MED ");               // show missed medicine message
                Uint_LCD(i+1);                        // show medicine number
                delay_ms(800);                        // display message for short time
                med_state[i] = 2;                     // mark medicine as handled
                Cmd_LCD(CLEAR_LCD);                   // clear screen
                return 0;                             // return 0 (alert finished)
            }
            return 1;                                 // alert still active
        }
        // reset state when minute changes after handling
        if(min != med_min[i] && med_state[i]==2)
            med_state[i] = 0;                         // reset state so system is ready for next day
    }
    return 0;                                         // return 0 if no medicine alert active
}

// function to display next upcoming medicine time
void DisplayNextMedicine(int hour,int min)
{
    int i;                                            // loop variable
    Cmd_LCD(GOTO_LINE2_POS0);                         // move LCD cursor to second line
    Str_LCD("NEXT MED ");                             // display heading text
    for(i=0;i<MED_COUNT;i++)                          // loop through all medicines
    {
        // check if medicine time is later than current time
        if(med_hour[i] > hour || (med_hour[i]==hour && med_min[i] > min))
        {
            Char_LCD(med_hour[i]/10 + 48);             // display hour tens digit
            Char_LCD(med_hour[i]%10 + 48);             // display hour units digit
            Char_LCD(':');                             // show colon
            Char_LCD(med_min[i]/10 + 48);              // display minute tens digit
            Char_LCD(med_min[i]%10 + 48);              // display minute units digit
            return;                                    // stop after displaying first upcoming medicine
        }
    }
    Str_LCD("NONE");                                   // display NONE if no future medicine scheduled
}
