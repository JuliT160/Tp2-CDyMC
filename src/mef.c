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
static char buffer[3] = {0}; // para ascii
static char letra_actual;
static const char* palabra;
static uint8_t buffer_index = 0;

static uint16_t tiempoFinal = 0;

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
static estado_t estado_anterior = INICIO;  // Variable para controlar cambios de estado

void MEF_Inicializar(){
    // Declaracion inicial
    estado_actual = INICIO;
    errores = 0;
    posicion = 0;
    buffer_index = 0;
    Timer0_ResetContador();
    LCDclr();
    LCDGotoXY(0,0);
    LCDstring((uint8_t*)"RECORDAR", 8);
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
    _delay_ms(200);  // Corrección: 200 ms es un error, debería ser 2000 ms según el enunciado
}

static uint8_t esDigito(char c) {
    return (c >= '0' && c <= '9');
}

static void agregarAlBuffer(char c) {
    if (buffer_index < 2) {
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
    LCDGotoXY(0,1);
    LCDstring((uint8_t*)"     ", 5);
    buffer_index = 0;
}

static void limpiarLineaInferior(void) {
    LCDGotoXY(0,1);
    LCDstring((uint8_t*)"     ", 5);
    buffer_index = 0;
}

static void mostrarMensajeDerrota(void) {
    LCDclr();
    LCDGotoXY(0,0);
    LCDstring((uint8_t*)"Derrota", 7);
    Timer0_ResetContador();
}

static void mostrarMensajeVictoria(void) {
    LCDclr();
    LCDGotoXY(0,0);
    LCDstring((uint8_t*)"Victoria", 8);
    LCDGotoXY(0,1);
    LCDstring((uint8_t*)"Tiempo: ", 8);
    LCDescribeDato(tiempoFinal, 4); // Mostrar tiempo en milisegundos
    Timer0_ResetContador();
}

static uint8_t palabraCompleta(void) {
    return (posicion >= PALABRA_LARGO - 1);
}

static void avanzarPosicion(void) {
    posicion++;
}

static void incrementarErrores(void) {
    errores++;
}

void MEF_Actualizar() {
    uint8_t tecla = 0;  // Inicializar tecla en cada iteración

    switch (estado_actual) {
        case INICIO:
            if (estado_anterior != INICIO) {
                LCDclr();
                LCDGotoXY(0, 0);
                LCDstring((uint8_t*)"RECORDAR", 8);
                LCDGotoXY(0, 1);
                LCDstring((uint8_t*)"Presiona *", 10);
            }
            if (KEYPAD_Scan(&tecla)) {
                LCDGotoXY(0, 1);
                LCDstring((uint8_t*)"Tecla: ", 7);
                LCDsendChar(tecla);
                LCDstring((uint8_t*)"   ", 3);  // Limpiar resto de la línea
                if (tecla == '*') {
                    devolverPalabra();
                    mostrarPalabra();
                    Timer0_ResetContador();
                    estado_actual = MOSTRAR_CLAVE;
                }
            }
            break;
        case MOSTRAR_CLAVE:
            if (estado_anterior != MOSTRAR_CLAVE) {
                mostrarPalabra();  // Solo mostrar la palabra al entrar al estado
            }
            if (Timer0_LeerContador() >= 2000) {  // Corregir a 2000 ms según el enunciado
                LCDclr();
                Timer0_ResetContador();
                estado_actual = ESPERAR_CARACTER;
            }
            break;	
        case ESPERAR_CARACTER:
            if (estado_anterior != ESPERAR_CARACTER) {
                LCDGotoXY(0, 1);
                LCDstring((uint8_t*)"Ingresa: ", 9);
            }
            if (KEYPAD_Scan(&tecla)) {
                if (tecla == '#') {
                    estado_actual = VALIDAR_CARACTER;
                } else if (esDigito(tecla)) {
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
                    avanzarPosicion();
                    estado_actual = ESPERAR_CARACTER;
                }
            } else {
                incrementarErrores();
                LCDGotoXY(0, 1);
                LCDstring((uint8_t*)"Error", 5);
                if (errores >= MAX_ERRORES) {
                    estado_actual = DERROTA;
                } else {
                    estado_actual = ERROR;
                    Timer0_ResetContador();
                }
            }
            break;
        case ERROR:
            if (estado_anterior != ERROR) {
                limpiarLineaInferior();
            }
            if (Timer0_LeerContador() >= 1000) {
                estado_actual = ESPERAR_CARACTER;
            }
            break;
        case DERROTA:
            if (estado_anterior != DERROTA) {
                mostrarMensajeDerrota();
            }
            if (Timer0_LeerContador() >= 5000) {
                MEF_Inicializar();
            }
            break;
        case VICTORIA:
            if (estado_anterior != VICTORIA) {
                mostrarMensajeVictoria();
            }
            if (Timer0_LeerContador() >= 5000) {
                MEF_Inicializar();
            }
            break;
        default:
            break;
    }
    estado_anterior = estado_actual;  // Actualizar el estado anterior
}