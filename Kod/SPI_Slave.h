/*
 * SPI_Slave.h
 *
 *  Author:	Herman Molinder		hermo276@student.liu.se
 *			Tore Land�n			torla816@student.liu.se
 */ 


#ifndef SPI_SLAVE_H_
#define SPI_SLAVE_H_

#include <avr/io.h>
#include <avr/interrupt.h>


//skicka styrbeslut
#define NOTHING_TO_SEND 0xff
#define TYPE_CONTROL_DECISION 32
void send_control_decision(uint8_t decision);

//Globala variabler
volatile int	package_counter;	// H�ller reda p� vilken typ av data som f�rv�ntas p� bussen. F�rv�ntar ny s�ndning d� == 0
volatile char	transmit_buffer;	// Data som ska skickas
volatile char	recieve_buffer;		// Data som tas emot
// Mottagen data
volatile char	type_recieved;		// SPI
volatile char	data_recieved;		// SPI
volatile char	check_recieved;		// SPI
// Data att s�nda
volatile char	type_transmit;		// SPI
volatile char	data_transmit;		// SPI
volatile char	check_transmit;		// SPI

volatile float X_Step_Length_From_Bus;
volatile float Y_Step_Length_From_Bus;
volatile int Angular_Step_Length_From_Bus;

volatile char MODE;

void SPI_init(void); //Initierar SPI Slave

//initierar timer1
void timer1_init();

// Returnerar en "checksum" = type XOR data.
char check_creator(char type,char data);

// check_decoder returnerar 1 om check == type XOR data, annars 0.
unsigned int check_decoder(char type, char data, char check);

void SPI_transmit_update();

/* 
 * DENNA SKRIVER STYR
 *
 * Kontroll att mottagen data �r ok!
 * Samt uppdatering av r�knare f�r data att skicka
 */
void SPI_control();

// Hj�lpfunktioner som kortar ner SPI_control()
void Manuver_Start();
void Manuver_Stop();
void Set_Speed();


// Denna funktion hanterar inkommande och utg�ende data p� bussen.
void SPI_transfer_update();





#endif /* SPI_SLAVE_H_ */