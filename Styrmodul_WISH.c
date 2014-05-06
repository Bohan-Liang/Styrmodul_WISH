/*
* Styrmodul_WISH.c
*
* Created: 2014-03-24 20:57:14
*  Author: Bohan
*/

// Tablera SIN och COS
// Taylorutveckla ACOS
// �ndra samtliga "float" till "float"

#include <avr/io.h>
#include "WISH_Gait_Engine.h"
#include "WISH_Regulator.h"
#include "SPI_Slave.h"
// #include "Hex_Keyboard.h"

int main(void)
{
	init_USART(1000000); // 250000 n�r anv�nder JTAG, 1000000 f�r real deal
	init_leg(0,150,90); //obs z = 85 vid vanlig g�ng.
	// init_leg(0,150,140); // f�r att testa kl�ttra
	
	//init_hex();
	
	SPI_init();
	timer1_init();
	init_regulator();
	
	sei(); // aktivera globalavbrott
	
	translate_leg_angle();
	
	write_to_all();
	Direction = FORWARD;

	//_delay_ms(1000);
	
	init_gait(24);
	
	while(1)
	{
		
		if (MODE == 0xff)
		{
			PID_regulator();
		}
		
		tripod_gait();
	}
	
	
	
}
