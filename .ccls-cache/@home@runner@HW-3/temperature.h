#ifndef TEMPERATURE_H
#define TEMPERATURE_H

//Global variables
extern unsigned int time; // Variable declared



//Functions
void Init_Temp(void);
void initPulse(void);
void Read_Temp(void);
void Temp_Mesurement(void);

//Handlers
void TC0_Handler(void);

#endif