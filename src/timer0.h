/*
 *
 * Created: 01/05/2025 19:00:25
 *  Author: julit
 */ 

#ifndef TIMER0_H
#define TIMER0_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "main.h"

void Timer0_Init(uint16_t frecuencia_hz);
uint16_t Timer0_LeerContador(void);
void Timer0_ResetContador(void);

extern volatile uint8_t flag_tick;

#endif