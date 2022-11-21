#include "temperature.h"
#include "at91sam3x8.h"
#include "display.h"
#include "main.h"
#include "system_sam3x.h"
#include <stdio.h>

void Init_Temp() {

  *AT91C_PMC_PCER = (AT91C_ID_PIOB);  // Enable PIOB
  *AT91C_PIOB_PER = (AT91C_PIO_PB25); // Enable PIO pin needed
  *AT91C_PMC_PCER =
      (1 << AT91C_ID_TC0); // Enable TC0 clocks (AT91C_ID_TC0 Fungerar ej??)
  *AT91C_TC0_CMR = (AT91C_TC_CLKS_TIMER_DIV1_CLOCK); // Capture mode for
                                                     // Timer_Clock1 (Value = 0)
  //*AT91C_TC0_CCR = 5;
  *AT91C_TC0_CCR = (AT91C_TC_SWTRG | AT91C_TC_CLKEN); // 0x5
  *AT91C_TC0_CMR = (AT91C_TC_LDRA_FALLING);           // 0x60000
  *AT91C_TC0_CMR = (*AT91C_TC0_CMR | (1 << 18));      // 0x60000

  *AT91C_PIOB_OER =
      (1 << 25); // Enable PIO pin needed as output (AT91C_PIO_PB25)
  while (time < 17) {
  } // Wait for RESET pulse to be complete
  time = 0;
  *AT91C_PIOB_ODR =
      (1 << 25); // Disable PIO pin needed as output (AT91C_PIO_PB25)

  NVIC_EnableIRQ((IRQn_Type)27);
  NVIC_ClearPendingIRQ((IRQn_Type)27);
  // NVIC_SetPriority((IRQn_Type)27, 2);
  *AT91C_TC0_IER = (1 << 6); // Enable interrupt for LDRBS (AT91C_TC_LDRBS)
}

void Temp_Mesurement(void) {
  *AT91C_PIOB_OER = AT91C_PIO_PB25; // Enable PIO pin needed as output
  Delay(25);
  *AT91C_PIOB_SODR = AT91C_PIO_PB25;
  *AT91C_PIOB_ODR = AT91C_PIO_PB25; // Disable PIO pin needed as output
  *AT91C_PIOB_CODR = AT91C_PIO_PB25;
  booleanFlagSet = (booleanFlagSet & (~(1 << (1 - 1)))); // Clear TC0 flag

  Delay(25);
  *AT91C_TC0_CCR = (AT91C_TC_SWTRG); // Trigger Software reset.
  *AT91C_TC0_IER = AT91C_TC_LDRBS;   // Enable interrupt for LDRBS
  *AT91C_TC0_SR;                     // Read and clear.
}

void TC0_Handler() {
  *AT91C_TC0_IDR = (AT91C_TC_LDRBS); // Disable interrupt for LDRBS
  booleanFlagSet =
      (booleanFlagSet | (1 << (1 - 1))); // Rising Edge detected, set TC Flag
}

void Read_Temp() {
  if ((booleanFlagSet & (1 << 0)) == 0)
    return;
  // if (*AT91C_TC0_RB <= *AT91C_TC0_RA)
  //  return;
  Temp_Mesurement();
  int temperature = 0;
  int tempDifference = (*AT91C_TC0_RB - *AT91C_TC0_RA);
  temperature = ((tempDifference / (42 * 5)) - 273.15); // 42 from MCK/2
  // printf("RB >: %d\n", *AT91C_TC0_RB);
  // printf("RA: %d\n", *AT91C_TC0_RA);
  // printf("Temperature: %d\n", temperature);

  char string[50];
  snprintf(string, 50, "%d", temperature);

  Write_Characters_2_Display(string);
}
