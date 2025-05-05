/*
 * timer0.c
 *
 * Created: 01/05/2025 19:01:31
 *  Author: julit
 */ 
#include "timer0.h"

volatile uint8_t flag_tick = 0;
static volatile uint16_t contador_milis = 0;

void Timer0_Init(uint16_t frecuencia_hz) {
	// Modo CTC, limpia el contador al llegar al valor guardado en OCR0A
	TCCR0A = (1 << WGM01);

	// Prescaler 64 copiado del datasheet del ATmega328P
	// F_CPU = 16MHz, por lo que el prescaler es 64
	TCCR0B = (1 << CS01) | (1 << CS00);

	// Calculo valor para OCR0A segun frecuencia deseada
	// OCR0A = (F_CPU / (prescaler * frecuencia)) - 1
	uint32_t ocr = (F_CPU / (64UL * frecuencia_hz)) - 1;
	OCR0A = (uint8_t)ocr;
	sei();
	// Habilito interrupcion por comparacion de OCR0A
	// Se habilita la interrupcion de comparacion A
	TIMSK0 |= (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect) {
	flag_tick = 1;
	contador_milis++;
}

uint16_t Timer0_LeerContador(void) {
	return contador_milis;
}

void Timer0_ResetContador(void) {
	contador_milis = 0;
}
