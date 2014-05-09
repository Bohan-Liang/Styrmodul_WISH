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
#define OBJECT_FRONT_MARGIN 50
#define OBJECT_FRONT_HALT 30

// Hastigheter
#define SPEED_CORRIDOR 40
#define MEDIUM_SPEED_KORRIDOR 10
#define HALT_SPEED_KORRIDOR 1
#define SPEED_CORNER 32
#define SPEED_CLIMB 40

#define HEIGHT_OFFSET_CLIMB 40

//Regulator
#define MAX_STRAFE_REGULATION 5
#define STRAFE_K_P 2
#define STRAFE_K_P_AFTER_TURN 4

// Marginaler
#define STRAFE_REGULATOR_MARGIN 5
#define MARGIN_FINISH_CLIMP 35

// F�r referenssystem
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

// vanlig reglering
void _regulate()
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

void init_autonomous_operation()
{
	_Autonomus_Frame_Rate = AUTONOMUS_FRAME_RATE;
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
	
	Error = 0; // Avvikelse fr�n mittlinje

	Diff_Error = 0; //speed_side_ways i ollan o robbe program


	Object_Right = 0; // ger ett v�rde 0 d� b�da sensorerna p� h�ger sida ser en v�gg. 1 d� en av sensorerna ser en v�gg och den andra inte, och 2 d� ingen ser.

	Object_Left = 0; // motsvarande fast f�r v�nster sida.

	Front_Sensor = UINT8_MAX; // fr�mre sensorn

	Back_Sensor = UINT8_MAX;

	Object_Front = 0;

	Object_Back = 0;
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
// k�rs efter ett kort avst�nd fram uppst�tt.
// dess uppgift �r att best�mma om det bara var en st�rning
// eller om och hur roboten ska agera
void _determin_obstacle()
{
	++Tick_Counter;
	_regulate();
	if(Tick_Counter > 3*FRAME_RATE)
	{
		// kl�tterhinder
		if(Type_Front_Sensor == BARRIER_DETECTED)
		{
			//�ndrar h�jd och vinkel
			FRAME_RATE = _Climb_Frame_Rate;
			Body_Height_Adjust = HEIGHT_OFFSET_CLIMB;
			Y_Pitch = 70*Direction;
			//�ndrar hastighet
			_Angular_Step_Length = 0;
			_Y_Step_Length = 0;
			_X_Step_Length = SPEED_CLIMB;
			//Byter assignment
			Tick_Counter = 0;
			Current_Assignment = CLIMB;
		}
		// korsning/sv�ng eller �terv�ndsgr�nd
		else
		{
			//v�nd om
			if (Right_Sensor == 0 && Left_Sensor == 0)
			{
				send_control_decision(REVERSES);
				_Angular_Step_Length = 0;
				_X_Step_Length = SPEED_CORRIDOR;
				Direction = -Direction;
				
				Current_Assignment = WALK_IN_CORRIDOR;
			}
			//rotera h�ger
			else if (Right_Sensor != 0)
			{
				_Y_Step_Length = 0;
				_X_Step_Length = 0;
				_Angular_Step_Length = TURN_RIGHT_SPEED;
				Tick_Counter = 0;
				Current_Assignment = ROTATE;
			}
			//rotera v�nster
			else // (Left_Sensor != 0)
			{
				_Y_Step_Length = 0;
				_X_Step_Length = 0;
				_Angular_Step_Length = TURN_LEFT_SPEED;
				Tick_Counter = 0;
				Current_Assignment = ROTATE;
			}
		}
	}
	else if(Forward_Sensor > OBJECT_FRONT_MARGIN)
	{
		_X_Step_Length = SPEED_CORRIDOR; //<-----------------------------------------------
		_Y_Step_Length = 0;
		_Angular_Step_Length = 0;
		Tick_Counter = 0;
		Current_Assignment = WALK_IN_CORRIDOR;
	}
	else if(Forward_Sensor < OBJECT_FRONT_HALT)
	{
		_X_Step_Length = HALT_SPEED_KORRIDOR;
		_Y_Step_Length = 0;
		_Angular_Step_Length = 0;
	} 
	// forts�tt i detta l�ge
}
void _climb()
{
	if(Backward_Sensor <= MARGIN_FINISH_CLIMP && Tick_Counter > 5*FRAME_RATE)
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
	// Hinder framf�r robot
	//-----------------------------------------------------------------
	if(Forward_Sensor <= OBJECT_FRONT_MARGIN)
	{
		_X_Step_Length = MEDIUM_SPEED_KORRIDOR;
		Tick_Counter = 0;
		_Y_Step_Length = 0;
		_Angular_Step_Length=0;
		Current_Assignment = DETERMIN_OBSTACLE;
	}
	//-----------------------------------------------------------------
	//	Korsning
	//-----------------------------------------------------------------
	// Just nu l�gre prioritet �n hinder framf�r
	else if (Right_Sensor == 1)
	{
		_Y_Step_Length = 0;
		send_control_decision(TURNING_RIGHT);
		_Angular_Step_Length = TURN_RIGHT_SPEED;
		_X_Step_Length = SPEED_CORNER;
		Tick_Counter = 0;
		Current_Assignment = TURN;
	}
	// kollar om ska sv�nga v�nster
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
		_regulate();
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
	//G�r tills ser korridor r�knar sedan upp Tick counter
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
	if(Direction == FORWARD)
	{
		_Error = Error;
		_Diff_Error = Diff_Error;
		Type_Front_Sensor = Object_Front;
		Type_Back_Sensor = Object_Back;
		Forward_Sensor = Front_Sensor;
		Backward_Sensor = Back_Sensor;
		Right_Sensor = Object_Right;
		Left_Sensor = Object_Left;
	}
	else
	{
		_Error = -Error;
		_Diff_Error = -Diff_Error;
		Forward_Sensor = Back_Sensor;
		Backward_Sensor = Front_Sensor;
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
	
	//S�tter hastigheter
	X_Step_Length = _X_Step_Length * Direction;
	Y_Step_Length = _Y_Step_Length;
	Angular_Step_Length = _Angular_Step_Length;
}