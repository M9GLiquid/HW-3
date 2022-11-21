#include "display.h"
#include "at91sam3x8.h"
#include "light_sensor.h"
#include "system_sam3x.h"
#include <stdio.h>

void Write_Characters_2_Display(unsigned char *key) {
  for (int i = 0; i < strlen(key) ; i++){
    Write_Character_2_Display(key[i]);
  }
}

void Write_Character_2_Display(unsigned int key) {
  // TODO: Set the rest of the characters!
  switch (key) {
  case 1:
    data = 0x11;
    break;
  case 2:
    data = 0x12;
    break;
  case 3:
    data = 0x13;
    break;
  case 4:
    data = 0x14;
    break;
  case 5:
    data = 0x15;
    break;
  case 6:
    data = 0x16;
    break;
  case 7:
    data = 0x17;
    break;
  case 8:
    data = 0x18;
    break;
  case 9:
    data = 0x19;
    break;
  case 10: // *
    data = 0x0A;
    break;
  case 11: // 0
    data = 0x10;
    break;
  case 12: // #
    data = 0x03;
    break;
  default:
    return;
  }
  // Write the data, by incrementing ADP each time written
  Write_Data_2_Display(data);
  Write_Command_2_Display(0xC0);
}

void Blank_Display() {
  // Set Cursor at (x,y) -> (0,0)
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x24);

  for (int x = 0; x < _DISPLAY_COL; x++) {
    for (int y = 0; y < _DISPLAY_ROW; y++) {
      // Write "nothing" into each "pixel" of the screen
      Write_Data_2_Display(0x00);
      Write_Data_2_Display(0x00);
      Write_Command_2_Display(0xC0);
    }
  }

  // Re-Set Cursor at (x,y) -> (0,0)
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x24);
}

unsigned char Read_Status_Display() {
  unsigned char temp;

  *AT91C_PIOC_OER = (0x1F000);          // make databus as input
  *AT91C_PIOC_ODR = (_DISPLAY_DATABUS); // make databus as input

  *AT91C_PIOC_SODR = (_DISPLAY_DIR); // Set dir as input
  *AT91C_PIOC_CODR = (_DISPLAY_OE);  // Clear/enable output (74chip 0 = enable)
  *AT91C_PIOC_SODR = (_DISPLAY_CD);  // Set C/D
  *AT91C_PIOC_CODR = (_DISPLAY_CE);  // Clear chip select display  CE
  *AT91C_PIOC_CODR = (_DISPLAY_RD);  // Clear read display
  Delay(25);                         // Make a Delay
  temp = (*AT91C_PIOC_PDSR &
          (_DISPLAY_DATABUS));      // Read data bus and save it in temp (PSDR?)
  *AT91C_PIOC_SODR = (_DISPLAY_CE); // Set chip select display
  *AT91C_PIOC_SODR = (_DISPLAY_RD); // Set read display
  *AT91C_PIOC_SODR = (_DISPLAY_OE); // Disable output (74chip)
  *AT91C_PIOC_CODR = (_DISPLAY_DIR); // Set dir as output (74chip)
  return temp;
}

void Write_Command_2_Display(unsigned char command) {
  // Wait until Read_Status_Display returns aKn O
  while ((~Read_Status_Display() & (0xC)) == 0xC) {
  }

  *AT91C_PIOC_OER = (0x2F000);          // make Command pins as input
  *AT91C_PIOC_OER = (_DISPLAY_DATABUS); // make databus as input

  *AT91C_PIOC_CODR = (_DISPLAY_DATABUS); // Clear databus
  *AT91C_PIOC_SODR =
      (command
       << 2); // Set Command to databus (To get back the 2 zeroes) PC(2-9)
  *AT91C_PIOC_CODR = (_DISPLAY_DIR);    // Set dir as output (74chip)
  *AT91C_PIOC_CODR = (_DISPLAY_OE);     // Enable 'output' (74chip)
  *AT91C_PIOC_OER = (_DISPLAY_DATABUS); // Set databus as output
  *AT91C_PIOC_SODR = (_DISPLAY_CD); // Set C/D signal High (1 = Command) //CD
                                    // High gives Command Write then WR = Low
  *AT91C_PIOC_CODR = (_DISPLAY_CE);     // Clear chip select display
  *AT91C_PIOC_CODR = (_DISPLAY_WR);     // Clear write display
  Delay(25);                            // Make a Delay
  *AT91C_PIOC_SODR = (_DISPLAY_CE);     // Set chip enable display
  *AT91C_PIOC_SODR = (_DISPLAY_WR);     // Set write display
  *AT91C_PIOC_SODR = (_DISPLAY_OE);     // Disable output (74chip)
  *AT91C_PIOC_ODR = (_DISPLAY_DATABUS); // Make databus as input
}

