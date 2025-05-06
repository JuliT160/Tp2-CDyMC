/*
 * keypad.h
 *
 * Created: 5/5/2025 20:31:17
 *  Author: julit
 */ 


/* keypad.h */
#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>
#include "main.h"

void KEYPAD_Init(void);
uint8_t KEYPAD_Scan(uint8_t *key);

#endif
