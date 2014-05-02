/*
* WISH_Gait_Engine.c
*
* Created: 2014-03-28 21:10:09
*  Author: Bohan
*/

#include "WISH_Gait_Engine.h"

// Initiera gångstilen
void init_gait(int frame_rate)
{
	FRAME_RATE = frame_rate; // Högre frame rate leder till mjukare rörelse men kan gör roboten långsam
	
	// Gör att första steg hamnar i mitten så att roboten inte gångar bakåt vid start
	Frame_Counter = frame_rate/2;
	
	// Beräkna tabellerade SIN och COS
	
	for(volatile int i = 0; i <= 400; i++)
	{
		// -20,0 till 0 sitter på index 0 till 200
		// 0 till 20,0 sitter på index 200 till 400
		SIN_TABLE[i] = sin(PI*(float)(i-200)/1800);

		COS_TABLE[i] = cos(PI*(float)(i-200)/1800);
	}
	
	//tabellerad frame_factor
	for (volatile int i = 0; i < frame_rate; i++)
	{
		COS_FRAME_FACTOR[i] = cos(PI*i/frame_rate);
		SIN_FRAME_FACTOR[i] = sin(PI*i/frame_rate);
	}
	
	// Börja med stillastående
	X_Step_Length = 0;
	Y_Step_Length = 0;
	Angular_Step_Length = 0;
	Body_Height_Adjust = 0;
	// X_Speed = 0;
	
	// Tripod gait, den ena "tripod" lyfter och för framåt
	left_front.Gait_State = TAKE_OFF_AND_LAND;
	left_back.Gait_State = TAKE_OFF_AND_LAND;
	right_middle.Gait_State = TAKE_OFF_AND_LAND;
	
	// den andra "tripod" står på marken och trycker bakåt så roboten förs framåt
	right_front.Gait_State = SWIM_BACK;
	right_back.Gait_State = SWIM_BACK;
	left_middle.Gait_State = SWIM_BACK;
}

void Emergency_Stop()
{
	set_pos_leg(&left_front, 0, 0, 0);
	set_pos_leg(&left_middle, 0, 0, 0);
	set_pos_leg(&left_back,  0, 0, 0);
	set_pos_leg(&right_front,  0, 0, 0);
	set_pos_leg(&right_middle,  0, 0, 0);
	set_pos_leg(&right_back,  0, 0, 0);
	translate_leg_angle();
	write_to_all();
	Frame_Counter = FRAME_RATE/2;
}


