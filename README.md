# LPC2148 Medicine Reminder System
Embedded C project using LPC2148 to create a user configurable medicine reminder system with RTC, LCD, keypad, and buzzer alerts.
## Overview
This project implements a **user configurable medicine reminder system** using the **LPC2148 ARM7 microcontroller**.  
The system allows users to set medicine timings and receive alerts when it is time to take medicine.

## Features
- Real-time clock (RTC) based time monitoring
- LCD display for time, date, and reminders
- User configurable medicine schedule
- Keypad based input interface
- Buzzer alert for medicine reminder
- Interrupt based user interaction
- Menu timeout protection

## Hardware Components
- LPC2148 Microcontroller
- 16x2 LCD Display
- 4x4 Matrix Keypad
- Buzzer / LED alert
- Switches (EINT0 and EINT1)

## Software Tools
- Embedded C
- Keil uVision
- Flash Magic

## System Operation
1. System initializes RTC, LCD, keypad, and interrupts.
2. Current time and date are displayed on LCD.
3. User can open the menu using **Switch 1 (EINT0)**.
4. Through the menu the user can:
   - Set RTC time
   - Set date and day
   - Configure medicine schedule
5. When the current RTC time matches a stored medicine time:
   - LCD displays **Take Medicine**
   - Buzzer alert is activated
6. User presses **Switch 2 (EINT1)** to acknowledge the reminder.

## Project Structure
main_project.c
rtc_.c
lcd.c
kpm_prototype.c
medicine.c
menu.c
inter_mp.c
delay.c


types.h
medicine.h
config.h
interrupt.h
menu.h
rtc.h
lcd.h
kpm_defines.h
lcd_defines.h
delay.h

## Author
Phani Doranala
Embedded Systems Project
