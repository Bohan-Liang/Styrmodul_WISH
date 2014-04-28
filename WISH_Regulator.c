/*
 * WISH_Regulator.c
 *
 * Created: 2014-04-15 14:28:38
 *  Author: Bohan
 */ 
#include "WISH_Regulator.h"

void init_regulator()
{
	Error = 0;
	Accumulated_Error = 0;
	Diff_Error = 0;
}

void PID_regulator()
{
	
	int P_reg = K_P*Error;
	int I_reg = K_I*Accumulated_Error;
	int D_reg = K_D*Diff_Error;
	int Regulator = P_reg + I_reg + D_reg;
	//float Error_float = (float)Error;
	if (Accumulated_Error <= -127 || Accumulated_Error >= 127)
	{
		Accumulated_Error = 0;
	}
	if (Object_Right == 0 && Object_Left == 0)
	{
		X_Step_Length = 20;
		
		if(Regulator <= 100 && Regulator >= -100)
		{
			Angular_Step_Length = Regulator;
		}
		else if (Regulator < -100)
		{
			Angular_Step_Length = -100;
		}
		else if (Regulator > 100)
		{
			Angular_Step_Length = 100;
		}
	}
	
	else if (Object_Right == 1)
	{
		X_Step_Length = 10;
		Angular_Step_Length = 20;
		Last_Front_Sensor = Front_Sensor;
	}
	
	else if (Object_Left == 1)
	{
		X_Step_Length = 10;
		Angular_Step_Length = -20;
		Last_Front_Sensor = Front_Sensor;
	}
	
	else if (Object_Right == 2)
	{
		int right_corner = 2;
		while (Object_Left != 3)
		{
			turning_corners(right_corner);
		}
		
	}
	
	else if (Object_Left == 2)
	{
		int left_corner = 1;
		
		while (Object_Left != 3)
		 {
			 turning_corners(left_corner);
		 }
	}
	
	else if (Object_Right == 3)
	{
		
	}
}


void turning_corners(int corner)
{
	
	if (Front_Sensor != 0 && Front_Sensor != 255)
	{
		if (Front_Sensor - Last_Front_Sensor <= 2)
		{
			if (corner == 1)
			{
				X_Step_Length = 0;
				Angular_Step_Length = -20;
			}
			
			else
			{
				X_Step_Length = 0;
				Angular_Step_Length = 20;
			}		
		}
		else if (Front_Sensor - Last_Front_Sensor >= 2)
		{
			if (corner == 1)
			{
				X_Step_Length = 20;
				Angular_Step_Length = -50;
			}
			else
			{
				X_Step_Length = 20;
				Angular_Step_Length = 50;
			}
			
		}
	}
	
	Last_Front_Sensor = Front_Sensor;
}