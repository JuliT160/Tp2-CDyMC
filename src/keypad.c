/* keypad.c */
#include "keypad.h"
#include <util/delay.h>

/* Mapeo físico: filas en PB4, PB3, PB0, PD7; columnas en PD3, PD5, PD4, PD2 */
static const uint8_t row_ddr_masks[4] = { (1<<PB4), (1<<PB3), (1<<PB0), (1<<PD7) };
static volatile uint8_t * const row_ddr_ports[4] = { &DDRB, &DDRB, &DDRB, &DDRD };
static volatile uint8_t * const row_port_ports[4] = { &PORTB, &PORTB, &PORTB, &PORTD };

static const uint8_t col_ddr_masks[4] = { (1<<PD3), (1<<PD5), (1<<PD4), (1<<PD2) };
static volatile uint8_t * const col_ddr_ports[4] = { &DDRD, &DDRD, &DDRD, &DDRD };
static volatile uint8_t * const col_port_ports[4] = { &PORTD, &PORTD, &PORTD, &PORTD };
static volatile uint8_t * const col_pin_ports[4]  = { &PIND,  &PIND,  &PIND,  &PIND  };

static const char keymap[4][4] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

void KEYPAD_Init(void) {
	// Filas = salidas, nivel alto
	for (uint8_t i = 0; i < 4; i++) {
		*row_ddr_ports[i]  |=  row_ddr_masks[i];
		*row_port_ports[i] |=  row_ddr_masks[i];
	}
	// Columnas = entradas con pull‑up
	for (uint8_t i = 0; i < 4; i++) {
		*col_ddr_ports[i]  &= ~col_ddr_masks[i];
		*col_port_ports[i] |=  col_ddr_masks[i];
	}
}

static uint8_t KeypadUpdate(void) {
	for (uint8_t row = 0; row < 4; row++) {
		// Poner todas las filas en 1
		for (uint8_t i = 0; i < 4; i++)
			*row_port_ports[i] |= row_ddr_masks[i];
		// Bajar la fila activa
		*row_port_ports[row] &= ~row_ddr_masks[row];
		
		// Leer columnas
		for (uint8_t col = 0; col < 4; col++) {
			if (!(*col_pin_ports[col] & col_ddr_masks[col])) {
				return (row * 4 + col);
			}
		}
	}
	return 0xFF; // No hay tecla presionada
}

uint8_t KEYPAD_Scan(uint8_t *key) {
	static uint8_t Old_key = 0xFF, Last_valid_key = 0xFF;
	uint8_t Key;
	
	Key = KeypadUpdate();
	if (Key == 0xFF) {
		Old_key = 0xFF;
		Last_valid_key = 0xFF;
		return 0;
	}
	
	if (Key == Old_key) {
		if (Key != Last_valid_key) {
			*key = keymap[Key/4][Key%4];
			Last_valid_key = Key;
			return 1;
		}
	}
	
	Old_key = Key;
	return 0;
}
