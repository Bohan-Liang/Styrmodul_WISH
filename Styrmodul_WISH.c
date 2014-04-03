/*
* Styrmodul_WISH.c
*
* Created: 2014-03-24 20:57:14
*  Author: Bohan
*/


#include <avr/io.h>
#include "WISH_Gait_Engine.h"
#include "Hex_Keyboard.h"

int main(void)
{
	init_USART(1000000); // 250000 när använder JTAG, 1000000 för real deal
	init_leg(0,150,85); //obs z = 85 vid vanlig gång.
	
	init_hex();
	sei(); // aktivera globalavbrott
	
	
	translate_leg_angle();
	
	write_to_all();

	_delay_ms(1000);
	
	init_gait(20);
	
	// X_Step_Length = 80;
	// Y_Step_Length = 40;
	// Angular_Step_Length = 5;
	
	
	while(1)
	{
		translate_PIN();
		tripod_gait();
	}
	
	X_Step_Length = 0;
	Y_Step_Length = 0;
	Angular_Step_Length = 0;
	tripod_gait();
	

/*

	for (double i = 0; i <= 20 ; i+=1)
	{
		
		body_rotate(&left_front,i,0,0);
		body_rotate(&left_middle,i,0,0);
		body_rotate(&left_back,i,0,0);
		body_rotate(&right_front,i,0,0);
		body_rotate(&right_middle,i,0,0);
		body_rotate(&right_back,i,0,0);
		
		translate_leg_angle();
		
		_delay_ms(20);
		
		write_to_all();
		
	}
	
	
	for (double i = 20; i >= -20 ; i-=1)
	{
		
		body_rotate(&left_front,i,0,0);
		body_rotate(&left_middle,i,0,0);
		body_rotate(&left_back,i,0,0);
		body_rotate(&right_front,i,0,0);
		body_rotate(&right_middle,i,0,0);
		body_rotate(&right_back,i,0,0);
		
		translate_leg_angle();
		
		_delay_ms(20);
		
		write_to_all();
	}
	
	for (double i = -20; i <= 0 ; i+=1)
	{
		
		body_rotate(&left_front,i,0,0);
		body_rotate(&left_middle,i,0,0);
		body_rotate(&left_back,i,0,0);
		body_rotate(&right_front,i,0,0);
		body_rotate(&right_middle,i,0,0);
		body_rotate(&right_back,i,0,0);
		
		translate_leg_angle();
		
		_delay_ms(20);
		
		write_to_all();
		
	}

	//-----------------------------------------

	for (double i = 0; i <= 20 ; i+=1)
	{
		
		body_rotate(&left_front,0,i,0);
		body_rotate(&left_middle,0,i,0);
		body_rotate(&left_back,0,i,0);
		body_rotate(&right_front,0,i,0);
		body_rotate(&right_middle,0,i,0);
		body_rotate(&right_back,0,i,0);
		
		translate_leg_angle();
		
		_delay_ms(20);
		
		write_to_all();
		
	}


	for (double i = 20; i >= -20 ; i-=1)
	{
		
		body_rotate(&left_front,0,i,0);
		body_rotate(&left_middle,0,i,0);
		body_rotate(&left_back,0,i,0);
		body_rotate(&right_front,0,i,0);
		body_rotate(&right_middle,0,i,0);
		body_rotate(&right_back,0,i,0);
		
		translate_leg_angle();
		
		_delay_ms(20);
		
		write_to_all();
	}

	for (double i = -20; i <= 0 ; i+=1)
	{
		
		body_rotate(&left_front,0,i,0);
		body_rotate(&left_middle,0,i,0);
		body_rotate(&left_back,0,i,0);
		body_rotate(&right_front,0,i,0);
		body_rotate(&right_middle,0,i,0);
		body_rotate(&right_back,0,i,0);
		
		translate_leg_angle();
		
		_delay_ms(20);
		
		write_to_all();
		
	}

	//-----------------------------------------

	for (double i = 0; i <= 20 ; i+=1)
	{
		
		body_rotate(&left_front,0,0,i);
		body_rotate(&left_middle,0,0,i);
		body_rotate(&left_back,0,0,i);
		body_rotate(&right_front,0,0,i);
		body_rotate(&right_middle,0,0,i);
		body_rotate(&right_back,0,0,i);
		
		translate_leg_angle();
		
		_delay_ms(20);
		
		write_to_all();
		
	}


	for (double i = 20; i >= -20 ; i-=1)
	{
		body_rotate(&left_front,0,0,i);
		body_rotate(&left_middle,0,0,i);
		body_rotate(&left_back,0,0,i);
		body_rotate(&right_front,0,0,i);
		body_rotate(&right_middle,0,0,i);
		body_rotate(&right_back,0,0,i);
		
		translate_leg_angle();
		
		_delay_ms(20);
		
		write_to_all();
	}

	for (double i = -20; i <= 0 ; i+=1)
	{
		
		body_rotate(&left_front,0,0,i);
		body_rotate(&left_middle,0,0,i);
		body_rotate(&left_back,0,0,i);
		body_rotate(&right_front,0,0,i);
		body_rotate(&right_middle,0,0,i);
		body_rotate(&right_back,0,0,i);
		
		translate_leg_angle();
		
		_delay_ms(20);
		
		write_to_all();
		
	}

	//-----------------------------------------
	
*/
}
