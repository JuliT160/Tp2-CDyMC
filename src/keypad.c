/*
 * CFile1.c
 *
 * Created: 01/05/2025 10:28:03
 *  Author: julit
 */ 

#include "keypad.h"

const uint8_t keymap[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'0', '*', '#', 'D'}
};

void KEYPAD_Init(void){
	// Configuro filas como salidas (D0..D3)
	DDRD |= 0x0F;
	// Configuro columnas como entradas (D4..D7) con pull-up
	DDRD &= ~(0xF0);
	PORTD |= 0xF0;
}

uint8_t KEYPAD_Scan(uint8_t *key) {
	
	for (uint8_t row = 0; row < 4; row++) {
		// Pongo todas las filas en 1
		PORTD |= 0x0F;
		// Bajo solo la fila actual
		PORTD &= ~(1 << row);
		_delay_ms(20); // Pequeño delay para que se estabilice

		for (uint8_t col = 0; col < 4; col++) {
			if (!(PIND & (1 << (col + 4)))) { // Si columna esta en 0 (tecla presionada)
				// Devuelvo el caracter correspondiente
				*key = keymap[row][col];
				return 1; // Se detecto tecla
			}
		}
	}

	return 0; // No se presionó ninguna tecla
}
