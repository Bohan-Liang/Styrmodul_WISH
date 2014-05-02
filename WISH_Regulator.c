/*
* WISH_Regulator.c
*
* Created: 2014-04-15 14:28:38
*  Author: Bohan
*/
#include "WISH_Regulator.h"
//#include "SPI_Slave.h"
#define SPEED_CORRIDOR 40
#define SPEED_CORNER 30

#define STRAFE_K_P 2

void init_regulator()
{
	Error = 0;
	Accumulated_Error = 0;
	Diff_Error = 0;
}

void PID_regulator()
{
	int P_reg = K_P*Error;
	//int I_reg = K_I*Accumulated_Error;
	int D_reg = K_D*Diff_Error;
	int Regulator = (P_reg + D_reg)*Direction;
	
	
	if(Direction == FORWARD)
	{
		Forward_Sensor = Front_Sensor;
	}
	else
	{
		Forward_Sensor = Back_Sensor;
	}
	
	
	
	
	if (Forward_Sensor > 25)
	{
		X_Step_Length = SPEED_CORRIDOR * Direction;
	}
	else if (Forward_Sensor <= 25)
	{
		X_Step_Length = 0;
		Angular_Step_Length = 0;
		if (Object_Right == 0 && Object_Left == 0)
		{
			//send_control_decision(REVERSES);
			Direction = -Direction;
		}
		else if (Object_Right != 0)
		{
			Angular_Step_Length = TURN_RIGHT * Direction;
			for (int i = 6*FRAME_RATE; i >= 0; --i)
			{
				tripod_gait();
			}
		}
		else if (Object_Left != 0)
		{
			Angular_Step_Length = TURN_LEFT * Direction;
			for (int i = 6*FRAME_RATE; i >= 0; --i)
			{
				tripod_gait();
			}
		}
	}
	
	
	// Kollar om korsning eller korridor
	if ((Object_Right == 1 && Direction == FORWARD) || (Object_Right == 3 && Direction == BACKWARD)) 
	{
		turning_corners(TURN_RIGHT);
		//send_control_decision(TURNING_RIGHT);
		}
	else if ((Object_Left == 1 && Direction == FORWARD) || (Object_Left == 3 && Direction == BACKWARD))
	{	
		turning_corners(TURN_LEFT);
		//send_control_decision(TURNING_LEFT);
	}
	
	else if (Object_Right !=2 && Object_Left != 2)
	{
		
		if(Regulator <= 70 && Regulator >= -70)
		{
			Angular_Step_Length = Regulator;
		}
		else if (Regulator < -70)
		{
			Angular_Step_Length = -70;
		}
		else if (Regulator > 70)
		{
			Angular_Step_Length = 70;
		}
	}
}

#define MARGIN 1
#define abs(a) ((a < 0) ? -a : a)
#define sign(a) ((a < 0) ? -1 : 1)

void turning_corners(int corner)
{
	X_Step_Length = SPEED_CORNER * Direction;
	Angular_Step_Length = corner * Direction;

	for (int i = 6*FRAME_RATE; i >= 0; --i)
	{
		tripod_gait();
	}

	Angular_Step_Length = 0;
	
	while((Object_Right == 2) || (Object_Left == 2))
	{
		tripod_gait();
	}
	
	for (int i = 2*FRAME_RATE; i >= 0; --i)
	{
		Y_Step_Length = -STRAFE_K_P * Error ;
		if(abs(Y_Step_Length) > 20)
		{
			Y_Step_Length = sign(Y_Step_Length) * 20 ;
		}
		tripod_gait();
	}
	
	
// 	Angular_Step_Length = 0;
// 	X_Step_Length = 0;
// 
// 	while(abs(Error) > MARGIN)
// 	{
// 		Y_Step_Length = -STRAFE_K_P * Error ;
// 		if(abs(Y_Step_Length) > 30)
// 		{
// 			Y_Step_Length = sign(Y_Step_Length) * 30 ;
// 		}
// 		tripod_gait();
// 	}
 	Y_Step_Length = 0;

	init_regulator();
}

