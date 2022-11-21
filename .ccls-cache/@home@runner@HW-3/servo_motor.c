#include "servo_motor.h"
#include "at91sam3x8.h"
#include "main.h"
#include "system_sam3x.h"
#include <stdio.h>

void Init_Servo_Motor() {
  //*AT91C_PMC_PCER = (AT91C_ID_PIOB); // Enable PMC
  // Let peripheral control the pin REG_PIOB_PDR
  *AT91C_PIOB_PDR = (AT91C_PIO_PB17);
  // Activate peripheral B to control the pin in REG_PIOB_ABSR (0 PIOA, 1 PIOB)
  *AT91C_PIOB_ABMR = (AT91C_PIO_PB17);
  *AT91C_PMC_PCER1 = (1 << 4); // Enable PWM Clock
  *AT91C_PWMC_ENA = (1 << 1);  // CHID1 AT91C_PWMC_CPRE_MCK_DIV_32
  *AT91C_PWMC_CH1_CMR = (AT91C_PWMC_CPRE_MCK_DIV_32);
  *AT91C_PWMC_CH1_CPRDR =
      (52500); // 20ms [84 000 000 HZ / 32 * 20 ((1/frek) = period]
  *AT91C_PWMC_CH1_CDTYR = (2625); // 52500 / 20 => 1 ms
}

void Move_Servo(int key) {
  switch (key) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
    *AT91C_PWMC_CH1_CDTYUPDR = (2625 + 262.5 * key); // 10 Degrees
    break;
  default:
    return;
  }
  Delay(200000);
}

// 2362 gives 90 Degrees
// 262.5 gives ~10 Degrees