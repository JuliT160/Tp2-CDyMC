/*
 * CFile1.c
 *
 * Created: 01/05/2025 10:28:03
 *  Author: julit
 */ 

#include "keypad.h"

void KEYPAD_Init(void) {
    DDRD |= 0x0F;       // Filas (D0-D3) como salidas
    DDRD &= ~(0xF0);    // Columnas (D4-D7) como entradas
    PORTD |= 0xF0;      // Activar pull-up en columnas
}

uint8_t KEYPAD_Scan(uint8_t *key) {
    const uint8_t keymap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    for (uint8_t row = 0; row < 4; row++) {
        PORTD |= 0x0F;          // Poner todas las filas en 1
        PORTD &= ~(1 << row);   // Bajar la fila actual a 0
        _delay_ms(5);           // Delay para estabilizar

        for (uint8_t col = 0; col < 4; col++) {
            if (!(PIND & (1 << (col + 4)))) {  // Si la columna está en 0
                *key = keymap[row][col];
                _delay_ms(20);      // Delay para debounce
                if (!(PIND & (1 << (col + 4)))) {  // Verificar nuevamente
                    return 1;       // Tecla confirmada
                }
            }
        }
    }
    return 0;  // No se detectó tecla
}
