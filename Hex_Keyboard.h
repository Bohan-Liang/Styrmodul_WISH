/*
 * Hex_Keyboard.h
 *
 * Created: 2014-04-01 10:50:37
 *  Author: Bohan
 */ 


#ifndef HEX_KEYBOARD_H_
#define HEX_KEYBOARD_H_
#include "WISH_Gait_Engine.h"

void init_hex();

void translate_PIN();

void translate_PIN_climb();

volatile unsigned char hex_input;

/*
volatile int PA_3; //LSB
volatile int PA_4;
volatile int PA_5;
volatile int PA_6; //MSB

*/

#endif /* HEX_KEYBOARD_H_ */