void Write_Data_2_Display(unsigned char data) {
  // Wait until Read_Status_Display returns an OK
  while ((~Read_Status_Display() & (0xC)) == 0xC) {
  }

  *AT91C_PIOC_OER = (0x2F000);          // Set databus as output
  *AT91C_PIOC_OER = (_DISPLAY_DATABUS); // Set databus as output

  *AT91C_PIOC_CODR = (_DISPLAY_DATABUS); // Clear databus
  *AT91C_PIOC_SODR = (data << 2);        // Set "data" to databus
  *AT91C_PIOC_CODR = (_DISPLAY_DIR);     // Set dir as output (74chip)
  *AT91C_PIOC_CODR = (_DISPLAY_OE);      // Enable 'output' (74chip)
  *AT91C_PIOC_OER = (_DISPLAY_DATABUS);  // Set databus as output
  *AT91C_PIOC_CODR = (_DISPLAY_CD); // Set C/D signal Low (0 = data) //CD Low
                                    // gives Data Write then WR = Low
  *AT91C_PIOC_CODR = (_DISPLAY_CE);     // Clear chip select display
  *AT91C_PIOC_CODR = (_DISPLAY_WR);     // Clear write display
  Delay(25);                            // Make a Delay
  *AT91C_PIOC_SODR = (_DISPLAY_CE);     // Set chip enable display
  *AT91C_PIOC_SODR = (_DISPLAY_WR);     // Set write display
  *AT91C_PIOC_SODR = (_DISPLAY_OE);     // Disable output (74chip)
  *AT91C_PIOC_ODR = (_DISPLAY_DATABUS); // Make databus as input
}

void Init_Display() {
  initDisplay();

  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40); // Set text home address
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x40);
  Write_Command_2_Display(0x42); // Set graphic home address
  Write_Data_2_Display(0x1e);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41); // Set text area
  Write_Data_2_Display(0x1e);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43); // Set graphic area
  Write_Command_2_Display(0x80); // text mode
  Write_Command_2_Display(0x94); // Text on graphic off

  Blank_Display();
}

void initDisplay() {
  
  *AT91C_PMC_PCER = (0x6000); // PIOC & PIOD
  *AT91C_PIOC_PER = (0xFFF3FC);       // Enable PC(2-9)
  *AT91C_PIOD_PER = (_DISPLAY_RESET); // 

  *AT91C_PIOC_PPUDR = (0xFFF3FC);       // 
  *AT91C_PIOD_PPUDR = (_DISPLAY_RESET); // 

  *AT91C_PIOD_OER = (_DISPLAY_RESET); // 

  *AT91C_PIOC_OER = (_DISPLAY_FS);  // 
  *AT91C_PIOC_OER = (_DISPLAY_RV);  // 
  *AT91C_PIOC_SODR = (_DISPLAY_FS); // 
  *AT91C_PIOC_CODR = (_DISPLAY_RV); // 

  *AT91C_PIOC_OER = (1 << 12); // 
  *AT91C_PIOC_OER = (1 << 13); // 

  *AT91C_PIOD_CODR = (_DISPLAY_RESET); // Reset Screen
  Delay(25); // 10ms of delay for the screen to start and such
  *AT91C_PIOD_SODR = (_DISPLAY_RESET); // Make the display act normal
}