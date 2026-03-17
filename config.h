#ifndef CONFIG_H              // check if CONFIG_H is already defined
#define CONFIG_H              // define CONFIG_H to avoid multiple inclusion
/* comment this for hardware */
#define SIMULATION            // used to run the program in simulation mode
/* interrupt pin select */
// configure pin P0.1 as external interrupt EINT0
#define SW1_PINSEL (3<<2)     // bits for PINSEL0 to enable EINT0 function on P0.1
// configure pin P0.3 as external interrupt EINT1
#define SW2_PINSEL (3<<6)     // bits for PINSEL0 to enable EINT1 function on P0.3
// VIC (Vectored Interrupt Controller) channel numbers
#define EINT0_VIC_CHNO 14     // interrupt channel number for EINT0
#define EINT1_VIC_CHNO 15     // interrupt channel number for EINT1
#ifndef SIMULATION            // if running on real hardware
#define BUZZER_ALERT 23       // buzzer connected to pin P0.23
#else                         // if running in simulation mode
#define LED_ALERT 23          // LED connected to pin P0.23 (used instead of buzzer)
#endif
#endif                        // end of header guard