// Beräkna benets position under en del-rörelse
void set_frame(leg_info* leg, float x_local, float y_local)
{
	// Om ingen kropps rotation finns är beräkningen onödig
/*	
	if(Y_Pitch != 0 || X_Roll != 0 || Z_Yaw != 0)
	{
		body_rotate(leg, X_Roll, Y_Pitch, Z_Yaw);
	}
*/	
	// Om ingen rotation sker kan beräkningen skippas
	
	if (Angular_Step_Length != 0)
	{
		// Samma cosinus och sinus värde kommer användas flera gånger
		// Räkna endast en gång för att spara beräknings kraft
		//float cos_angle = cos(PI*Angular_Step_Length/180);
		//float sin_angle = sin(PI*Angular_Step_Length/180);
		float cos_angle = COS_TABLE[Angular_Step_Length + 200];
		float sin_angle = SIN_TABLE[Angular_Step_Length + 200];
		
		
		// Benens förflytande för att rotera roboten
		// Rotationsmatris i 2D används här
		x_local = x_local + leg->x_from_center*cos_angle
		+ leg->y_from_center*sin_angle
		- leg->x_from_center;
		
		y_local = y_local + leg->y_from_center*cos_angle
		- leg->x_from_center*sin_angle
		- leg->y_from_center;
	}
	
	// Använd sinus-liknande funktion för naturlig rörelse
	float frame_factor = PI*Frame_Counter/FRAME_RATE;
	float cos_frame_factor = cos(frame_factor);
	float sin_frame_factor = sin(frame_factor);
	
	// Tryck fram-bakben ihop
	float x_reach_adjust;
	
	if (x_local >= 0)
	{
		x_reach_adjust = x_local;
	}
	else
	{
		x_reach_adjust = -x_local;
	}
	
	if (Body_Height_Adjust > 0)
	{
		x_reach_adjust = 0;
	}
	
	if (leg->Gait_State == TAKE_OFF_AND_LAND)
	{
		
		// float step_length = sqrt(SQ(x_local) + SQ(y_local));
		float z_amplitude; // = step_length*(-sin_frame_factor);
		float y_reach_adjust = 0;
				
		if (Body_Height_Adjust < 20)
		{
			if (x_local >= 30 || x_local <= -30)
			{
				y_reach_adjust = 30;
			}			
			z_amplitude = sqrt(SQ(x_local) + SQ(y_local))*(-sin_frame_factor);
		}
		else
		{
			y_reach_adjust = 25 + Body_Height_Adjust;
			z_amplitude = (Z_OFFSET_LEG + Body_Height_Adjust)*(-sin_frame_factor);
		}
		
		switch (leg->leg_number)
		{
			case 0:
			set_pos_leg(leg,
			x_local*(-cos_frame_factor) - x_reach_adjust,
			y_local*(-cos_frame_factor) + y_reach_adjust*sin_frame_factor,
			z_amplitude);
			break;
			
			case 3:
			set_pos_leg(leg,
			x_local*(-cos_frame_factor) - x_reach_adjust,
			y_local*(-cos_frame_factor) - y_reach_adjust*sin_frame_factor,
			z_amplitude);
			break;
			
			case 2:
			set_pos_leg(leg,
			x_local*(-cos_frame_factor) + x_reach_adjust,
			y_local*(-cos_frame_factor) + y_reach_adjust*sin_frame_factor,
			z_amplitude);
			break;
			
			case 5:
			set_pos_leg(leg,
			x_local*(-cos_frame_factor) + x_reach_adjust,
			y_local*(-cos_frame_factor) - y_reach_adjust*sin_frame_factor,
			z_amplitude);
			break;
			
			case 1:
			set_pos_leg(leg,
			x_local*(-cos_frame_factor),
			y_local*(-cos_frame_factor) + y_reach_adjust*sin_frame_factor,
			z_amplitude);
			break;
			
			case 4:
			set_pos_leg(leg,
			x_local*(-cos_frame_factor),
			y_local*(-cos_frame_factor) - y_reach_adjust*sin_frame_factor,
			z_amplitude);
			break;
			
			default:
			break;
		}
		
		if (Frame_Counter == FRAME_RATE - 1)
		{
			leg->Gait_State = SWIM_BACK;
		}
	}
	else if (leg->Gait_State == SWIM_BACK)
	{
		if (leg->leg_number == 0 || leg->leg_number == 3)
		{
			set_pos_leg(leg,
			x_local*(cos_frame_factor) - x_reach_adjust,
			y_local*(cos_frame_factor),
			0);
		}
		else if (leg->leg_number == 2 || leg->leg_number == 5)
		{
			set_pos_leg(leg,
			x_local*(cos_frame_factor) + x_reach_adjust,
			y_local*(cos_frame_factor),
			0);
		}
		else
		{
			set_pos_leg(leg,
			x_local*(cos_frame_factor),
			y_local*(cos_frame_factor),
			0);
		}
		
		if (Frame_Counter == FRAME_RATE - 1)
		{
			leg->Gait_State = TAKE_OFF_AND_LAND;
		}
	}
}


