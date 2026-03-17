#ifndef MEDICINE_H            // check if MEDICINE_H is already defined
#define MEDICINE_H            // define MEDICINE_H to avoid multiple inclusion

// function to configure medicine schedule using keypad
void SetMedicineSchedule(void);   // allows user to enter medicine times
// function to check if medicine reminder should trigger
// hour ? current RTC hour
// min  ? current RTC minute
// med_ack ? pointer used to detect acknowledgement (SW2 press)
int CheckMedicineReminder(int hour,int min,int *med_ack);
// function to display next upcoming medicine time
void DisplayNextMedicine(int hour,int min); // shows next medicine after current time

#endif                        // end of header guard
