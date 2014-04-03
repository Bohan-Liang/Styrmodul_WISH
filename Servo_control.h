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

The above data types can be modified by keyword ‘
unsigned.

char a; // range of values for a: -128, …, 0, …, 127
unsigned char b; // range of values for b: 0, 1, 2, …, 255
unsigned long int c; // range of values for c: 0, 1, …, 232 - 1
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

// Hjälpfunktioner för USART
// RXD0 på PortD0
// TXD0 på PortD1

/* AX-12 stödjer "half duplex"
   Bara en linje används, dvs
   sändning och mottagning kan EJ ske samtidigt
 */

/******************
 *
 *	OBS!VIKTIG!
 *
 *	koppla ihop TX0 och RX0.(Pin 14 och 15)
 *
 ******************/
// Initiera med önskad baud rate
void init_USART(long baud_rate);

// Aktivera sändning (avaktivera mottagning)
void USART_transmit_mode();

// tvärtom
void USART_receive_mode();

void send_to_servo(unsigned char data);
// OBS: mottagning sker via en avbrott-vektor

// läs av senaste fel från reception_buffer
int get_error_code();

// omvandla abosluta servovinkel till vinkeldata
// vinklar är (-150,150) grader
int angle_to_data(double angle);

// KUNGLIG FUNKTION! styr alla servo samtldigt
void write_to_all();

// omvandla grader (-150 - 150) till servos vinkeldata (0 - 1024)
void translate_leg_angle();

// hjälpfunktion för kvadrering
double SQ(double num);

// omvandla kartesiska koordinater till servovinklar hos ett ben
void set_pos_leg(leg_info* leg, double x, double y, double z);

// Initiera bennummer
void init_leg(double init_x, double init_y, double init_z);







//void set_ID(int id);
//void servo_test(double angle); // testprogram som inte ska användas till PhantomX!
//void servo_test2(double angle);
//void reset_factory();
// Styr samtliga servo samtidigt
//OBS: endast med samma typ av kommando och samma instruktionslängd
// Implementation ska diskuteras.
// void sync_write();


#endif /* SERVO_CONTROL_H_ */