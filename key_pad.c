#include "main.h"
#include "at91sam3x8.h"
#include "servo_motor.h"
#include "key_pad.h"
#include "system_sam3x.h"

void Init_keypad(){
  *AT91C_PMC_PCER = (0x6000); // Enable PIOC & PIOD
  
  *AT91C_PIOC_PER = (_KEYPAD_PINS); // Peripheral enable PC(2-5), PC(7-9)
  *AT91C_PIOD_PER = (_KEYPAD_OE);   // Enable PD2

  *AT91C_PIOC_PPUDR = (_KEYPAD_PINS); // Pullup disable PC(2-5), PC(7-9)
  *AT91C_PIOD_PPUDR = (_KEYPAD_OE);   // Pullup disable PD(2)
}

void Read_Keys(){
  *AT91C_PIOD_OER = (_KEYPAD_OE);          // Output Enable
  *AT91C_PIOC_OER = (_KEYPAD_COLUMN_PINS); // Make all Column pin as output

  *AT91C_PIOD_CODR = (_KEYPAD_OE);          // Clear OE KEY BUS (Active Low)
  *AT91C_PIOC_SODR = (_KEYPAD_COLUMN_PINS); // Set all Column pin as high
  *AT91C_PIOC_ODR = (_KEYPAD_ROW_PINS);     // Disable all Row pins
  for (int col = 0; col < 3; col++) {       // LOOP Column 0 - 2
    Delay(25);                           // Duct tape solution :) Fast Pressing
    *AT91C_PIOC_CODR = (1 << (7 + col)); // Clear one column at the time
    for (int row = 0; row < 4; row++) {  // LOOP Row 0 - 3
      // Read row and check if bit is zero
      if ((*AT91C_PIOC_PDSR & (1 << (2 + row))) == 0) {
        int value = row * 3 + col + 1; // Value = Row*3+Col+1;
        if (value == 11)
          value = 0;
        Move_Servo(value);
        
        *AT91C_PIOC_SODR = (1 << (7 + col));     // Set all Column pin
        *AT91C_PIOC_ODR = (_KEYPAD_COLUMN_PINS); // Make all Column pin as input
        return;
      }
    }
  }  
}
