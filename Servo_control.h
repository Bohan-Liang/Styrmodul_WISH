/*
 * Servo_control.h
 *
 * Skapad: 2014-03-24 21:01:15
 * Innehåll: Funktioner som driver servo samt omvandning mellan koordinater och servovinklar
 */ 

#ifndef SERVO_CONTROL_H_
#define SERVO_CONTROL_H_

#define F_CPU 16000000UL // Ange CPU frekvens
							// beräkning av baud rate och fördröjningsfunktioner är beroende av detta

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "Control_table.h"
#include "Leg_Def.h"

// Användbara genväg (macros)
#define BIT_SET(register_name, bit_name) register_name |= (1<<bit_name)
#define BIT_CLEAR(register_name, bit_name) register_name &= ~(1<<bit_name)
#define BIT_IS_SET(register_name, bit_name) register_name & (1<<bit_name)
#define BIT_IS_CLEAR(register_name, bit_name) !(register_name & (1<<bit_name))

// Globala konstanter

#define RECEPTION_BUFFER_SIZE 32 // Ange storleken på buffret för return packet, jag tycker 32 bör räcka

// Globala variabler
unsigned char reception_buffer[RECEPTION_BUFFER_SIZE]; // lagrar return packet från servo
volatile int buffer_index; // index för att information ska hamna i rätt plats
int error_code; //lagra senaste felkoden från servo

// Initiera med önskad baud rate
void init_USART(long baud_rate);

// Aktivera sändning (avaktivera mottagning)
void USART_transmit_mode();

// tvärtom
void USART_receive_mode();

void send_to_servo(unsigned char data);
// OBS: mottagning sker via en avbrott-vektor

// läs av senaste fel från reception_buffer
// int get_error_code();

// omvandla abosluta servovinkel till vinkeldata
// vinklar är (-150,150) grader
int angle_to_data(float angle);

// KUNGLIG FUNKTION! styr alla servo samtldigt
void write_to_all();

// styr en servo
void set_single_servo_2byte(unsigned char id, unsigned char register_name, int data);
void set_single_servo_1byte(unsigned char id, unsigned char register_name, unsigned char data);

void set_compliance(unsigned char id, unsigned char CW_margin, unsigned char CCW_margin, unsigned char CW_slope, unsigned char CCW_slope);

// omvandla grader (-150 - 150) till servos vinkeldata (0 - 1024)
void translate_leg_angle();

// hjälpfunktion för kvadrering
float SQ(float num);

// omvandla kartesiska koordinater till servovinklar hos ett ben
void set_pos_leg(leg_info* leg, float x, float y, float z);

// Initiera benen med sina initiala parametrar
void init_leg(float init_x, float init_y, float init_z);

#endif /* SERVO_CONTROL_H_ */