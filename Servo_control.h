/*
 * Servo_control.h
 *
 * Created: 2014-03-24 21:01:15
 *  Author: Bohan
 */ 



/*
The main data types in C are

char: 8-bit integer in AVR
int: 16-bi i i AVR bit integer in AVR
long int: 32-bit integer in AVR

The above data types can be modified by keyword �
unsigned.

char a; // range of values for a: -128, �, 0, �, 127
unsigned char b; // range of values for b: 0, 1, 2, �, 255
unsigned long int c; // range of values for c: 0, 1, �, 232 - 1
*/
#ifndef SERVO_CONTROL_H_
#define SERVO_CONTROL_H_

#define F_CPU 16000000UL // Ange CPU frekvens

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

// Hj�lpfunktioner f�r USART
// RXD0 p� PortD0
// TXD0 p� PortD1

/* AX-12 st�djer "half duplex"
   Bara en linje anv�nds, dvs
   s�ndning och mottagning kan EJ ske samtidigt
 */

/******************
 *
 *	OBS!VIKTIG!
 *
 *	koppla ihop TX0 och RX0.(Pin 14 och 15)
 *
 ******************/
// Initiera med �nskad baud rate
void init_USART(long baud_rate);

// Aktivera s�ndning (avaktivera mottagning)
void USART_transmit_mode();

// tv�rtom
void USART_receive_mode();

void send_to_servo(unsigned char data);
// OBS: mottagning sker via en avbrott-vektor

// l�s av senaste fel fr�n reception_buffer
int get_error_code();

// omvandla abosluta servovinkel till vinkeldata
// vinklar �r (-150,150) grader
int angle_to_data(double angle);

// KUNGLIG FUNKTION! styr alla servo samtldigt
void write_to_all();

// omvandla grader (-150 - 150) till servos vinkeldata (0 - 1024)
void translate_leg_angle();

// hj�lpfunktion f�r kvadrering
double SQ(double num);

// omvandla kartesiska koordinater till servovinklar hos ett ben
void set_pos_leg(leg_info* leg, double x, double y, double z);

// Initiera bennummer
void init_leg(double init_x, double init_y, double init_z);







//void set_ID(int id);
//void servo_test(double angle); // testprogram som inte ska anv�ndas till PhantomX!
//void servo_test2(double angle);
//void reset_factory();
// Styr samtliga servo samtidigt
//OBS: endast med samma typ av kommando och samma instruktionsl�ngd
// Implementation ska diskuteras.
// void sync_write();


#endif /* SERVO_CONTROL_H_ */