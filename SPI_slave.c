/*
* SPI_slave.c
*
*  Author:	Herman Molinder		hermo276@student.liu.se
*			Tore Landén			torla816@student.liu.se
*/

#include "SPI_Slave.h"
#include "WISH_Gait_Engine.h"
#include "WISH_autonomus.h"

//Initierar SPI Slave
void SPI_init(void)
{
	//spi pins on port b, MISO output, all other input
	DDRB = (1<<DDB6);
	// SPI enable, Slave
	SPCR = (1<<SPE)|(1<<SPIE);
}

//styrbeslut
volatile uint8_t control_decision;
void send_control_decision(uint8_t decision)
{
	control_decision = decision;
}


//initierar timer1
void timer1_init()
{
	//Timer prescaler = 8
	TCCR1B |= (1<<CS11);
	//Start timer
	TCNT1 = 0;
	//Enable overflow interrupt
	TIMSK1 |= (1<<TOIE1);
}

// Returnerar en "checksum" = type XOR data.
char check_creator(char type,char data)
{
	return type^data;
}

// check_decoder returnerar 1 om check == type XOR data, annars 0.
unsigned int check_decoder(char type, char data, char check)
{
	char is_check = type^data;
	if(is_check == check)
	return 1;
	else return 0;
}

/*
* DENNA SKRIVER STYR
*/
void SPI_transmit_update()
{
	if(control_decision != NOTHING_TO_SEND)
	{
		type_transmit = TYPE_CONTROL_DECISION;
		data_transmit = control_decision;
		control_decision = NOTHING_TO_SEND;
		check_transmit = check_creator(type_transmit, data_transmit);
	}
}

/*
* DENNA SKRIVER STYR
*
* Kontroll att mottagen data är ok!
* Samt uppdatering av räknare för data att skicka
*/
void SPI_control()
{
	if (check_decoder(type_recieved, data_recieved, check_recieved))
	{
		 signed char signed_temp = data_recieved;
		
		switch (type_recieved)
		{
			case 0x00:
			// Manuell eller autonom
			// 0x00 är man; 0xff är auto
			MODE = data_recieved;
			if(MODE == 0xff)
			{
				FRAME_RATE = AUTONOMUS_FRAME_RATE;
			}
			else
			{
				FRAME_RATE = MANUAL_FRAME_RATE;
			}
			break;
			
			case 0x01:
			//Manuver_Start();
			X_Step_Length_From_Bus = (float)signed_temp;
			break;

			case 0x02:
			//Manuver_Stop();
			Y_Step_Length_From_Bus = (float)signed_temp;
			break;
			
			case 0x03: // hastighet i x-leden
			Angular_Step_Length_From_Bus = signed_temp;
			//Set_Speed();
			break;
			
			case 0x04:
			Emergency_Stop();
			break;
			
			case 0x05:
			Error = data_recieved;
			Accumulated_Error += Error;
			
			break;
			case 0x06:
			Diff_Error = data_recieved;
			break;
			
			case 0x07:
			Object_Right = data_recieved;
			break;
			
			case 0x08:
			Object_Left = data_recieved;
			break;
			
			case 0x09:
			Front_Sensor = data_recieved;
			break;
			
			case 0x0A:
			Back_Sensor = data_recieved;
			break;
			
			case 0x0B:
			Object_Front = data_recieved;
			break;
			case 0x0C:
			Object_Back = data_recieved;
			break;
			case 0x0D:
			break;
			case 0x0E:
			break;
			case 0x0F:
			break;
			case 0x10:
			break;
			case 0x11:
			break;
			case 0x12:
			break;
			// und so weiter
			case 0x13:
			// sista typen som ska uppdateras
			
			case 0x21:
			// K_P = data_recieved;
			break;
			
			case 0x22:
			// K_D = data_recieved;
			break;

		}
	}
}


// Denna funktion hanterar inkommande och utgående data på bussen.
void SPI_transfer_update()
{
	switch(package_counter)
	{
		// type
		case 0:
		type_recieved = recieve_buffer;
		transmit_buffer = data_transmit;
		package_counter = 1;
		break;
		
		// data
		case 1:
		data_recieved = recieve_buffer;
		transmit_buffer = check_transmit;
		package_counter = 2;
		break;
		
		// check
		case 2:
		check_recieved = recieve_buffer;
		SPI_control();
		SPI_transmit_update();
		transmit_buffer = type_transmit;
		package_counter = 0;
	}
}

