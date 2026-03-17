#ifndef RTC_H                 // check if RTC_H is already defined
#define RTC_H                 // define RTC_H to prevent multiple inclusion

#include "types.h"            // include custom data types like s32, u32

// oscillator frequency of LPC2148
#define FOSC 12000000        // main crystal frequency = 12 MHz
// CPU clock frequency
#define CCLK (5 * FOSC)      // CPU clock = 5 × oscillator frequency
// peripheral clock frequency
#define PCLK (CCLK / 4)      // peripheral clock = CPU clock / 4
// RTC prescaler values used to generate 1 second tick
#define PREINT_VAL (s32)((PCLK / 32768) - 1)        // integer prescaler part
#define PREFRAC_VAL (PCLK -((PREINT_VAL + 1) * 32768)) // fractional prescaler part

// RTC control register bits
#define RTC_ENABLE (1<<0)     // bit to enable RTC
#define RTC_RESET  (1<<1)     // bit to reset RTC counters
#define RTC_CLKSRC (1<<4)     // bit to select RTC clock source
// weekday definitions
#define SUN 0                 // Sunday = 0
#define MON 1                 // Monday = 1
#define TUE 2                 // Tuesday = 2
#define WED 3                 // Wednesday = 3
#define THU 4                 // Thursday = 4
#define FRI 5                 // Friday = 5
#define SAT 6                 // Saturday = 6

// global RTC variables declared in rtc.c but used in other files
extern s32 hour,min,sec;     // current time variables
extern s32 date,month,year;  // current date variables
extern s32 day;              // current weekday
extern s32 key;              // keypad key variable

// function to initialize RTC hardware
void RTC_Init(void);
// functions to read and display time
void GetRTCTimeInfo(s32*,s32*,s32*);   // read hour, minute, second from RTC
void DisplayRTCTime(u32,u32,u32);      // display time on LCD
// functions to read and display date
void GetRTCDateInfo(s32*,s32*,s32*);   // read date, month, year
void DisplayRTCDate(u32,u32,u32);      // display date on LCD
// functions to read and display weekday
void GetRTCDay(s32*);                  // read weekday from RTC
void DisplayRTCDay(u32);               // display weekday on LCD
// functions to set RTC values
void SetRTCTimeInfo(u32,u32,u32);      // write hour, minute, second to RTC
void SetRTCDateInfo(u32,u32,u32);      // write date, month, year to RTC
void SetRTCDay(u32);                   // write weekday to RTC
// function to set initial RTC values
void RTC_SetInitial(void);             // set default time and date
// repeated declarations (same functions declared again)
void SetRTCTimeInfo(u32,u32,u32);      // set RTC time
void SetRTCDateInfo(u32,u32,u32);      // set RTC date
void SetRTCDay(u32);                   // set RTC weekday
// repeated initial setup declaration
void RTC_SetInitial(void);             // initialize RTC values
// functions to update RTC values through keypad input
void RTC_UpdateTime(void);             // update hour, minute, second
void RTC_UpdateDate(void);             // update date, month, year
void RTC_UpdateDay(void);              // update weekday
// function to display invalid input scrolling message
void InvalidMsgScroll(void);


#endif                                 // end of header guard