// Sekvensiera benrörelse
void tripod_gait()
{
	// _delay_ms(10); // Uppdaterings intervall.
	// tiden för servo att nå sitt slutposition innan nästa position begärs
	
	// Behöver inte göra något om inget rörelse finns
	//if (X_Step_Length != 0 || Y_Step_Length != 0 || Angular_Step_Length != 0 ||
	//Y_Pitch != 0 || X_Roll != 0 || Z_Yaw != 0)
	if (X_Step_Length != 0 || Y_Step_Length != 0 || Angular_Step_Length != 0)
	{
		// kopiera in från globala variabler. eftersom globala variabler kan ändras när osm helst
		float x_copy = X_Step_Length;
		float y_copy = Y_Step_Length;
		set_frame(&left_front, x_copy, y_copy);
		set_frame(&left_middle, x_copy, y_copy);
		set_frame(&left_back, x_copy, y_copy);
		set_frame(&right_front, x_copy, y_copy);
		set_frame(&right_middle, x_copy, y_copy);
		set_frame(&right_back, x_copy, y_copy);
		
		if (Frame_Counter == FRAME_RATE - 1)
		{
			Frame_Counter = 0;
		}
		else
		{
			++Frame_Counter;
		}
	}

	/*
	
	else
	{
	Frame_Counter = FRAME_RATE/2;
	
	body_rotate(&left_front, X_Roll, Y_Pitch, Z_Yaw);
	body_rotate(&left_middle, X_Roll, Y_Pitch, Z_Yaw);
	body_rotate(&left_back, X_Roll, Y_Pitch, Z_Yaw);
	body_rotate(&right_front, X_Roll, Y_Pitch, Z_Yaw);
	body_rotate(&right_middle, X_Roll, Y_Pitch, Z_Yaw);
	body_rotate(&right_back, X_Roll, Y_Pitch, Z_Yaw);
	
	set_pos_leg(&left_front, left_front.rotate_offset_x, left_front.rotate_offset_y, left_front.rotate_offset_z);
	set_pos_leg(&left_middle, left_middle.rotate_offset_x, left_middle.rotate_offset_y, left_middle.rotate_offset_z);
	set_pos_leg(&left_back, left_back.rotate_offset_x, left_back.rotate_offset_y, left_back.rotate_offset_z);
	
	set_pos_leg(&right_front, right_front.rotate_offset_x, right_front.rotate_offset_y, right_front.rotate_offset_z);
	set_pos_leg(&right_middle, right_middle.rotate_offset_x, right_middle.rotate_offset_y, right_middle.rotate_offset_z);
	set_pos_leg(&right_back, right_back.rotate_offset_x, right_back.rotate_offset_y, right_back.rotate_offset_z);
	
	}
	*/
	translate_leg_angle();
	write_to_all();
}

// Rotera kroppens koordinataxlar (Pitch, Roll, Yaw)
void body_rotate(leg_info* leg, int x_angle, int y_angle, int z_angle)
{
	/*
	float sin_x_angle = sin(PI*x_angle/180);
	float cos_x_angle = cos(PI*x_angle/180);
	
	float sin_y_angle = sin(PI*y_angle/180);
	float cos_y_angle = cos(PI*y_angle/180);
	
	float sin_z_angle = sin(PI*z_angle/180);
	float cos_z_angle = cos(PI*z_angle/180);
	*/
	float sin_x_angle = SIN_TABLE[x_angle + 200];
	float cos_x_angle = COS_TABLE[x_angle + 200];
	
	float sin_y_angle = SIN_TABLE[y_angle + 200];
	float cos_y_angle = COS_TABLE[y_angle + 200];
	
	float sin_z_angle = SIN_TABLE[z_angle + 200];
	float cos_z_angle = COS_TABLE[z_angle + 200];
	// Rotationsmatris i 3D
	leg->rotate_offset_x = leg->x_from_center*cos_y_angle*cos_z_angle +
	leg->y_from_center*(cos_x_angle*sin_z_angle + sin_x_angle*sin_y_angle*cos_z_angle) +
	leg->z_from_center*(sin_x_angle*sin_z_angle - cos_x_angle*sin_y_angle*cos_z_angle) -
	leg->x_from_center;
	
	leg->rotate_offset_y = leg->x_from_center*(-cos_y_angle)*sin_z_angle +
	leg->y_from_center*(cos_x_angle*cos_z_angle - sin_x_angle*sin_y_angle*sin_z_angle) +
	leg->z_from_center*(sin_x_angle*cos_z_angle + cos_x_angle*sin_y_angle*sin_z_angle) -
	leg->y_from_center;
	
	leg->rotate_offset_z = leg->x_from_center*sin_y_angle +
	leg->y_from_center*(-sin_x_angle*cos_y_angle) +
	leg->z_from_center*cos_x_angle*cos_y_angle -
	leg->z_from_center;
}
