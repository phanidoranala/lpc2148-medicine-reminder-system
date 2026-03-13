#include <lpc214x.h>
#include "rtc.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm_defines.h"
#include "delay.h"
#include "medicine.h"
#include "config.h"
#include "interrupt.h"
#include "menu.h"


// RTC variables used across modules
s32 hour,min,sec;
s32 date,month,year;
s32 day;

// flags set by interrupt service routines
int menu_flag = 0;   // set when SW1 is pressed
int med_ack = 0;     // set when SW2 acknowledges medicine
int last_min;
int main()
{
    RTC_Init();          // start RTC
    Init_LCD();          // initialize LCD display
    initKPM();           // initialize keypad
    Interrupt_Init();    // configure SW1 and SW2 interrupts
#ifndef SIMULATION
		IODIR0 |= (1<<BUZZER_ALERT);
    /*IODIR0 |= (1<<LED_ALERT);     // configure LED pin as output
    IOSET0 = (1<<LED_ALERT);*/      // LED OFF (active low)
#else
   // IODIR0 |= (1<<BUZZER_ALERT);  // configure buzzer pin as output
	  IODIR0 |= (1<<LED_ALERT);     // configure LED pin as output
    IOSET0 = (1<<LED_ALERT);      // LED OFF (active low)
#endif
    while(1)
    {
        // check if SW1 requested menu
        if(menu_flag)
        {
            menu_flag = 0;          // clear request

            VICIntEnClr = (1<<14);  // disable SW1 interrupt temporarily

            Menu_Setup();           // open configuration menu

            Cmd_LCD(CLEAR_LCD);     // clear screen after menu exit

            EXTINT = (1<<0);        // clear interrupt flag

            VICIntEnable = (1<<14); // enable SW1 interrupt again
        }
        // read current RTC time
        GetRTCTimeInfo(&hour,&min,&sec);
        // read current weekday
        GetRTCDay(&day);
        // read current date
        GetRTCDateInfo(&date,&month,&year);
        // check if medicine reminder should run
       if(CheckMedicineReminder(hour,min,&med_ack)==0)
				{
						DisplayRTCTime(hour,min,sec);
						DisplayRTCDay(day);

						// show NEXT MED only when minute changes
						if(min != last_min)
						{
								last_min = min;

								DisplayNextMedicine(hour,min);   // show next medicine
								delay_ms(1000); 							// show for 1 second
								Cmd_LCD(GOTO_LINE2_POS0+11);
								Str_LCD("     ");
						}

						DisplayRTCDate(date,month,year);     // normal display
				}
        // return 1 means medicine alert active
        // so RTC display is skipped
    }
}

