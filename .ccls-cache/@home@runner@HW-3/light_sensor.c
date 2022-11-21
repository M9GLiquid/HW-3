#include "main.h"
#include "display.h"
#include "at91sam3x8.h"
#include "light_sensor.h"
#include "system_sam3x.h"
#include <stdio.h>


void Init_LightSensor(){
  *AT91C_PMC_PCER = (1 << 11); // Enable PMC_PIOA
  *AT91C_PMC_PCER1 = (1 << 5); // Start clock for ADC_Clock (37)
  *AT91C_PIOA_PER = (1 << 27);
  *AT91C_PIOA_PPUDR = (1 << 27);
  *AT91C_ADCC_MR= (1 << 9);   // Set the prescaler to 2 in ADC_MR->ADCClock = 14MHz
  // ADCClock = 84000000 / ( (PRESCAL+1) * 2 ) == 14 000 000 => PRESCAL = 2, (1 << 9). 

  *AT91C_ADCC_CHER = (1 << 1); // Enable (ADC) Channel 1 & 2
  *AT91C_ADCC_CHER = (1 << 2); // Enable (ADC) Channel 1 & 2
  NVIC_ClearPendingIRQ((IRQn_Type) 37);
  NVIC_SetPriority((IRQn_Type) 37, 0);
  NVIC_EnableIRQ((IRQn_Type) 37);
} 

void Light_Measurement(){
  *AT91C_ADCC_CR = (1 << 1); // START
  //*AT91C_ADCC_SR; // ADC Interrupt Status Register Clears
  // Trigger Interrupt on end of conversion of both Channel 0 & 1
  *AT91C_ADCC_IER = (1 << 1); // End of Conversion AT91C_ADC_EOC5, AT91C_ADC_EOC6
  *AT91C_ADCC_IER = (1 << 2); // End of Conversion AT91C_ADC_EOC5, AT91C_ADC_EOC6
}

void ADC_Handler(){
  *AT91C_ADCC_IDR = (1 << 1); // Disable interrupt for ADC5
  *AT91C_ADCC_IDR = (1 << 2); // Disable interrupt for ADC6
  int x = *AT91C_ADCC_SR; // get the one that triggered
  if ((x & (1 << 1)) == (1 << 1))
    booleanFlagSet = (booleanFlagSet | (1 << (2 - 1)));;
  if ((x & (1 << 2)) == (1 << 2))
    booleanFlagSet = (booleanFlagSet | (1 << (3 - 1)));;
}

/**
  * Number represent the brightness of the source
  * High (Very high), number means brighter Light source.
  * Low (Down to 0), number means Darker Light source.
*/
void Read_Light(){
  Light_Measurement();
  
  double lightCDR = 0;
  
  if ((booleanFlagSet & (1 << 2)) == (1 << 2)){
    lightCDR = *AT91C_ADCC_CDR1;
    booleanFlagSet = (booleanFlagSet & (~(1 << (2 - 1))));// Clear LS1 flag
  }
  if ((booleanFlagSet & (1 << 3)) == (1 << 3)){ // Light Sensor 2 Has a new value
    lightCDR = *AT91C_ADCC_CDR2;
    booleanFlagSet = (booleanFlagSet & (~(1 << (3 - 1))));// Clear LS2 flag
  }
  lightCDR = (lightCDR * 3.3)/0xFFF;
  char string[50];
  snprintf(string, 50, "%f", lightCDR);
  
  Write_Characters_2_Display(string);
}