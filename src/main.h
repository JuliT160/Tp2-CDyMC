#ifndef MAIN_H
#define MAIN_H

#define F_CPU 16000000UL

// Bibliotecas estandar
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <avr/interrupt.h>

// AVR
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

// Includes de archivos propios
#include "lcd.h"
#include "keypad.h"
#include "timer0.h"
#include "mef.h"

#endif // MAIN_H