// Avbrottsvektorn SPI Serial Transfer Complete
ISR(SPI_STC_vect)
{
	recieve_buffer = SPDR;
	SPI_transfer_update();
	SPDR = transmit_buffer;
	// Reset timer1
	TCNT1 = 0;
}

// Nollställer package_counter.
ISR(TIMER1_OVF_vect)
{
	package_counter = 0;
}


// Byt håll mellan rotationer
// extra funktioner
// mellanslag -> initial läget

// Hjälpfunktioner som kortar ner SPI_control()
void Manuver_Start()
{
	switch(data_recieved)
	{
		case 0x00:
		Emergency_Stop();
		X_Step_Length = 0;
		Y_Step_Length = 0;
		Angular_Step_Length = 0;
		// 				X_Step_Length_temp = 0;
		// 				Y_Step_Length_temp = 0;
		// 				Angular_Step_Length_temp = 0;
		break;
		
		case 0x02:
		if (X_Step_Length < X_Speed)
		{
			X_Step_Length += 1;
		}
		else
		{
			X_Step_Length = X_Speed;
		}
		break;
		
		case 0x03:
		if (X_Step_Length > -X_Speed)
		{
			X_Step_Length -= 1;
		}
		else
		{
			X_Step_Length = -X_Speed;
		}
		break;
		
		case 0x04:
		if (X_Step_Length >= 30  || X_Step_Length <= -30)
		{
			if(Angular_Step_Length < 40)
			Angular_Step_Length += 1;
		}
		else if(Angular_Step_Length < 100)
		{
			Angular_Step_Length += 1;
		}
		break;
		
		case 0x05:
		//Begränsar vridningsvinkeln då x_step_length är större än 30
		//eller mindre än -30. Är till för att skydda benen från att slå ihop
		if (X_Step_Length >= 30  || X_Step_Length <= -30)
		{
			if(Angular_Step_Length > -40) //begränsning på -5 grader för angular_step_length, då ovanstående if-sats är uppfylld
			Angular_Step_Length -= 1; //Minskar vridningsvinkeln, dvs högersväng elliptisk bana. Minskar med -1 grad per knapptryck.
		}
		else if(Angular_Step_Length > -100) // då steglängden är större än if-satsen ovan är begränsningen -10 grader för angular_step_length
		{
			Angular_Step_Length -= 1;  //Minskar vridningsvinkeln, dvs högersväng elliptisk bana. Minskar med -1 grad per steg.
		}
		break;
		
		case 0x0a:
		if (Y_Step_Length > -30)
		{
			Y_Step_Length -= 1;
		}
		break;
		
		case 0x0b:
		if (Y_Step_Length < 30)
		{
			Y_Step_Length += 1;
		}
		break;
		
		default:
		break;
	}
}

void Manuver_Stop()
{
	switch(data_recieved)
	{
		case 0:
		// 				Emergency_Stop();
		// 				X_Step_Length = 0;
		// 				Y_Step_Length = 0;
		// 				Angular_Step_Length = 0;
		// 				X_Step_Length_temp = 0;
		// 				Y_Step_Length_temp = 0;
		// 				Angular_Step_Length_temp = 0;
		break;
		
		case 2:
		// Fall through
		case 3:
		X_Step_Length = 0;
		break;
		
		case 4:
		Angular_Step_Length = 0;
		break;
		
		case 5:
		Angular_Step_Length = 0;
		break;
		
		case 0x0a:
		// Fall through
		case 0x0b:
		Y_Step_Length = 0;
		break;
		
		default:
		break;
	}
}

void Set_Speed()
{
	switch(data_recieved)
	{
		case 0:
		X_Speed = 10;
		break;
		
		case 1:
		X_Speed = 20;
		break;
		
		case 2:
		X_Speed = 30;
		break;
		
		case  3:
		X_Speed = 40;
		break;
		
		default:
		break;
	}
}