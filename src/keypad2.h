/*
 * teclado4x4.h
 *
 * Created: 25/4/2022 14:31:13
 *  Author: Barcala
 */ 


 #ifndef KEYPAD2_H_
 #define KEYPAD2_H_
 
 #include "main.h"
 
 // Prototipos de funci�n
 
 /************************************************************************/
 /* ESTABLECE LOS PUERTOS PARA LA LECTURA DE LOS 16 PULSADORES                                                                     */
 /************************************************************************/
 void KEYPAD_Init();
 
 // Permite encuestar el teclado para determinar si se presion� una tecla y el valor de la misma. La funci�n debe retornar 0
 // si no se presion� ninguna tecla o 1 si se presion� alguna. 
 //El valor de la tecla (o car�cter ASCII) debe retornarse por el puntero pasado como par�metro (*key).
 uint8_t KEYPAD_Scan(uint8_t *key);
 
 #endif /* TECLADO4X4_H_ */