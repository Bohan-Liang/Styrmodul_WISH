/*
 * Hex_Kayboard.c
 *
 * Created: 2014-04-01 10:50:58
 *  Author: Bohan
 */
#include "Hex_Keyboard.h"

void init_hex()
{
	//aktivera int0 f�r avbrott
	EIMSK = 0x01;
	EICRA = 0b00000011;
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
		if(X_Step_Length > -40) // Begr�nsar max hastigeheten f�r roboten. Denna gr�ns �r framtestad s� att hexapoden g�r mjukt.
		X_Step_Length -= 5; // s�nker stegl�ngden med 5 mm
		break;

		case 0b00100000:
		if (X_Step_Length >= 35  || X_Step_Length <= -35) //Begr�nsar vridningsvinkeln d� x_step_length �r st�rre �n 45
                                                            //eller mindre �n -45. �r till f�r att skydda benen fr�n att sl� ihop
		{
			if(Angular_Step_Length > -5) //begr�nsning p� -5 grader f�r angular_step_length, d� ovanst�ende if-sats �r uppfylld
			Angular_Step_Length -= 1; //Minskar vridningsvinkeln, dvs h�gersv�ng elliptisk bana. Minskar med -1 grad per knapptryck.
		}
		else if(Angular_Step_Length > -10) // d� stegl�ngden �r st�rre �n if-satsen ovan �r begr�nsningen -10 grader f�r angular_step_length
		Angular_Step_Length -= 1;  //Minskar vridningsvinkeln, dvs h�gersv�ng elliptisk bana. Minskar med -1 grad per steg.
		break;

		case 0b00101000:
		X_Step_Length = 0;
		Y_Step_Length = 0;  //stannar roboten
		Angular_Step_Length = 0;
		break;

		case 0b00110000:
		if (X_Step_Length >= 35 || X_Step_Length <= -35) //Begr�nsar vridningsvinkeln d� x_step_length �r st�rre �n 45
                                                         //eller mindre �n -45. �r till f�r att skydda benen fr�n att sl� ihop
		{
			if(Angular_Step_Length < 5) //begr�nsning p� 5 grader f�r angular_step_length, d� ovanst�ende if-sats �r uppfylld
			Angular_Step_Length += 1;//�kar vridningsvinkeln, dvs v�nstersv�ng elliptisk bana. �kar med 1 grad per knapptryck.
		}
		else if(Angular_Step_Length < 10) // d� stegl�ngden �r mindre �n if-satsen ovan �r begr�nsningen 10 grader f�r angular_step_length
		Angular_Step_Length += 1; //�kar vridningsvinkeln, dvs v�nstersv�ng elliptisk bana. �kar med 1 grad per steg.
		break;

		case 0b00111000:
		if(Y_Step_Length < 30) //begr�nsning f�r att g�ngen ska bli mjuk.
		Y_Step_Length += 3; // strafe �t v�nster. �kar stegl�ngden f�r strafe med 3 mm per knapptryck.
		break;

		case 0b01000000:
		if(X_Step_Length < 40) // Begr�nsar max stegl�ngden f�r roboten. Denna gr�ns �r testad s� att hexapoden g�r mjukt.
		X_Step_Length += 5; // �kar stegl�ngden med 5 mm
		break;

		case 0b01001000:
		if(Y_Step_Length > -30) //begr�nsning f�r att g�ngen ska bli mjuk.
		Y_Step_Length -= 3; // strafe �t h�ger. minskar stegl�ngden f�r strafe med 3 mm per knapptryck.
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

void translate_PIN_climb()
{
	hex_input &= 0b01111000;
	
	switch(hex_input)
	{
		case 0b00010000:
		if(X_Step_Length > -40) // Begr�nsar max hastigeheten f�r roboten. Denna gr�ns �r framtestad s� att hexapoden g�r mjukt.
		X_Step_Length -= 5; // s�nker stegl�ngden med 5 mm
		break;

		case 0b00100000:
		if (X_Step_Length >= 25 || X_Step_Length <= -25) //Begr�nsar vridningsvinkeln d� x_step_length �r st�rre �n 45
		//eller mindre �n -45. �r till f�r att skydda benen fr�n att sl� ihop
		{
			if(Angular_Step_Length > -5) //begr�nsning p� -5 grader f�r angular_step_length, d� ovanst�ende if-sats �r uppfylld
			Angular_Step_Length -= 1; //Minskar vridningsvinkeln, dvs h�gersv�ng elliptisk bana. Minskar med -1 grad per knapptryck.
		}
		else if(Angular_Step_Length > -10) // d� stegl�ngden �r st�rre �n if-satsen ovan �r begr�nsningen -10 grader f�r angular_step_length
		Angular_Step_Length -= 1;  //Minskar vridningsvinkeln, dvs h�gersv�ng elliptisk bana. Minskar med -1 grad per steg.
		break;

		case 0b00101000:
		X_Step_Length = 0;
		Y_Step_Length = 0;  //stannar roboten
		Angular_Step_Length = 0;
		break;

		case 0b00110000:
		if (X_Step_Length >= 25 || X_Step_Length <= -25) //Begr�nsar vridningsvinkeln d� x_step_length �r st�rre �n 45
		//eller mindre �n -45. �r till f�r att skydda benen fr�n att sl� ihop
		{
			if(Angular_Step_Length < 5) //begr�nsning p� 5 grader f�r angular_step_length, d� ovanst�ende if-sats �r uppfylld
			Angular_Step_Length += 1;//�kar vridningsvinkeln, dvs v�nstersv�ng elliptisk bana. �kar med 1 grad per knapptryck.
		}
		else if(Angular_Step_Length < 10) // d� stegl�ngden �r mindre �n if-satsen ovan �r begr�nsningen 10 grader f�r angular_step_length
		Angular_Step_Length += 1; //�kar vridningsvinkeln, dvs v�nstersv�ng elliptisk bana. �kar med 1 grad per steg.
		break;

		case 0b00111000:
		if(Y_Step_Length < 30) //begr�nsning f�r att g�ngen ska bli mjuk.
		Y_Step_Length += 3; // strafe �t v�nster. �kar stegl�ngden f�r strafe med 3 mm per knapptryck.
		break;

		case 0b01000000:
		if(X_Step_Length < 40) // Begr�nsar max stegl�ngden f�r roboten. Denna gr�ns �r testad s� att hexapoden g�r mjukt.
		X_Step_Length += 5; // �kar stegl�ngden med 5 mm
		break;

		case 0b01001000:
		if(Y_Step_Length > -30) //begr�nsning f�r att g�ngen ska bli mjuk.
		Y_Step_Length -= 3; // strafe �t h�ger. minskar stegl�ngden f�r strafe med 3 mm per knapptryck.
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
