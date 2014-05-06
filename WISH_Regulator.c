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
	K_P = 1;
	K_D = 4;
}

void PID_regulator()
{
	int P_reg = K_P*Error;
	//int I_reg = K_I*Accumulated_Error;
	int D_reg = K_D*Diff_Error;
	int Regulator = (P_reg + D_reg) * Direction;
	
	
	if(Direction == FORWARD)
	{
		Type_Front_Sensor = Object_Front;
		Type_Back_Sensor = Object_Back;
		Forward_Sensor = Front_Sensor;
		Right_Sensor = Object_Right;
		Left_Sensor = Object_Left;
	}
	else
	{
		Forward_Sensor = Back_Sensor;
		Type_Back_Sensor = Object_Front;
		Type_Front_Sensor = Object_Back;
		
		switch(Object_Left)
		{
			case 0:
			Right_Sensor = 0;
			break;
			
			case 1:
			Right_Sensor = 3;
			break;
			
			case 2:
			Right_Sensor = 2;
			break;
			
			case 3:
			Right_Sensor = 1;
			break;
			
			default:
			break;
		}
		
		switch(Object_Right)
		{
			case 0:
			Left_Sensor = 0;
			break;
			
			case 1:
			Left_Sensor = 3;
			break;
			
			case 2:
			Left_Sensor = 2;
			break;
			
			case 3:
			Left_Sensor =1;
			break;
			
			default:
			break;
		}
		
		
	}
	
	
	if (Forward_Sensor > 70)
	{
		X_Step_Length = SPEED_CORRIDOR * Direction;
	}
	else if(Forward_Sensor <= 70)
	{
		if(Type_Front_Sensor == 3)
		{
			if (Body_Height_Adjust == 0)
			{
				FRAME_RATE = 60;
				//Frame_Counter = 30;
				//left_front.Gait_State = TAKE_OFF_AND_LAND;
				//left_back.Gait_State = TAKE_OFF_AND_LAND;
				//right_middle.Gait_State = TAKE_OFF_AND_LAND;
					//
				//right_front.Gait_State = SWIM_BACK;
				//right_back.Gait_State = SWIM_BACK;
				//left_middle.Gait_State = SWIM_BACK;
				Body_Height_Adjust = 50;
			}
		}
		
		else if (Body_Height_Adjust > 0 && Forward_Sensor < 50)
		{
			if (Body_Height_Adjust > 0)
			{
				FRAME_RATE = 24;
				//Frame_Counter = 12;
				//left_front.Gait_State = TAKE_OFF_AND_LAND;
				//left_back.Gait_State = TAKE_OFF_AND_LAND;
				//right_middle.Gait_State = TAKE_OFF_AND_LAND;
//
				//right_front.Gait_State = SWIM_BACK;
				//right_back.Gait_State = SWIM_BACK;
				//left_middle.Gait_State = SWIM_BACK;
				Body_Height_Adjust = 0;
			}
		}
		
		else if (Type_Front_Sensor != 3)
		{
			if (Forward_Sensor <= 25)
			{
				X_Step_Length = 0;
				Angular_Step_Length = 0;
				if (Right_Sensor == 0 && Left_Sensor == 0)
				{
					//send_control_decision(REVERSES);
					Direction = -Direction;
				}
				else if (Right_Sensor != 0)
				{
					Angular_Step_Length = TURN_RIGHT; //* Direction;
					for (int i = 6*FRAME_RATE; i >= 0; --i)
					{
						tripod_gait();
					}
				}
					
				else if (Left_Sensor != 0)
				{
					Angular_Step_Length = TURN_LEFT; //* Direction;
					for (int i = 6*FRAME_RATE; i >= 0; --i)
					{
						tripod_gait();
					}
				}
			}
			else
			{
				X_Step_Length = SPEED_CORRIDOR * Direction;
			}
		}
		
		
	}
					
					// Kollar om korsning eller korridor
	if (Right_Sensor == 1) //(Object_Right == 1 && Direction == FORWARD) || (Object_Right == 3 && Direction == BACKWARD))
	{
		turning_corners(TURN_RIGHT);
		//send_control_decision(TURNING_RIGHT);
	}
	else if (Left_Sensor == 1) //(Object_Left == 1 && Direction == FORWARD) || (Object_Left == 3 && Direction == BACKWARD))
	{
		turning_corners(TURN_LEFT);
		//send_control_decision(TURNING_LEFT);
	}
					
	else if (Type_Front_Sensor == 3)
	{
		Body_Height_Adjust = 50;
	}
					
	else if (Type_Front_Sensor == 3 && Body_Height_Adjust > 0 && Forward_Sensor < 50)
	{
		Body_Height_Adjust = 0;
	}
					
	else if (Right_Sensor != 2 && Left_Sensor !=2) //Object_Right !=2 && Object_Left != 2)
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
	Angular_Step_Length = corner; //* Direction;

	for (int i = 6*FRAME_RATE; i >= 0; --i)
	{
		tripod_gait();
	}

	Angular_Step_Length = 0;					
	
	while((Right_Sensor == 2) || (Left_Sensor == 2)) // gå tills någon av sidorna ser en vägg. leap of faith.
	{
		tripod_gait();
	}
					
	for (int i = 2*FRAME_RATE; i >= 0; --i)
	{
		Y_Step_Length = -STRAFE_K_P * Error;
		if(abs(Y_Step_Length) > 20)
		{
			Y_Step_Length = sign(Y_Step_Length) * 20;
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

