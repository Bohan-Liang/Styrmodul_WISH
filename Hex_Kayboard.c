/*
 * Hex_Kayboard.c
 *
 * Created: 2014-04-01 10:50:58
 *  Author: Bohan
 */ 
#include "Hex_Keyboard.h"

void init_hex()
{
	//aktivera int0 för avbrott
	EIMSK = 0x01;
	EICRA = 0b00000011;
	// DDRA = 0;
	Y_Pitch = 0;
	X_Roll = 0;
	Z_Yaw = 0;
}

ISR(INT0_vect)
{
	hex_input = PINA; 
}

void translate_PIN()
{
	hex_input &= 0b01111000;
	
	switch(hex_input)
	{
		case 0b00010000:
		if(X_Step_Length > -50) 
		X_Step_Length -= 5; // sänker steglängden med 5 mm
		break;
		
		case 0b00100000:
		if (X_Step_Length >= 45  || X_Step_Length <= -45)
		{
			if(Angular_Step_Length > -5)
			Angular_Step_Length -= 1;
		}
		else if(Angular_Step_Length > -10)
		Angular_Step_Length -= 1;  //ökar vridningsvinkeln, dvs högersväng elliptisk bana. ökar med 5 mm
		break;
		
		case 0b00101000:
		X_Step_Length = 0;
		Y_Step_Length = 0;  //stannar roboten
		Angular_Step_Length = 0;
		break;
		
		case 0b00110000:
		if (X_Step_Length >= 45 || X_Step_Length <= -45)
		{
			if(Angular_Step_Length < 5)
			Angular_Step_Length += 1;
		}
		else if(Angular_Step_Length < 10)
		Angular_Step_Length += 1; //minskar vridningsvinkeln, dvs vänstersväng elliptisk bana. minskar med 5 mm
		break;
		
		case 0b00111000:
		if(Y_Step_Length < 30)
		Y_Step_Length += 3; // sänker steglängden med 5 mm
		break;
		
		case 0b01000000:
		if(X_Step_Length < 50)
		X_Step_Length += 5; // sänker steglängden med 5 mm
		break;
		
		case 0b01001000:
		if(Y_Step_Length > -30)
		Y_Step_Length -= 3; // sänker steglängden med 5 mm
		break;
		
		case 0b00001000:
		Y_Pitch += 1;
		break;
		
		case 0b01010000:
		Y_Pitch -= 1;
		break;
		
		case 0b01110000:
		Y_Pitch = 0;
		break;
		
		case 0b00011000:
		X_Roll += 1;
		break;
		
		case 0b01011000:
		X_Roll -= 1;
		break;
		
		case 0b01111000:
		X_Roll = 0;
		break;
		
		
		default:
		break;	
	}
	
	hex_input &= 0;
}