/*
 * Styrmodul_WISH.c
 *
 * Created: 2014-03-24 20:57:14
 *  Author: Bohan
 */ 


#include <avr/io.h>
#include "WISH_Gait_Engine.h"

int main(void)
{
	init_USART(1000000); // 250000 när använder JTAG, 1000000 för real deal
	init_leg(0,160,65);
	sei(); // aktivera globalavbrott
	
	
	translate_leg_angle();
	
	write_to_all();




	_delay_ms(1000);



	for (double x = 0; x <= 70 ; x+=2)
	{
		
		set_pos_leg(&left_front, x, 0 ,65);
		set_pos_leg(&left_middle, x, 0, 65);
		set_pos_leg(&left_back, x, 0, 65);
		
		set_pos_leg(&right_front, x, 0, 65);
		set_pos_leg(&right_middle, x, 0, 65);
		set_pos_leg(&right_back, x, 0, 65);
		
		translate_leg_angle();
		
		_delay_ms(10);
		
		write_to_all();
		
	}
	
	
	for (double x = 70; x >= -70 ; x-=2)
	{
		
		set_pos_leg(&left_front, x, 0 ,65);
		set_pos_leg(&left_middle, x, 0, 65);
		set_pos_leg(&left_back, x, 0, 65);
		
		set_pos_leg(&right_front, x, 0, 65);
		set_pos_leg(&right_middle, x, 0, 65);
		set_pos_leg(&right_back, x, 0, 65);
		
		translate_leg_angle();
		
		_delay_ms(10);
		
		write_to_all();
	}
	
	for (double x = -70; x <= 0 ; x+=2)
	{
		
		set_pos_leg(&left_front, x, 0 ,65);
		set_pos_leg(&left_middle, x, 0, 65);
		set_pos_leg(&left_back, x, 0, 65);
		
		set_pos_leg(&right_front, x, 0, 65);
		set_pos_leg(&right_middle, x, 0, 65);
		set_pos_leg(&right_back, x, 0, 65);
		
		translate_leg_angle();
		
		_delay_ms(10);
		
		write_to_all();
		
	}

	_delay_ms(200);
	
	for (double i = 0; i <= 35 ; i+=2)
	{
		
		set_pos_leg(&left_front, 0 , i, 65);
		set_pos_leg(&left_middle, 0, i, 65);
		set_pos_leg(&left_back, 0, i, 65);
		
		set_pos_leg(&right_front, 0, i, 65);
		set_pos_leg(&right_middle, 0, i, 65);
		set_pos_leg(&right_back, 0, i, 65);
				
		
		translate_leg_angle();
		
		_delay_ms(10);
		
		write_to_all();
		
	}
	
	// _delay_ms(100);
	
	for (double i = 35; i >= -35 ; i-=2)
	{
		
		set_pos_leg(&left_front, 0 , i, 65);
		set_pos_leg(&left_middle, 0, i, 65);
		set_pos_leg(&left_back, 0, i, 65);
		
		set_pos_leg(&right_front, 0, i, 65);
		set_pos_leg(&right_middle, 0, i, 65);
		set_pos_leg(&right_back, 0, i, 65);
		translate_leg_angle();
		
		_delay_ms(10);
		
		write_to_all();
	}
	
	for (double i = -35; i <= 0 ; i+=2)
	{
		
		set_pos_leg(&left_front, 0 , i, 65);
		set_pos_leg(&left_middle, 0, i, 65);
		set_pos_leg(&left_back, 0, i, 65);
		
		set_pos_leg(&right_front, 0, i, 65);
		set_pos_leg(&right_middle, 0, i, 65);
		set_pos_leg(&right_back, 0, i, 65);
		
		translate_leg_angle();
		
		_delay_ms(10);
		
		write_to_all();
		
	}

		_delay_ms(1000);
		
		for (double i = 65; i <= 150 ; i+=2)
		{
			
			set_pos_leg(&left_front, 0 , 0, i);
			set_pos_leg(&left_middle, 0, 0, i);
			set_pos_leg(&left_back, 0, 0, i);
			
			set_pos_leg(&right_front, 0, 0, i);
			set_pos_leg(&right_middle, 0, 0, i);
			set_pos_leg(&right_back, 0, 0, i);
			translate_leg_angle();
			
			_delay_ms(10);
			
			write_to_all();
		}
		
		_delay_ms(1000);
		
		for (double i = 150; i >= 65 ; i-=2)
		{
			
			set_pos_leg(&left_front, 0 , 0, i);
			set_pos_leg(&left_middle, 0, 0, i);
			set_pos_leg(&left_back, 0, 0, i);
			
			set_pos_leg(&right_front, 0, 0, i);
			set_pos_leg(&right_middle, 0, 0, i);
			set_pos_leg(&right_back, 0, 0, i);
			translate_leg_angle();
			
			_delay_ms(10);
			
			write_to_all();
		}
}