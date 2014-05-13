/*
* WISH_Regulator.c
*
* Created: 2014-04-15 14:28:38
*  Author: Bohan
*/
#include "WISH_autonomus.h"
#include "SPI_Slave.h"

#include <stdbool.h>
// Hinder
#define BARRIER_DETECTED 3
#define OBJECT_FRONT_MARGIN 40
#define OBJECT_FRONT_HALT 25

// Hastigheter
#define SPEED_CORRIDOR 32
#define MEDIUM_SPEED_KORRIDOR 10
#define HALT_SPEED_KORRIDOR 0
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
#define OPEN_FORWARD_MARGIN 70


#define NO_TURN 0
#define RIGHT_TURN 1
#define LEFT_TURN 2
uint8_t Last_Turn;
bool Back_From_Dead_End;
uint8_t Skip_Turn;

// För referenssystem
float _X_Step_Length;
float _Y_Step_Length;
float _Angular_Step_Length;
signed char _Error;
signed char _Diff_Error;
unsigned char Forward_Sensor_High;


unsigned char _Left_Distance;
unsigned char _Right_Distance;

int _Autonomus_Frame_Rate;
int _Climb_Frame_Rate;

#define REULATE_STRAFE(KP)													\
_Y_Step_Length = -KP * Error; 											\
if(abs(Y_Step_Length) > MAX_STRAFE_REGULATION)							\
_Y_Step_Length = sign(_Y_Step_Length) * MAX_STRAFE_REGULATION


void _start_double_check_sensor_value_right()
{
	_Y_Step_Length = 0;
	_Angular_Step_Length = 0;
	//_X_Step_Length -= 5;
	Tick_Counter = 0;
	Current_Assignment = DOUBLE_CHECK_SENSOR_VALUE_RIGHT;
}
void _start_double_check_sensor_value_left()
{
	_Y_Step_Length = 0;
	_Angular_Step_Length = 0;
	//_X_Step_Length -= 5;
	Tick_Counter = 0;
	Current_Assignment = DOUBLE_CHECK_SENSOR_VALUE_LEFT;
}
void _start_climb()
{
	//Ändrar höjd och vinkel
	FRAME_RATE = _Climb_Frame_Rate;
	Body_Height_Adjust = HEIGHT_OFFSET_CLIMB;
	Y_Pitch = 70*Direction;
	//Ändrar hastighet
	_Angular_Step_Length = 0;
	_Y_Step_Length = 0;
	_X_Step_Length = SPEED_CLIMB;
	//Byter assignment
	Tick_Counter = 0;
	Current_Assignment = CLIMB;
}
void _start_walk_in_corridor()
{
	Tick_Counter = 0;
	_X_Step_Length =SPEED_CORRIDOR;
	_Y_Step_Length = 0;
	_Angular_Step_Length = 0;
	
	Body_Height_Adjust = 0;
	// för säkerhets skull nollställs pich här
	Y_Pitch = 0;
	set_body_rotation();
	Current_Assignment = WALK_IN_CORRIDOR;
}
void _start_turn(int dir)
{
	_Y_Step_Length = 0;
	send_control_decision(TURNING_LEFT);
	/*if(dir == TURN_LEFT_SPEED)
	{
		_start_double_check_sensor_value_left();
	}
	else
	{
		_start_double_check_sensor_value_right();
	}*/
	
	_Angular_Step_Length = dir;
	_X_Step_Length = SPEED_CORNER;
	Tick_Counter = 0;
	Current_Assignment = TURN;
}
void _start_finish_turn()
{
	_X_Step_Length = SPEED_CORRIDOR;
	_Angular_Step_Length = 0;
	Tick_Counter = 0;
	send_control_decision(FINISHED_TURNING);
	Current_Assignment = FINISH_TURN;
}
void _start_go_past_crossing()
{
	Y_Step_Length = 0;
	_Angular_Step_Length = 0;
	Tick_Counter = 0;
	Current_Assignment = GO_PAST_CROSSING;
}
void _finish_turn()
{
	//Går tills ser korridor räknar sedan upp Tick counter
	if((Right_Sensor != 2) && (Left_Sensor != 2)) //
	{
		_X_Step_Length=0;
		++Tick_Counter;
		REULATE_STRAFE(STRAFE_K_P);
	}
	
	if(Tick_Counter > 0 && abs(Error) <= 2)
	{
		_start_walk_in_corridor();
	}
	else if(Tick_Counter >= 5*FRAME_RATE)
	{
		_start_walk_in_corridor();
	}
	/*if(Tick_Counter >= 2*FRAME_RATE)
	{
	Tick_Counter = 0;
	_X_Step_Length =SPEED_CORRIDOR;
	_Y_Step_Length = 0;
	Current_Assignment = WALK_IN_CORRIDOR;
	}
	else if (Tick_Counter > 0)
	{
		_start_walk_in_corridor();
		//++Tick_Counter;
	}*/
}
#define DECIRED_DISTANCE_SIDE 32
#define SIDE_K_P 4

