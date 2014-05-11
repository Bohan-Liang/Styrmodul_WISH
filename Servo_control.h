/*
 * Servo_control.h
 *
 * Skapad: 2014-03-24 21:01:15
 * Inneh�ll: Funktioner som driver servo samt omvandning mellan koordinater och servovinklar
 */ 

#ifndef SERVO_CONTROL_H_
#define SERVO_CONTROL_H_

#define F_CPU 16000000UL // Ange CPU frekvens
							// ber�kning av baud rate och f�rdr�jningsfunktioner �r beroende av detta

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "Control_table.h"
#include "Leg_Def.h"

// Anv�ndbara genv�g (macros)
#define BIT_SET(register_name, bit_name) register_name |= (1<<bit_name)
#define BIT_CLEAR(register_name, bit_name) register_name &= ~(1<<bit_name)
#define BIT_IS_SET(register_name, bit_name) register_name & (1<<bit_name)
#define BIT_IS_CLEAR(register_name, bit_name) !(register_name & (1<<bit_name))

// Globala konstanter

#define RECEPTION_BUFFER_SIZE 32 // Ange storleken p� buffret f�r return packet, jag tycker 32 b�r r�cka

// Globala variabler
unsigned char reception_buffer[RECEPTION_BUFFER_SIZE]; // lagrar return packet fr�n servo
volatile int buffer_index; // index f�r att information ska hamna i r�tt plats
int error_code; //lagra senaste felkoden fr�n servo

// Initiera med �nskad baud rate
void init_USART(long baud_rate);

// Aktivera s�ndning (avaktivera mottagning)
void USART_transmit_mode();

// tv�rtom
void USART_receive_mode();

void send_to_servo(unsigned char data);
// OBS: mottagning sker via en avbrott-vektor

// l�s av senaste fel fr�n reception_buffer
// int get_error_code();

// omvandla abosluta servovinkel till vinkeldata
// vinklar �r (-150,150) grader
int angle_to_data(float angle);

// KUNGLIG FUNKTION! styr alla servo samtldigt
void write_to_all();

// styr en servo
void set_single_servo_2byte(unsigned char id, unsigned char register_name, int data);
void set_single_servo_1byte(unsigned char id, unsigned char register_name, unsigned char data);

void set_compliance(unsigned char id, unsigned char CW_margin, unsigned char CCW_margin, unsigned char CW_slope, unsigned char CCW_slope);

// omvandla grader (-150 - 150) till servos vinkeldata (0 - 1024)
void translate_leg_angle();

// hj�lpfunktion f�r kvadrering
float SQ(float num);

// omvandla kartesiska koordinater till servovinklar hos ett ben
void set_pos_leg(leg_info* leg, float x, float y, float z);

// Initiera benen med sina initiala parametrar
void init_leg(float init_x, float init_y, float init_z);

#endif /* SERVO_CONTROL_H_ */