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
#include "WISH_Regulator.h"
#include "SPI_Slave.h"
// #include "Hex_Keyboard.h"

int main(void)
{
	init_USART(1000000); // 250000 när använder JTAG, 1000000 för real deal
	init_leg(0,150,90); //obs z = 85 vid vanlig gång.
	// init_leg(0,150,140); // för att testa klättra
	
	//init_hex();
	
	SPI_init();
	timer1_init();
	
	sei(); // aktivera globalavbrott
	
	translate_leg_angle();
	
	write_to_all();

	//_delay_ms(1000);
	
	init_gait(30);
	
	while(1)
	{
		//translate_PIN();
// 		X_Step_Length = X_Step_Length_temp;
// 		Y_Step_Length = Y_Step_Length_temp;
// 		Angular_Step_Length = Angular_Step_Length_temp;
		SPI_control();
		test_regulator(1);
		tripod_gait();
	}

}
