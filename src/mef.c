/*
 * mef.c
 *
 * Created: 01/05/2025 21:01:18
 *  Author: julit
 */ 
#include "mef.h"

#define MAX_ERRORES 3
#define PALABRA_LARGO 5

extern const char* diccionario[];		// Extern declara una variable que se encuentra en otro archivo
extern const uint8_t ARR_SIZE;

static uint8_t errores = 0;
static uint8_t posicion = 0;
static char buffer[4] = {0}; // para ascii (3 dígitos + null)
//static char letra_actual;
static const char* palabra;
static uint8_t buffer_index = 0;
static uint8_t enPartida = 0;
static uint16_t tiempoFinal = 0;
static uint8_t mensajeError = 0;

typedef enum {
	INICIO,
	MOSTRAR_CLAVE,
	ESPERAR_CARACTER,
	VALIDAR_CARACTER,
	ERROR,
	DERROTA,
	VICTORIA
} estado_t;

static estado_t estado_actual;

void MEF_Inicializar(){
	// Declaracion inicial
	estado_actual = INICIO;
	errores = 0;
	posicion = 0;
	buffer_index = 0;
	enPartida = 1;
	Timer0_ResetContador();
	LCDclr();
	LCDGotoXY(0,0);
	LCDstring((uint8_t*)"RECORDAR PASS", 13);
	LCDGotoXY(0,1);
	LCDstring((uint8_t*)"Presiona *", 10);
}

static void devolverPalabra(){
	// Iniciar semilla
	srand(Timer0_LeerContador());
	
	//Obtener pocicion aleatoria, el diccionario y el arrsize deben estar declarados
	uint8_t i = rand() % ARR_SIZE;
	
	palabra = diccionario[i];		// Guardo la palabra elegida
}

static void mostrarPalabra(){
	LCDclr();
	LCDGotoXY(0,0);
	LCDstring((uint8_t*)palabra, PALABRA_LARGO);
}

static uint8_t esDigito(char c) {
	return (c >= '0' && c <= '9');
}

static void agregarAlBuffer(char c) {
	if (buffer_index < 3) {
		buffer[buffer_index++] = c;
	}
}

static uint8_t caracterCorrecto(void) {
	buffer[buffer_index] = '\0';
	uint8_t codigo = atoi(buffer);
	return ((char)codigo == palabra[posicion] && (((char)codigo >= 'A' && (char)codigo <= 'Z') || ((char)codigo >= 'a' && (char)codigo <= 'z')));
}

static void mostrarCaracterEnPantalla(void) {
	LCDGotoXY(posicion, 0);
	LCDsendChar(palabra[posicion]);
	LCDGotoXY(posicion + 1, 0);  // Mover el cursor justo después del carácter mostrado
	buffer_index = 0;
}

static void limpiarLineaInferior() {
	LCDGotoXY(0,1);
	LCDstring((uint8_t*)"          ", 10);
	buffer_index = 0;
	LCDGotoXY(posicion, 0);
	
}

static uint8_t palabraCompleta(void) {
	return (posicion >= PALABRA_LARGO - 1);
}

static void mostrarMensajeError(void) {
	LCDGotoXY(0,1);
	char mensaje[16];
	sprintf(mensaje, "Errores: %d", errores);
	LCDstring((uint8_t*)mensaje, strlen(mensaje));
}


void MEF_Actualizar() {
	char tecla;
	static uint8_t posicion_actual = 0;
	
	switch (estado_actual){
		case INICIO:
			if (KEYPAD_Scan((uint8_t*)&tecla) && tecla == '*') {
				devolverPalabra();
				mostrarPalabra();
				Timer0_ResetContador();
				posicion_actual = 0;
				estado_actual = MOSTRAR_CLAVE;
			}
			break;
		case MOSTRAR_CLAVE:
			if (Timer0_LeerContador() >= 2000) {
				LCDclr();
				Timer0_ResetContador();
				posicion_actual = 0;
				estado_actual = ESPERAR_CARACTER;
			}
			break;	
		case ESPERAR_CARACTER:
			if (KEYPAD_Scan((uint8_t*)&tecla)) {
				if (tecla == '#') {
					posicion_actual = 0;
					estado_actual = VALIDAR_CARACTER;
				} else if (esDigito(tecla) && posicion_actual < 3) {
					LCDGotoXY(posicion_actual, 1);
					LCDsendChar(tecla);
					posicion_actual++;
					agregarAlBuffer(tecla);
				}
			}
			break;
		case VALIDAR_CARACTER:
			if (caracterCorrecto()) {
				mostrarCaracterEnPantalla();
				if (palabraCompleta()) {
					tiempoFinal = Timer0_LeerContador();
					estado_actual = VICTORIA;
				} else {
					posicion++;
					posicion_actual = 0;
					estado_actual = ESPERAR_CARACTER;
				}
			} else {
				errores++;
				if (errores >= MAX_ERRORES) {
					estado_actual = DERROTA;
				} else {
					estado_actual = ERROR;
					mensajeError = 1;
					Timer0_ResetContador();
				}
			}
			limpiarLineaInferior();
			break;
		case ERROR:
			if (mensajeError) {
				mostrarMensajeError();
				mensajeError = 0;
				Timer0_ResetContador();
			}
			if (Timer0_LeerContador() >= 2000) {
				limpiarLineaInferior();
				posicion_actual = 0;
				estado_actual = ESPERAR_CARACTER;
			}
			break;
		case DERROTA:
			if (enPartida) {
				enPartida = 0;
				LCDclr();
				LCDGotoXY(0,0);
				LCDstring((uint8_t*)"Derrota", 7);
				Timer0_ResetContador();
			}
			if (Timer0_LeerContador() >= 5000) {
				MEF_Inicializar();
			}
			break;
		case VICTORIA:
			if (enPartida) {
				enPartida = 0;
				LCDclr();
				LCDGotoXY(0,0);
				LCDstring((uint8_t*)"Victoria", 8);
				LCDGotoXY(0,1);
				char tiempo_str[16];
				sprintf(tiempo_str, "Tiempo: %u s", tiempoFinal/1000);
				LCDstring((uint8_t*)tiempo_str, strlen(tiempo_str));
				Timer0_ResetContador();
			}
			if (Timer0_LeerContador() >= 5000) {
				MEF_Inicializar();
			}
			break;
		default:
			break;
	}
}