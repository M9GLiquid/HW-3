#ifndef KEY_PAD_H
#define KEY_PAD_H

// KEYPAD
#define _KEYPAD_COLUMN_PINS ((unsigned int)0x380)
#define _KEYPAD_ROW_PINS ((unsigned int)0x3C)    
#define _KEYPAD_OE ((unsigned int)0x4)           
#define _KEYPAD_PINS ((unsigned int)0x3BC)       

void Init_keypad(void);
void Read_Keys(void);

#endif