#include "main.h"
#include "at91sam3x8.h"
#include "display.h"
#include "key_pad.h"
#include "light_sensor.h"
#include "servo_motor.h"
#include "system_sam3x.h"
#include "temperature.h"

unsigned char data = 0;           //  variable defined
unsigned int time = 0;            //  variable defined
unsigned char booleanFlagSet = 0; // All flags set as false/0; variable defined

int main(void) {
  config();

  while (1) { // infinity loop
    Read_Temp();
    Write_Character_2_Display(' ');
    Read_Light();
    Read_Keys();

    Delay(5000000);
    Blank_Display();
  }
  return 0;
}

void config() {
  SystemInit();
  SysTick_Config(SystemCoreClock * 0.001); // 1 ms Initiate Pulse
  Init_Display();
  Init_Temp();
  Init_LightSensor();
  Light_Measurement();
  Init_Servo_Motor();
}

void Delay(int value) {
  for (int i = 0; i < value; i++)
    __asm("nop"); // asm() Doesn't work in newer versions of c, c99 does not
                  // support asm(); command
}

void SysTick_Handler() { time++; }