void _go_past_crossing()
{
	if(Object_Left != 2 && Object_Right !=2)
	{
		_start_walk_in_corridor();
	}
 	else if(Object_Left == 0) 	
	{
 		_Y_Step_Length = SIDE_K_P*(DECIRED_DISTANCE_SIDE-_Left_Distance); 
		if (_Y_Step_Length < -MAX_STRAFE_REGULATION)
		{
			_Y_Step_Length = -MAX_STRAFE_REGULATION;
		}
		else if (_Y_Step_Length > MAX_STRAFE_REGULATION)
		{
			_Y_Step_Length = MAX_STRAFE_REGULATION;
		}
 	}
 	else if (Object_Right == 0)
 	{
 		_Y_Step_Length = SIDE_K_P*(Right_Distance - DECIRED_DISTANCE_SIDE);
		if (_Y_Step_Length < -MAX_STRAFE_REGULATION)
		{
			_Y_Step_Length = -MAX_STRAFE_REGULATION;
		}
		else if (_Y_Step_Length > MAX_STRAFE_REGULATION)
		{
			_Y_Step_Length = MAX_STRAFE_REGULATION;
		}
 	}
	else
	{
		_Y_Step_Length = 0;
		_Angular_Step_Length = 0;
	}
	
}
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
	Forward_Sensor_High = UINT8_MAX;
	Last_Turn = NO_TURN;
	Back_From_Dead_End = false;
	Skip_Turn = NO_TURN;
	
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
	
	Error = 0; // Avvikelse från mittlinje

	Diff_Error = 0; //speed_side_ways i ollan o robbe program


	Object_Right = 0; // ger ett värde 0 då båda sensorerna på höger sida ser en vägg. 1 då en av sensorerna ser en vägg och den andra inte, och 2 då ingen ser.

	Object_Left = 0; // motsvarande fast för vänster sida.

	Front_Sensor = UINT8_MAX; // främre sensorn

	Back_Sensor = UINT8_MAX;

	Front_Sensor_High = UINT8_MAX;
	Back_sensor_High = UINT8_MAX;
		
	Object_Front = 0;

	Object_Back = 0;
}
void reset_autonomus_operation()
{
	
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
// körs efter ett kort avstånd fram uppstått.
// dess uppgift är att bestämma om det bara var en störning
// eller om och hur roboten ska agera
#define MS_TO_TICK(ms) ms*FRAME_RATE
void _start_determin_obstacle()
{
	//Emergency_Stop();
	//_X_Step_Length = HALT_SPEED_KORRIDOR;
	Tick_Counter = 0;
	_Y_Step_Length = 0;
	_Angular_Step_Length=0;
	Current_Assignment = DETERMIN_OBSTACLE;
}
void _determin_obstacle()
{
	#define  X_SPEED_STEP 10
	if (Forward_Sensor > OBJECT_FRONT_MARGIN)
	{
		Z_Yaw = 0;
		set_body_rotation();
		_start_walk_in_corridor();
	
	}
	else if (_X_Step_Length > 0)
	{
		if(_X_Step_Length > X_SPEED_STEP)
		{	
			_X_Step_Length -= X_SPEED_STEP;
		}
		else
		{
			_X_Step_Length = 0;
			Tick_Counter = 0;
		}
	}
	else if(Tick_Counter < 16)
	{
		Z_Yaw += 6;
	}
	else if(Tick_Counter< 48)
	{
		Z_Yaw -= 6;
	}
	else if(Tick_Counter < 64)
	{
		Z_Yaw +=6;
	}
	else
	{
		Z_Yaw = 0;
		set_body_rotation();
		if (Type_Front_Sensor == BARRIER_DETECTED)
		{
			_start_climb();
		}
		
		else
		{
			Back_From_Dead_End = true;
			Direction = -Direction;
			_start_walk_in_corridor();
		}
	}
	++Tick_Counter;
	#undef X_SPEED_STEP
	/*
	if(Tick_Counter <= FRAME_RATE)
	{
		_Angular_Step_Length = TURN_LEFT_SPEED;
	}
	else if ( Tick_Counter <= 5*FRAME_RATE)
	{
		_Angular_Step_Length = 0;
		if (Forward_Sensor > OBJECT_FRONT_MARGIN)
		{
			_start_walk_in_corridor();
		}
	}
	else if ( Tick_Counter <= 7*FRAME_RATE)
	{
		_Angular_Step_Length = TURN_RIGHT_SPEED;	
	}
	else if ( Tick_Counter <= 12*FRAME_RATE)
	{
		_Angular_Step_Length = 0;
		if (Forward_Sensor > OBJECT_FRONT_MARGIN)
		{
			Z_Yaw = 0;
			set_body_rotation();
			_start_walk_in_corridor();
		}
	}
	else if (Tick_Counter <= 13*FRAME_RATE)
	{
		_Angular_Step_Length = TURN_LEFT_SPEED;
	}
	else
	{
		if (Type_Front_Sensor == BARRIER_DETECTED)
		{
			_start_climb();
		}
		
		else
		{
			Direction = -Direction;
			_start_walk_in_corridor();
		}
	}
	*/
// 	if(Tick_Counter > 3*FRAME_RATE)
// 	{
// 		// klätterhinder
// 		if(Type_Front_Sensor == BARRIER_DETECTED)
// 		{
// 			_start_climb();
// 		}
// 		// korsning/sväng eller återvändsgränd
// 		else
// 		{
// 			//vänd om
// 			if (Right_Sensor == 0 && Left_Sensor == 0)
// 			{
// 				Back_From_Dead_End = true;
// 				Direction = -Direction;
// 				_start_walk_in_corridor();
// 			}
// 			//rotera höger
// 			else if (Right_Sensor != 0)
// 			{
// 				_Y_Step_Length = 0;
// 				_X_Step_Length = 0;
// 				_Angular_Step_Length = TURN_RIGHT_SPEED;
// 				Tick_Counter = 0;
// 				Y_Pitch = 0;
// 				Current_Assignment = ROTATE;
// 			}
// 			//rotera vänster
// 			else // (Left_Sensor != 0)
// 			{
// 				_Y_Step_Length = 0;
// 				_X_Step_Length = 0;
// 				_Angular_Step_Length = TURN_LEFT_SPEED;
// 				Tick_Counter = 0;
// 				Y_Pitch = 0;
// 				Current_Assignment = ROTATE;
// 			}
// 		}
// 	}
// 	else if(Forward_Sensor > OBJECT_FRONT_MARGIN)
// 	{
// 		_X_Step_Length = SPEED_CORRIDOR; //<-----------------------------------------------
// 		_Y_Step_Length = 0;
// 		_Angular_Step_Length = 0;
// 		Tick_Counter = 0;
// 		Y_Pitch = 0;
// 		Current_Assignment = WALK_IN_CORRIDOR;
// 	}
// 	else if(Forward_Sensor < OBJECT_FRONT_HALT)
// 	{
// 		_X_Step_Length = HALT_SPEED_KORRIDOR;
// 		_Y_Step_Length = 0;
// 		Y_Pitch = 0;
// 		_Angular_Step_Length = 0;
// 	} 
// 	// fortsätt i detta läge
}

void _climb()
{
	//tvingar den att gå några steg framåt
	if(Tick_Counter <= 5*FRAME_RATE)
	{
		++Tick_Counter;
	}
	// går tills mönstret på baksensorn inträffar
	else
	{
		if(Backward_Sensor <= MARGIN_FINISH_CLIMP && Tick_Counter > 8*FRAME_RATE)
		{
			Y_Pitch = 0;
			set_body_rotation();
			FRAME_RATE = _Autonomus_Frame_Rate;
			_start_walk_in_corridor();
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
}

void _double_check_sensor_value_right()
{
	#define  SPEED_STEP 5
	if (_X_Step_Length > SPEED_STEP && Tick_Counter == 0)
	{
		_X_Step_Length -= SPEED_STEP;
	}
	else if(Tick_Counter == 0)
	{
		Tick_Counter=1;
		_X_Step_Length = 0;
	}
	else if(Tick_Counter < 5*FRAME_RATE)
	{
		if(Right_Sensor == 0)
		{
			_start_walk_in_corridor();
		}
	}
	else if(Tick_Counter > 5*FRAME_RATE)
	{
		//_X_Step_Length += SPEED_STEP;
		//if (_X_Step_Length > SPEED_CORNER)
		//{
			_Y_Step_Length = 0;
			send_control_decision(TURNING_LEFT);
			_Angular_Step_Length = TURN_RIGHT_SPEED;
			_X_Step_Length = SPEED_CORNER;
			Tick_Counter = 0;
			Current_Assignment = TURN;
		//}
	}
	#undef  SPEED_STEP
}

void _double_check_sensor_value_left()
{
	#define  SPEED_STEP 5
	if (_X_Step_Length > SPEED_STEP && Tick_Counter == 0)
	{
		_X_Step_Length -= SPEED_STEP;
	}
	else if(Tick_Counter == 0)
	{
		Tick_Counter=1;
		_X_Step_Length = 0;
	}
	else if(Tick_Counter < 5*FRAME_RATE)
	{
		if(Right_Sensor == 0)
		{
			_start_walk_in_corridor();
		}
	}
	else
	{
		//_X_Step_Length += SPEED_STEP;
		//if (_X_Step_Length > SPEED_CORRIDOR)
		//{
			_Y_Step_Length = 0;
			send_control_decision(TURNING_LEFT);
			_Angular_Step_Length = TURN_LEFT_SPEED;
			_X_Step_Length = SPEED_CORNER;
			Tick_Counter = 0;
			Current_Assignment = TURN;
		//}
	}
	#undef  SPEED_STEP
}

void _walk_in_corridor()
{
	if(Right_Sensor == 3 && Skip_Turn == RIGHT_TURN)
	{
		Last_Turn = NO_TURN;
		Back_From_Dead_End = false;
		Skip_Turn = NO_TURN;
	}
	else if(Left_Sensor == 3 && Skip_Turn == LEFT_TURN)
	{
		Last_Turn = NO_TURN;
		Back_From_Dead_End = false;
		Skip_Turn = NO_TURN;
	}
	//-----------------------------------------------------------------
	// Hinder framför robot
	//-----------------------------------------------------------------
	if(Forward_Sensor <= OBJECT_FRONT_MARGIN)
	{
		_start_determin_obstacle();
	}
	//-----------------------------------------------------------------
	//	Korsning
	//-----------------------------------------------------------------
	// Just nu lägre prioritet än hinder framför
	
	// Vanlig reglering
	else                                                                     
	{
		if(Right_Sensor == 0 && Left_Sensor == 0)
		{
			_regulate();
		}
		// Om går förbi korridor på höger sida
		else if(Left_Sensor == 0 && Skip_Turn == RIGHT_TURN)
		{
			_Y_Step_Length = SIDE_K_P*(DECIRED_DISTANCE_SIDE-_Left_Distance);
			if (_Y_Step_Length < -MAX_STRAFE_REGULATION)
			{
				_Y_Step_Length = -MAX_STRAFE_REGULATION;
			}
			else if (_Y_Step_Length > MAX_STRAFE_REGULATION)
			{
				_Y_Step_Length = MAX_STRAFE_REGULATION;
			}
		}
		// Om går förbi korridor på vänster sida
		else if (Right_Sensor == 0 && Skip_Turn == LEFT_TURN)
		{
			_Y_Step_Length = SIDE_K_P*(Right_Distance - DECIRED_DISTANCE_SIDE);
			if (_Y_Step_Length < -MAX_STRAFE_REGULATION)
			{
				_Y_Step_Length = -MAX_STRAFE_REGULATION;
			}
			else if (_Y_Step_Length > MAX_STRAFE_REGULATION)
			{
				_Y_Step_Length = MAX_STRAFE_REGULATION;
			}
		}
		else
		{
			_Angular_Step_Length = 0;
			_Y_Step_Length = 0;
		}
	}
	
	//kollar om kan svänga höger
	if (Right_Sensor == 1)
	{
		if((Last_Turn == LEFT_TURN && Back_From_Dead_End == true) || Skip_Turn == RIGHT_TURN)
		{
			Skip_Turn = RIGHT_TURN;
			// tillbaka vid samma korsning och får inte svänga
			// höger
		}
		else
		{
			Last_Turn = RIGHT_TURN;
			Skip_Turn = NO_TURN;
			Back_From_Dead_End = false;
			_start_turn(TURN_RIGHT_SPEED);
		}
	}
	// kollar om kan svänga vänster
	if (Left_Sensor == 1)
	{
		//_start_go_past_crossing();
		if((Last_Turn == RIGHT_TURN && Back_From_Dead_End == true) || Skip_Turn == LEFT_TURN)
		{
			Skip_Turn = LEFT_TURN;
			// tillbaka vid samma korsning och får inte svänga
			// höger
		}
		else
		{
			Last_Turn = LEFT_TURN;
			Skip_Turn = NO_TURN;
			Back_From_Dead_End = false;
			_start_turn(TURN_LEFT_SPEED);
		}
	}
}

void _turn()
{
	++Tick_Counter;
	if (Tick_Counter >= 6*FRAME_RATE)
	{
		_start_finish_turn();
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
		
		Forward_Sensor_High = Front_Sensor_High;
		
		_Left_Distance = Left_Distance;
		_Right_Distance= Right_Distance;
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
		
		Forward_Sensor_High = Back_sensor_High;
		
		_Left_Distance = Right_Distance;
		_Right_Distance= Left_Distance;
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
		//---------------------------------------------------------------------
		case GO_PAST_CROSSING:
		//---------------------------------------------------------------------
		_go_past_crossing();
		break;
		//---------------------------------------------------------------------
		case DOUBLE_CHECK_SENSOR_VALUE_LEFT:
		//---------------------------------------------------------------------
		_double_check_sensor_value_right();
		break;
		//---------------------------------------------------------------------
		case DOUBLE_CHECK_SENSOR_VALUE_RIGHT:
		//---------------------------------------------------------------------
		_double_check_sensor_value_right();
		break;
		
	}
	
	//Sätter hastigheter
	X_Step_Length = _X_Step_Length * Direction;
	Y_Step_Length = _Y_Step_Length;
	Angular_Step_Length = _Angular_Step_Length;
}