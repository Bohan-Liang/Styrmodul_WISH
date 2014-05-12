/*
* Styrmodul_WISH.c
*
* Created: 2014-03-24 20:57:14
*  Author: Bohan
*/

// Tablera SIN och COS
// Taylorutveckla ACOS
// Ändra samtliga "float" till "float"

#include <avr/io.h>
#include "WISH_Gait_Engine.h"
#include "WISH_autonomus.h"
#include "SPI_Slave.h"
// #include "Hex_Keyboard.h"

#define Compliance 64

int main(void)
{
	init_USART(1000000);
	_delay_ms(100);
	
	SPI_init();
	timer1_init();

	
	sei(); // aktivera globalavbrott
	
	init_leg(0,150,90); //obs z = 85 vid vanlig gång.
	// init_leg(0,150,140); // för att testa klättra
	
	// Sätter compliance för alla servo förutom COXA leder
	_delay_ms(500);
	
 	set_compliance(3, 1, 1, Compliance, Compliance);
 	_delay_us(1200);
	set_compliance(4, 1, 1, Compliance, Compliance);
 	_delay_us(1200);
 	set_compliance(5, 1, 1, Compliance, Compliance);
	_delay_us(1200);
	set_compliance(6, 1, 1, Compliance, Compliance);
	_delay_us(1200);
	set_compliance(9, 1, 1, Compliance, Compliance);
	_delay_us(1200);
	set_compliance(10, 1, 1, Compliance, Compliance);
	_delay_us(1200);
	set_compliance(11, 1, 1, Compliance, Compliance);
	_delay_us(1200);
	set_compliance(12, 1, 1, Compliance, Compliance);
	_delay_us(1200);
	set_compliance(15, 1, 1, Compliance, Compliance);
	_delay_us(1200);
	set_compliance(16, 1, 1, Compliance, Compliance);
	_delay_us(1200);
	set_compliance(17, 1, 1, Compliance, Compliance);
	_delay_us(1200);
	set_compliance(18, 1, 1, Compliance, Compliance);
	_delay_us(1200);
	
	_delay_ms(500);
	
	translate_leg_angle();
	
	write_to_all();
	Direction = FORWARD;

	//_delay_ms(1000);
	
	init_gait(MANUAL_FRAME_RATE);
	
	init_autonomous_operation();

	
	while(1)
	{
		
		if (MODE == 0xff)
		{
			autonomous_operation();
		}
		else
		{
			X_Step_Length = X_Step_Length_From_Bus;
			Y_Step_Length = Y_Step_Length_From_Bus;
			Angular_Step_Length = Angular_Step_Length_From_Bus;
		}
		tripod_gait();
	}
}
