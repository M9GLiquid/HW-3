#ifndef DISPLAY_H
#define DISPLAY_H

// PIOC
#define _DISPLAY_RV ((unsigned int)1 << 19)  //
#define _DISPLAY_FS ((unsigned int)1 << 18)  //
#define _DISPLAY_WR ((unsigned int)1 << 17)  //
#define _DISPLAY_RD ((unsigned int)1 << 16)  //
#define _DISPLAY_CE ((unsigned int)1 << 15)  //
#define _DISPLAY_CD ((unsigned int)1 << 14)  //
#define _DISPLAY_DIR ((unsigned int)1 << 13) //
#define _DISPLAY_OE ((unsigned int)1 << 12)  //
#define _DISPLAY_DATABUS ((unsigned int)0x3FC)
// PIOD
#define _DISPLAY_RESET ((unsigned int)0x1)

#define _DISPLAY_COL ((unsigned char)0x50) // 32, 40, 64, 80(0x50)
#define _DISPLAY_ROW                                                           \
  ((unsigned char)0x20) // 2, 4, 6 ,8 ,10, 12, 14, 16, 20, 24, 28, 32(0x20)

void Delay(int Value);
void Blank_Display(void);
extern unsigned char data;

unsigned char Read_Status_Display(void);
void Write_Command_2_Display(unsigned char command);
void Write_Data_2_Display(unsigned char data);
void Write_Character_2_Display(char key);
void Write_Characters_2_Display(char *key);
void Init_Display(void);
// All initial setup values
void init(void);
void initDisplay(void);

#endif