#include <lpc214x.h>
#include "medicine.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm_defines.h"
#include "delay.h"
#include "config.h"
#include "types.h"
#include "rtc.h"

#define MED_COUNT 3   // maximum medicines supported

// store medicine hours in array
s32 med_hour[MED_COUNT] = {-1,-1,-1};
// store medicine minutes in array
s32 med_min[MED_COUNT]  = {-1,-1,-1};

// medicine states
// 0 waiting
// 1 alert active
// 2 handled
int med_state[MED_COUNT] = {0,0,0};
// total medicines configured
int med_total = 3;

// set medicine schedule using keypad
void SetMedicineSchedule(void)
{
    int i,j;
    // read number of medicines
    do{
        Cmd_LCD(CLEAR_LCD);             // clear lcd screen
        Str_LCD("NO OF MED(1-3)");      // ask medicine count
        med_total = ReadNum();          // read value from keypad
        if(med_total == -2)             // A pressed -> exit
            return;
        if(med_total == -1)             // no digits entered
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("ENTER INPUT");     // show message
            delay_ms(500);
        }
        else if(med_total < 1 || med_total > 3)   // check valid range
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("INVALID INPUT");
            delay_ms(500);
        }
    }while(med_total < 1 || med_total > 3);


    // configure each medicine
    for(i=0;i<med_total;i++)
    {
        // read medicine hour
        do{
            Cmd_LCD(CLEAR_LCD);        // clear screen
            Str_LCD("MED");
            Uint_LCD(i+1);             // show medicine number
            Str_LCD(" HR(0-23)");
            med_hour[i] = ReadNum();   // read hour value
            if(med_hour[i] == -2)      // exit to menu
                return;
            if(med_hour[i] == -1)      // no input entered
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
        }while(med_hour[i] < 0 || med_hour[i] > 23);

        // read medicine minute
        do{
            Cmd_LCD(CLEAR_LCD);        // clear display
            Str_LCD("MED");
            Uint_LCD(i+1);             // display medicine number
            Str_LCD(" MIN(0-59)");
            med_min[i] = ReadNum();    // read minute
            if(med_min[i] == -2)       // exit
                return;
            if(med_min[i] == -1)       // no input
            {
                Cmd_LCD(GOTO_LINE2_POS0);
                Str_LCD("ENTER INPUT");
                delay_ms(500);
            }
            else if(med_min[i] < 0 || med_min[i] > 59)  // invalid minute
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
                       med_min[i]  == med_min[j])  // same hour & minute
                    {
                        Cmd_LCD(GOTO_LINE2_POS0);
                        Str_LCD("TIME ALREADY SET"); // duplicate message
                        delay_ms(800);

                        i--;      // repeat same medicine entry
                        break;
                    }
                }
                if(j != i)
                    continue;
            }
        }while(med_min[i] < 0 || med_min[i] > 59 || med_hour[i] == -1);
        med_state[i] = 0;   // reset state to waiting
    }
    // disable unused medicine slots
    for(i=med_total;i<MED_COUNT;i++)
    {
        med_hour[i] = -1;   // mark as unused
        med_min[i]  = -1;
        med_state[i] = 0;
    }
    Cmd_LCD(CLEAR_LCD);
    Str_LCD("MED SET DONE");   // show success message
    delay_ms(800);
}

// check medicine reminder
int CheckMedicineReminder(int hour,int min,int *med_ack)
{
    int i;
    for(i=0;i<med_total;i++)
    {
        // check if current time matches medicine time
        if((med_hour[i] != -1) &&
           (hour == med_hour[i] && min == med_min[i]) &&
           med_state[i] == 0)
        {
            Cmd_LCD(CLEAR_LCD);        // clear display
            Str_LCD("TAKE MED ");
            Uint_LCD(i+1);             // show medicine number
#ifdef SIMULATION
            IOCLR0 = (1<<LED_ALERT);   // turn ON LED alert
#else
            IOSET0 = (1<<BUZZER_ALERT);// start buzzer alert
#endif
            med_state[i] = 1;          // mark alert active
        }

        // if alert active
        if(med_state[i] == 1)
        {
            if(*med_ack)               // medicine acknowledged
            {
#ifdef SIMULATION
                IOSET0 = (1<<LED_ALERT);  // turn OFF LED
#else
                IOCLR0 = (1<<BUZZER_ALERT);// stop buzzer
#endif
                Cmd_LCD(CLEAR_LCD);
                Str_LCD("MED ");
                Uint_LCD(i+1);
                Str_LCD(" TAKEN");        // show taken message
                delay_ms(800);
                Cmd_LCD(CLEAR_LCD);
                Str_LCD("NEXT MED AT");
                Cmd_LCD(GOTO_LINE2_POS0);
                if(i+1 < med_total)      // check next medicine exists
                {
                    Char_LCD(med_hour[i+1]/10 + 48);
                    Char_LCD(med_hour[i+1]%10 + 48);
                    Char_LCD(':');
                    Char_LCD(med_min[i+1]/10 + 48);
                    Char_LCD(med_min[i+1]%10 + 48);
                }
                else
                {
                    Str_LCD("NO MORE");  // no more medicines
                }
                delay_ms(800);
                med_state[i] = 2;        // mark handled
                *med_ack = 0;            // reset acknowledgement
                Cmd_LCD(CLEAR_LCD);
                return 0;
            }

            // check missed medicine
            if(min != med_min[i])
            {
#ifdef SIMULATION
                IOSET0 = (1<<LED_ALERT);
#else
                IOCLR0 = (1<<BUZZER_ALERT);
#endif
                Cmd_LCD(CLEAR_LCD);
                Str_LCD("MISSED MED ");
                Uint_LCD(i+1);         // show missed medicine
                delay_ms(800);
                med_state[i] = 2;      // mark handled
                Cmd_LCD(CLEAR_LCD);
                return 0;
            }
            return 1;
        }
        // reset state when minute changes
        if(min != med_min[i] && med_state[i]==2)
            med_state[i] = 0;
    }
    return 0;
}

// display next medicine time
void DisplayNextMedicine(int hour,int min)
{
    int i;
    Cmd_LCD(GOTO_LINE2_POS0);
    Str_LCD("NEXT MED ");
    for(i=0;i<MED_COUNT;i++)
    {
        if(med_hour[i] > hour || (med_hour[i]==hour && med_min[i] > min))
        {
            Char_LCD(med_hour[i]/10 + 48);
            Char_LCD(med_hour[i]%10 + 48);
            Char_LCD(':');
            Char_LCD(med_min[i]/10 + 48);
            Char_LCD(med_min[i]%10 + 48);
            return;
        }
    }
    Str_LCD("NONE");   // no next medicine
}

