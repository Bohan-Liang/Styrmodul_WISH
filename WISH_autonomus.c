/*
* WISH_Regulator.c
*
* Created: 2014-04-15 14:28:38
*  Author: Bohan
*/
#include "WISH_autonomus.h"
#include "SPI_Slave.h"


// Hinder
#define BARRIER_DETECTED 3
#define OBJECT_FRONT_MARGIN 30

// Hastigheter
#define SPEED_CORRIDOR 40
#define SPEED_CORNER 32

#define HEIGHT_OFFSET_CLIMB 40

//Regulator
#define MAX_STRAFE_REGULATION 5
#define STRAFE_K_P 2
#define STRAFE_K_P_AFTER_TURN 4

// Marginaler
#define STRAFE_REGULATOR_MARGIN 5

// För referenssystem
float _X_Step_Length;
float _Y_Step_Length;
float _Angular_Step_Length;
signed char _Error;
signed char _Diff_Error;

int _Autonomus_Frame_Rate;
int _Climb_Frame_Rate;

#define REULATE_STRAFE(KP)													\
	_Y_Step_Length = -KP * Error; 											\
	if(abs(Y_Step_Length) > MAX_STRAFE_REGULATION)							\
		_Y_Step_Length = sign(_Y_Step_Length) * MAX_STRAFE_REGULATION


void init_autonomous_operation()
{
	_Autonomus_Frame_Rate = FRAME_RATE;
	_Climb_Frame_Rate = 48;
	
	FRAME_RATE = _Autonomus_Frame_Rate;
	
	Error = 0;
	Accumulated_Error = 0;
	Diff_Error = 0;
	K_P = 1;
	K_D = 4;
	
	_Error = 0;
	_Diff_Error = 0;
	
	X_Step_Length = SPEED_CORRIDOR;
	Angular_Step_Length = 0;
	Y_Step_Length = 0;
	
	_X_Step_Length = SPEED_CORRIDOR;
	_Y_Step_Length = 0;
	_Angular_Step_Length = 0;
	
	Current_Assignment = WALK_IN_CORRIDOR;
	Direction = FORWARD;
}

