#include "mef.h"
#include "main.h"

// Estados de la MEF
typedef enum {
    ESTADO_INICIAL,
    ESTADO_ESPERANDO_PALABRA,
    ESTADO_INGRESANDO_PALABRA,
    ESTADO_PALABRA_CORRECTA,
    ESTADO_PALABRA_INCORRECTA
} estado_t;

extern const char* diccionario[];
extern const uint8_t ARR_SIZE;

// Variables globales de la MEF
static estado_t estado_actual;
static uint8_t palabra_actual;
static uint8_t indice_letra;
static char palabra_ingresada[6]; // 5 letras + '\0'

void MEF_Inicializar(void) {
    estado_actual = ESTADO_INICIAL;
    palabra_actual = 0;
    indice_letra = 0;
    LCDclr();
    LCDstring((uint8_t*)"Presione *", 10);
}

void MEF_Actualizar(void) {
    uint8_t tecla;
    if (!KEYPAD_Scan(&tecla)) return; // No se presionó ninguna tecla
    
    switch (estado_actual) {
        case ESTADO_INICIAL:
            if (tecla == '*') {
                estado_actual = ESTADO_ESPERANDO_PALABRA;
                LCDclr();
                LCDstring((uint8_t*)diccionario[palabra_actual], 5);
            }
            break;
            
        case ESTADO_ESPERANDO_PALABRA:
            if (tecla == '*') {
                estado_actual = ESTADO_INGRESANDO_PALABRA;
                indice_letra = 0;
                LCDclr();
                LCDstring((uint8_t*)"Ingrese:", 8);
            }
            break;
            
        case ESTADO_INGRESANDO_PALABRA:
            if (tecla == '#') {
                palabra_ingresada[indice_letra] = '\0';
                if (strcmp(palabra_ingresada, diccionario[palabra_actual]) == 0) {
                    estado_actual = ESTADO_PALABRA_CORRECTA;
                    LCDclr();
                    LCDstring((uint8_t*)"Correcto!", 9);
                } else {
                    estado_actual = ESTADO_PALABRA_INCORRECTA;
                    LCDclr();
                    LCDstring((uint8_t*)"Incorrecto!", 11);
                }
            } else if (tecla != '*' && indice_letra < 5) {
                palabra_ingresada[indice_letra++] = tecla;
                LCDsendChar(tecla);
            }
            break;
            
        case ESTADO_PALABRA_CORRECTA:
        case ESTADO_PALABRA_INCORRECTA:
            if (tecla == '*') {
                palabra_actual = (palabra_actual + 1) % ARR_SIZE;
                estado_actual = ESTADO_ESPERANDO_PALABRA;
                LCDclr();
                LCDstring((uint8_t*)diccionario[palabra_actual], 5);
            }
            break;
    }
}
