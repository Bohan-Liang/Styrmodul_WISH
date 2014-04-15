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
		if(X_Step_Length > -40) // Begränsar max hastigeheten för roboten. Denna gräns är framtestad så att hexapoden går mjukt.
		X_Step_Length -= 5; // sänker steglängden med 5 mm
		break;

		case 0b00100000:
		if (X_Step_Length >= 35  || X_Step_Length <= -35) //Begränsar vridningsvinkeln då x_step_length är större än 45
                                                            //eller mindre än -45. Är till för att skydda benen från att slå ihop
		{
			if(Angular_Step_Length > -5) //begränsning på -5 grader för angular_step_length, då ovanstående if-sats är uppfylld
			Angular_Step_Length -= 1; //Minskar vridningsvinkeln, dvs högersväng elliptisk bana. Minskar med -1 grad per knapptryck.
		}
		else if(Angular_Step_Length > -10) // då steglängden är större än if-satsen ovan är begränsningen -10 grader för angular_step_length
		Angular_Step_Length -= 1;  //Minskar vridningsvinkeln, dvs högersväng elliptisk bana. Minskar med -1 grad per steg.
		break;

		case 0b00101000:
		X_Step_Length = 0;
		Y_Step_Length = 0;  //stannar roboten
		Angular_Step_Length = 0;
		break;

		case 0b00110000:
		if (X_Step_Length >= 35 || X_Step_Length <= -35) //Begränsar vridningsvinkeln då x_step_length är större än 45
                                                         //eller mindre än -45. Är till för att skydda benen från att slå ihop
		{
			if(Angular_Step_Length < 5) //begränsning på 5 grader för angular_step_length, då ovanstående if-sats är uppfylld
			Angular_Step_Length += 1;//Ökar vridningsvinkeln, dvs vänstersväng elliptisk bana. ökar med 1 grad per knapptryck.
		}
		else if(Angular_Step_Length < 10) // då steglängden är mindre än if-satsen ovan är begränsningen 10 grader för angular_step_length
		Angular_Step_Length += 1; //Ökar vridningsvinkeln, dvs vänstersväng elliptisk bana. ökar med 1 grad per steg.
		break;

		case 0b00111000:
		if(Y_Step_Length < 30) //begränsning för att gången ska bli mjuk.
		Y_Step_Length += 3; // strafe åt vänster. Ökar steglängden för strafe med 3 mm per knapptryck.
		break;

		case 0b01000000:
		if(X_Step_Length < 40) // Begränsar max steglängden för roboten. Denna gräns är testad så att hexapoden går mjukt.
		X_Step_Length += 5; // Ökar steglängden med 5 mm
		break;

		case 0b01001000:
		if(Y_Step_Length > -30) //begränsning för att gången ska bli mjuk.
		Y_Step_Length -= 3; // strafe åt höger. minskar steglängden för strafe med 3 mm per knapptryck.
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
		if(X_Step_Length > -40) // Begränsar max hastigeheten för roboten. Denna gräns är framtestad så att hexapoden går mjukt.
		X_Step_Length -= 5; // sänker steglängden med 5 mm
		break;

		case 0b00100000:
		if (X_Step_Length >= 25 || X_Step_Length <= -25) //Begränsar vridningsvinkeln då x_step_length är större än 45
		//eller mindre än -45. Är till för att skydda benen från att slå ihop
		{
			if(Angular_Step_Length > -5) //begränsning på -5 grader för angular_step_length, då ovanstående if-sats är uppfylld
			Angular_Step_Length -= 1; //Minskar vridningsvinkeln, dvs högersväng elliptisk bana. Minskar med -1 grad per knapptryck.
		}
		else if(Angular_Step_Length > -10) // då steglängden är större än if-satsen ovan är begränsningen -10 grader för angular_step_length
		Angular_Step_Length -= 1;  //Minskar vridningsvinkeln, dvs högersväng elliptisk bana. Minskar med -1 grad per steg.
		break;

		case 0b00101000:
		X_Step_Length = 0;
		Y_Step_Length = 0;  //stannar roboten
		Angular_Step_Length = 0;
		break;

		case 0b00110000:
		if (X_Step_Length >= 25 || X_Step_Length <= -25) //Begränsar vridningsvinkeln då x_step_length är större än 45
		//eller mindre än -45. Är till för att skydda benen från att slå ihop
		{
			if(Angular_Step_Length < 5) //begränsning på 5 grader för angular_step_length, då ovanstående if-sats är uppfylld
			Angular_Step_Length += 1;//Ökar vridningsvinkeln, dvs vänstersväng elliptisk bana. ökar med 1 grad per knapptryck.
		}
		else if(Angular_Step_Length < 10) // då steglängden är mindre än if-satsen ovan är begränsningen 10 grader för angular_step_length
		Angular_Step_Length += 1; //Ökar vridningsvinkeln, dvs vänstersväng elliptisk bana. ökar med 1 grad per steg.
		break;

		case 0b00111000:
		if(Y_Step_Length < 30) //begränsning för att gången ska bli mjuk.
		Y_Step_Length += 3; // strafe åt vänster. Ökar steglängden för strafe med 3 mm per knapptryck.
		break;

		case 0b01000000:
		if(X_Step_Length < 40) // Begränsar max steglängden för roboten. Denna gräns är testad så att hexapoden går mjukt.
		X_Step_Length += 5; // Ökar steglängden med 5 mm
		break;

		case 0b01001000:
		if(Y_Step_Length > -30) //begränsning för att gången ska bli mjuk.
		Y_Step_Length -= 3; // strafe åt höger. minskar steglängden för strafe med 3 mm per knapptryck.
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