unsigned char Transform_Objekt_Backward(unsigned char Object)
{
	switch(Object)
	{
		case 0:
		return	0;
		break;
		
		case 1:
		return 3;
		break;
		
		case 2:
		return 2;
		break;
		
		case 3:
		return 1;
		break;
		
		default:
		return 0;
		break;
	}
}
void _determin_obstacle()
{
	++Tick_Counter;
	if(Forward_Sensor > 70)
	{
		X_Step_Length = 0;
		Tick_Counter = 0;
		Current_Assignment = WALK_IN_CORRIDOR;
	}
	/*else if(Tick_Counter > 5*FRAME_RATE)
	{
		FRAME_RATE = _Climb_Frame_Rate;
		Body_Height_Adjust = HEIGHT_OFFSET_CLIMB;
		Y_Pitch = 70*Direction;
			
		Angular_Step_Length = 0;
		Y_Step_Length = 0;
			
		Tick_Counter = 0;
		Current_Assignment = CLIMB;
	}*/
}
void _climb()
{
	if(Back_Sensor <= 30 && Tick_Counter > 5*FRAME_RATE)
	{
		FRAME_RATE = _Autonomus_Frame_Rate;
		Body_Height_Adjust = 0;
		Y_Pitch = 0;
		set_body_rotation();
		Y_Step_Length = 0;
		Angular_Step_Length = 0;
		Current_Assignment = WALK_IN_CORRIDOR;
	}
	
	if(Back_Sensor > 80)
	{
		++Tick_Counter;
	}
	
	if (Right_Sensor != 2 && Left_Sensor !=2)
	{
		//Rotations reglering
		int Regulator = (K_P*_Error + K_D*_Diff_Error);

		if(Regulator <= 70 && Regulator >= -70)
		{
			_Angular_Step_Length = Regulator;
		}
		else if (Regulator < -70)
		{
			_Angular_Step_Length = -70;
		}
		else if (Regulator > 70)
		{
			_Angular_Step_Length = 70;
		}
		// Strafe reglering
		if(abs(_Error) >= STRAFE_REGULATOR_MARGIN)
		{
			REULATE_STRAFE(STRAFE_K_P);
		}
		else
		{
			_Y_Step_Length = 0;
		}
	}
}
void _walk_in_corridor()
{
	//-----------------------------------------------------------------
	// Hinder framför robot
	//-----------------------------------------------------------------
	if(Forward_Sensor <= 70 && Object_Front == BARRIER_DETECTED)
	{
		X_Step_Length = 5;
		Y_Step_Length = 0;
		Angular_Step_Length = 0;
		Tick_Counter = 0;
		Current_Assignment = DETERMIN_OBSTACLE;
	}	
	else if (Forward_Sensor <= OBJECT_FRONT_MARGIN)
	{
		//vänd om
		if (Right_Sensor == 0 && Left_Sensor == 0)
		{
			send_control_decision(REVERSES);
			Angular_Step_Length = 0;
			Direction = -Direction;
		}
		//rotera höger
		else if (Right_Sensor != 0)
		{
			_Y_Step_Length = 0;
			_X_Step_Length = 0;
			_Angular_Step_Length = TURN_LEFT_SPEED;
			Tick_Counter = 0;
			Current_Assignment = ROTATE;
		}
		//rotera vänster
		else if (Left_Sensor != 0)
		{
			_Y_Step_Length = 0;
			_X_Step_Length = 0;
			_Angular_Step_Length = TURN_LEFT_SPEED;
			Tick_Counter = 0;
			Current_Assignment = ROTATE;
		}
	}

	//-----------------------------------------------------------------
	//	Korsning
	//-----------------------------------------------------------------
	if (Right_Sensor == 1)
	{
		_Y_Step_Length = 0;
		send_control_decision(TURNING_RIGHT);
		_Angular_Step_Length = TURN_RIGHT_SPEED;
		_X_Step_Length = SPEED_CORNER;
		Tick_Counter = 0;
		Current_Assignment = TURN;
	}
	// kollar om ska svänga vänster
	else if (Left_Sensor == 1)
	{
		_Y_Step_Length = 0;
		send_control_decision(TURNING_LEFT);
		_Angular_Step_Length = TURN_LEFT_SPEED;
		_X_Step_Length = SPEED_CORNER;
		Tick_Counter = 0;
		Current_Assignment = TURN;
	}
	// Vanlig reglering
	else if (Right_Sensor != 2 && Left_Sensor !=2)
	{
		//Rotations reglering
		int Regulator = (K_P*_Error + K_D*_Diff_Error);

		if(Regulator <= 70 && Regulator >= -70)
		{
			_Angular_Step_Length = Regulator;
		}
		else if (Regulator < -70)
		{
			_Angular_Step_Length = -70;
		}
		else if (Regulator > 70)
		{
			_Angular_Step_Length = 70;
		}
		// Strafe reglering 
		if(abs(_Error) >= STRAFE_REGULATOR_MARGIN)
		{
			REULATE_STRAFE(STRAFE_K_P);
		}
		else
		{
			_Y_Step_Length = 0;
		}
	}
}
void _turn()
{
	++Tick_Counter;
	if (Tick_Counter >= 6*FRAME_RATE)
	{
		_X_Step_Length = SPEED_CORRIDOR;
		_Angular_Step_Length = 0;
		Tick_Counter = 0;
		send_control_decision(FINISHED_TURNING);
		Current_Assignment = FINISH_TURN;
	}
}
void _finish_turn()
{
	//Går tills ser korridor räknar sedan upp Tick counter
	if((Right_Sensor != 2) && (Left_Sensor != 2)) //
	{
		if(Tick_Counter == 0)
			Tick_Counter = 1;
		REULATE_STRAFE(STRAFE_K_P);
	}
	
	
	/*if(Tick_Counter >= 2*FRAME_RATE)
	{
		Tick_Counter = 0;
		_X_Step_Length =SPEED_CORRIDOR;
		_Y_Step_Length = 0;
		Current_Assignment = WALK_IN_CORRIDOR;
	}
	else*/ if (Tick_Counter > 0)
	{
		Tick_Counter = 0;
		_X_Step_Length =SPEED_CORRIDOR;
		_Y_Step_Length = 0;
		Current_Assignment = WALK_IN_CORRIDOR;
		//++Tick_Counter;
	}
}
void _rotate()
{
	++Tick_Counter;
	if (Tick_Counter > 6*FRAME_RATE)
	{
		_X_Step_Length = SPEED_CORRIDOR * Direction;
		_Angular_Step_Length = 0;
		Current_Assignment = WALK_IN_CORRIDOR;
	}
}

void autonomous_operation()
{
	if(Front_Sensor < 30)
	{
		send_control_decision(2);
	}
	else if (Back_Sensor < 30)
	{
		send_control_decision(1);
	}
	
	if(Direction == FORWARD)
	{
		_Error = Error;
		_Diff_Error = Diff_Error;
		Type_Front_Sensor = Object_Front;
		Type_Back_Sensor = Object_Back;
		Forward_Sensor = Front_Sensor;
		Right_Sensor = Object_Right;
		Left_Sensor = Object_Left;
	}
	else
	{
		_Error = -Error;
		_Diff_Error = -Diff_Error;
		Forward_Sensor = Back_Sensor;
		Type_Back_Sensor = Object_Front;
		Type_Front_Sensor = Object_Back;
		Right_Sensor = Transform_Objekt_Backward(Object_Left);
		Left_Sensor  = Transform_Objekt_Backward(Object_Right);
	}
	
	switch (Current_Assignment)
	{
		//---------------------------------------------------------------------
		case WALK_IN_CORRIDOR:
		//---------------------------------------------------------------------
		_walk_in_corridor();
		break;
		//---------------------------------------------------------------------
		case TURN:
		//---------------------------------------------------------------------
		_turn();
		break;
		//---------------------------------------------------------------------
		case FINISH_TURN:
		//---------------------------------------------------------------------
		_finish_turn();
		break;
		//---------------------------------------------------------------------
		case ROTATE:
		//---------------------------------------------------------------------
		_rotate();
		break;
		//---------------------------------------------------------------------
		case CLIMB:
		//---------------------------------------------------------------------
		_climb();
		break;
		//---------------------------------------------------------------------
		case DETERMIN_OBSTACLE:
		//---------------------------------------------------------------------
		_determin_obstacle();
		break;
	}
	
	//Sätter hastigheter
	X_Step_Length = _X_Step_Length * Direction;
	Y_Step_Length = _Y_Step_Length;
	Angular_Step_Length = _Angular_Step_Length;
}