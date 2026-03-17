#include <lpc214x.h>      // LPC2148 microcontroller register definitions
#include "rtc.h"          // RTC related functions
#include "lcd.h"          // LCD driver functions
#include "lcd_defines.h"  // LCD command macros
#include "kpm_defines.h"  // keypad definitions
#include "delay.h"        // delay functions
#include "medicine.h"     // medicine reminder logic
#include "config.h"       // project configuration macros
#include "interrupt.h"    // interrupt initialization and ISR declarations
#include "menu.h"         // menu system functions

// RTC variables used across modules
s32 hour,min,sec;        // variables to store current hour, minute, second
s32 date,month,year;     // variables to store current date, month, year
s32 day;                 // variable to store weekday value (0-6)
// flags set by interrupt service routines
int menu_flag = 0;       // set to 1 when SW1 interrupt requests menu
int med_ack = 0;         // set to 1 when SW2 acknowledges medicine alert
int last_min;            // stores previous minute value to detect minute change

int main()
{
    RTC_Init();          // initialize RTC module and start time counting
    Init_LCD();          // initialize LCD hardware and configure display
    initKPM();           // initialize keypad pins for input
    Interrupt_Init();    // configure external interrupts for SW1 and SW2

#ifndef SIMULATION       // check if running on real hardware
		IODIR0 |= (1<<BUZZER_ALERT);   // configure buzzer pin as output
#else                    // if running in simulation mode
	  IODIR0 |= (1<<LED_ALERT);     // configure LED pin as output
    IOSET0 = (1<<LED_ALERT);       // turn LED OFF initially (active low)
#endif

    while(1)              // infinite loop for continuous system operation
    {
        // check if SW1 requested menu
        if(menu_flag)     // if interrupt set menu_flag
        {
            menu_flag = 0;          // clear the flag
            VICIntEnClr = (1<<14);  // temporarily disable SW1 interrupt
            Menu_Setup();           // open configuration menu (time, medicine etc.)
            Cmd_LCD(CLEAR_LCD);     // clear LCD after exiting menu
            EXTINT = (1<<0);        // clear external interrupt flag
            VICIntEnable = (1<<14); // enable SW1 interrupt again
        }

        // read current RTC time
        GetRTCTimeInfo(&hour,&min,&sec);   // get hour, minute, second from RTC registers
        // read current weekday
        GetRTCDay(&day);                   // get day of week from RTC
        // read current date
        GetRTCDateInfo(&date,&month,&year); // get date, month, year from RTC
        // check if medicine reminder should run
        if(CheckMedicineReminder(hour,min,&med_ack)==0)  // if no medicine alert active
				{
						DisplayRTCTime(hour,min,sec);   // display current time on LCD
						DisplayRTCDay(day);              // display weekday next to time
						// show NEXT MED only when minute changes
						if(min != last_min)             // check if minute value changed
						{
							    last_min = min;           // update last recorded minute
								DisplayNextMedicine(hour,min);   // show next medicine schedule
								delay_ms(1000);                  // display message for 1 second
								Cmd_LCD(GOTO_LINE2_POS0+11);     // move cursor to specific position
								Str_LCD("     ");                // clear displayed medicine text
						}
						DisplayRTCDate(date,month,year);     // display date on LCD
				}
        // return value 1 means medicine alert is active
        // so normal RTC display will be skipped
    }
}
