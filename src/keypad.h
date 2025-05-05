/*
 * keypad.h
 *
 * Created: 5/5/2025 20:31:17
 *  Author: julit
 */ 


 #ifndef TECLADO4X4_H_
 #define TECLADO4X4_H_
 
 #include "main.h"
 
 void KEYPAD_Init();
 uint8_t KEYPAD_Scan(uint8_t *key);
 
 #endif /* TECLADO4X4_H_ */