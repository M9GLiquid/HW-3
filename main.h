#ifndef MAIN_H
#define MAIN_H

/* SPECIAL
0000 0000
Bit     Type
0[1]     TC Rising Edge (TC0)
1[2]     ADC Light 1 (LS1)
2[3]     ADC Light 2 (LS2)
3[4]
4[5]
5[6]
6[7]
7[8]
*/
extern unsigned char booleanFlagSet; // Variable declared
// Handlers
void SysTick_Handler(void);

// Functions
void config(void);
void Delay(int Value);

#endif