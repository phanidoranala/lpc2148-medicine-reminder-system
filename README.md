# LPC2148 Medicine Reminder System

Embedded C project using the LPC2148 ARM7 microcontroller to implement a user configurable medicine reminder system.
The system allows users to set medicine timings and provides alerts using LCD messages and buzzer notifications.

---

## Block Diagram

![Block Diagram](images/blockdiagram.png)

---

## Overview

This project implements a real-time medicine reminder system using the LPC2148 microcontroller.
The system continuously monitors time using the Real Time Clock (RTC) and compares it with the user-configured medicine schedule.

When the current time matches the stored medicine time, the system generates an alert using a buzzer or LED and displays a reminder message on the LCD display.

Users can configure the system through a 4x4 matrix keypad interface. The system also supports interrupt-based controls using external switches.

---

## Working Principle

The system works based on **continuous real-time comparison** between RTC time and stored medicine schedules.

1. RTC continuously updates time
2. User sets medicine timings
3. System compares both values
4. If match occurs → Alert is triggered
5. User acknowledges → System resets

---

## RTC (Real Time Clock)

The LPC2148 has an inbuilt RTC module:

* Maintains time using 32.768 kHz clock
* Stores time in registers (SEC, MIN, HOUR, etc.)
* Works independently of CPU

---

## LCD Interfacing

A 16x2 LCD is used to display:

* Time & Date
* Menu
* Alerts

Operates in **4-bit mode** to save pins.

---

## Keypad Interfacing

4x4 matrix keypad works using:

* Row-column scanning
* Detects pressed key by intersection

Used for:

* Menu selection
* Time input

---

## Interrupts Used

| Interrupt | Function          |
| --------- | ----------------- |
| EINT0     | Open menu         |
| EINT1     | Acknowledge alert |

Interrupts provide **fast response without polling**.

---

# Proteus Simulation

## RTC Time Display

![RTC](images/simulation04.png)

Displays current time and date from RTC.

---

## Menu Display

![Menu](images/simulation01.png)

Appears when **SW1 (EINT0)** is pressed.

---

## Invalid Input Handling

![Invalid](images/simulation02.png)

Displays error for wrong input.

---

## Next Medicine Display

![Next](images/simulation07.png)

Shows next scheduled medicine.

---

## Medicine Alert

![Alert](images/simulation05.png)

* LCD shows **TAKE MED X**
* Buzzer/LED activated

---

## Medicine Acknowledgement

![Taken](images/simulation06.png)

* Alert stops
* Displays **MED X TAKEN**

---

## System Operation

### Initialization

* RTC, LCD, Keypad initialized

### Menu Access

* SW1 → Open menu

### Scheduling

* User enters time

### Monitoring

* Continuous RTC comparison

### Alert

* Triggered on match

### Acknowledgement

* SW2 → Stops alert

---

## Advantages

* Accurate real-time monitoring
* Low power consumption
* User-friendly interface
* Interrupt-based fast response

---

## Applications

* Medicine reminder systems
* Elderly care
* Smart healthcare devices
* Embedded alert systems

---

## Author

**Phani Doranala**
Embedded Systems Project
LPC2148 Medicine Reminder System
