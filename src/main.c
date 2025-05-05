/*
 * Tp2.c
 *
 * Created: 28/04/2025 15:44:17
 * Author : julit
 */ 
#include "main.h"

extern volatile uint8_t flag_tick;

const char* diccionario[] = {
	"Arbol", "Boton", "CDyMC", "ClavE", "Facil",
	"Gafas", "Hojas", "LiBro", "Lanza", "Nieve",
	"PeRro", "PecES", "PiAno", "PrYKe", "RUEDa",
	"SERIE", "SalUd", "Salud", "Silla", "Tecla",
	"Valor", "Verde", "YnHRz", "hARdD", "silla"
};

const uint8_t ARR_SIZE = 25;

int main(void) {
	// init 
	Timer0_Init(1000);   // Timer0: interrupci�n
	
	LCD_Init();
	LCDvisible();
	LCDcursorOnBlink();
	
	KEYPAD_Init();
	//MEF_Inicializar();     

	while (1) {
		if (flag_tick) {
			flag_tick = 0;
			//MEF_Actualizar();
		}
	}
	
}