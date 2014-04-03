/*
 * WISH_Gait_Engine.c
 *
 * Created: 2014-03-28 21:10:09
 *  Author: Bohan
 */ 

#include "WISH_Gait_Engine.h"

void init_gait(int frame_rate)
{
	// UPDATE_INTERVAL = interval;
	FRAME_RATE = frame_rate;
	Frame_Counter = frame_rate/2;
	
	X_Step_Length = 0;
	Y_Step_Length = 0;
	Angular_Step_Length = 0;
	
	left_front.Gait_State = TAKE_OFF_AND_LAND;
	left_back.Gait_State = TAKE_OFF_AND_LAND;
	right_middle.Gait_State = TAKE_OFF_AND_LAND;
	
	right_front.Gait_State = SWIM_BACK;
	right_back.Gait_State = SWIM_BACK;
	left_middle.Gait_State = SWIM_BACK;
}

void set_frame(leg_info* leg, double x_local, double y_local)
{
	if(Y_Pitch != 0 || X_Roll != 0 || Z_Yaw != 0)
	{
		body_rotate(leg, X_Roll, Y_Pitch, Z_Yaw);
	}
	
	if (Angular_Step_Length != 0)
	{
		double cos_angle = cos(PI*Angular_Step_Length/180);
		double sin_angle = sin(PI*Angular_Step_Length/180);
		
		x_local = x_local + leg->x_from_center*cos_angle
						  + leg->y_from_center*sin_angle
						  - leg->x_from_center;
		
		y_local = y_local + leg->y_from_center*cos_angle
						  - leg->x_from_center*sin_angle
						  - leg->y_from_center;
	}
	double frame_factor = PI*Frame_Counter/FRAME_RATE;
	double cos_frame_factor = cos(frame_factor);
	double sin_frame_factor = sin(frame_factor);
	if (leg->Gait_State == TAKE_OFF_AND_LAND)
	{
		/*
		set_pos_leg(leg,
					x_local*(-cos_frame_factor),
					y_local*(-cos_frame_factor),
					sqrt(SQ(x_local) + SQ(y_local))*(-sin_frame_factor));
					*/
		if ((Y_Step_Length <= 10 || Y_Step_Length >= -10) &&
			(Angular_Step_Length <= 3 || Angular_Step_Length >= -3) &&
			(X_Step_Length >= 20 || X_Step_Length <= -20)) // vid nästan framåt körning kommer x-, y-leden rita ellips i luften
		{
			if (leg->leg_number <= 2)
			{
				set_pos_leg(leg,
				x_local*(-cos_frame_factor) + leg->rotate_offset_x,
				y_local*(-cos_frame_factor) + 20*sin_frame_factor + leg->rotate_offset_y,
				sqrt(SQ(x_local) + SQ(y_local))*(-sin_frame_factor) + leg->rotate_offset_z);
			}
			else
			{
				set_pos_leg(leg,
				x_local*(-cos_frame_factor) + leg->rotate_offset_x,
				y_local*(-cos_frame_factor) - 20*sin_frame_factor + leg->rotate_offset_y,
				sqrt(SQ(x_local) + SQ(y_local))*(-sin_frame_factor) + leg->rotate_offset_z);
			}
		}
		else
		{
			set_pos_leg(leg,
			x_local*(-cos_frame_factor) + leg->rotate_offset_x,
			y_local*(-cos_frame_factor) + leg->rotate_offset_y,
			sqrt(SQ(x_local) + SQ(y_local))*(-sin_frame_factor) + leg->rotate_offset_z);
		}
		
		if (Frame_Counter == FRAME_RATE - 1)
		{
			leg->Gait_State = SWIM_BACK;
		}
	}
	else if (leg->Gait_State == SWIM_BACK)
	{
		/*
		set_pos_leg(leg,
					x_local*(cos_frame_factor),
					y_local*(cos_frame_factor),
					0);
		*/
		
		set_pos_leg(leg, x_local - 2*x_local*Frame_Counter/FRAME_RATE + leg->rotate_offset_x,
					y_local - 2*y_local*Frame_Counter/FRAME_RATE + leg->rotate_offset_y, leg->rotate_offset_z);
		
		
		if (Frame_Counter == FRAME_RATE - 1)
		{
			leg->Gait_State = TAKE_OFF_AND_LAND;
		}
	}
}

void tripod_gait()
{
	_delay_ms(10);
	if (X_Step_Length != 0 || Y_Step_Length != 0 || Angular_Step_Length != 0 ||
		Y_Pitch != 0 || X_Roll != 0 || Z_Yaw != 0)
	{
		double x_copy = X_Step_Length;
		double y_copy = Y_Step_Length;
		
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
			Frame_Counter++;
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

void body_rotate(leg_info* leg, double x_angle, double y_angle, double z_angle)
{
	double sin_x_angle = sin(PI*x_angle/180);
	double cos_x_angle = cos(PI*x_angle/180);
	
	double sin_y_angle = sin(PI*y_angle/180);
	double cos_y_angle = cos(PI*y_angle/180);
	
	double sin_z_angle = sin(PI*z_angle/180);
	double cos_z_angle = cos(PI*z_angle/180);
	
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