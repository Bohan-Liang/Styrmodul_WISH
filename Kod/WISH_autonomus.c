/*
* WISH_Regulator.c
*
* Created: 2014-04-15 14:28:38
*  Author: Bohan
*/

//  Att fixa
// 		Rotation vid �terv�ndsgr�nd
// 		Reglering f�rbi korridor
// 		Kolla sensorer n�r kl�ttrar
// 		B�ttre placering av robot innan rotation
	
// hastighetsl�gen
float speed_corridor;
float speed_corner;
// hastighet 1
#define SPEED_CORRIDOR_S1	32
#define SPEED_CORNER_S1		20
#define FRAME_RATE_S1		30
// hastighet 2
#define SPEED_CORRIDOR_S2	32
#define SPEED_CORNER_S2		20
#define FRAME_RATE_S2		26
// hastighet 3
#define SPEED_CORRIDOR_S3	40
#define SPEED_CORNER_S3		20
#define FRAME_RATE_S3		26
// hastighet 4
#define SPEED_CORRIDOR_S4	40
#define SPEED_CORNER_S4		20
#define FRAME_RATE_S4		24

#include "WISH_autonomus.h"
#include "SPI_Slave.h"

#include <stdbool.h>
// Hinder
#define BARRIER_DETECTED 3
#define OBJECT_FRONT_MARGIN 40
#define OBJECT_FRONT_HALT 25

int Turn_Speed;// ny f�r sv�ng

// Hastigheter
// Har varit 32, 34
#define MEDIUM_SPEED_KORRIDOR 10
#define HALT_SPEED_KORRIDOR 0
#define SPEED_CLIMB 40

#define HEIGHT_OFFSET_CLIMB 40

//Regulator
#define MAX_STRAFE_REGULATION 5
#define STRAFE_K_P 2
#define STRAFE_K_P_AFTER_TURN 4

// Marginaler
#define STRAFE_REGULATOR_MARGIN 5
#define MARGIN_FINISH_CLIMB 35
#define OPEN_FORWARD_MARGIN 70


#define NO_TURN 0
#define RIGHT_TURN 1
#define LEFT_TURN 2
uint8_t Last_Turn;
bool Back_From_Dead_End;
uint8_t Skip_Turn;

// F�r referenssystem
float _X_Step_Length;
float _Y_Step_Length;
int _Angular_Step_Length;
signed char _Error;
signed char _Diff_Error;
unsigned char Forward_Sensor_High;



unsigned char _Left_Distance;
unsigned char _Right_Distance;

int _Autonomus_Frame_Rate;
int _Climb_Frame_Rate;



#define REGULATE_STRAFE(KP)													\
_Y_Step_Length = -KP * Error; 											\
if(abs(_Y_Step_Length) > MAX_STRAFE_REGULATION)							\
_Y_Step_Length = sign(_Y_Step_Length) * MAX_STRAFE_REGULATION

void _start_climb()
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
	send_control_decision(Activate_climbing_mode);
}
void _start_walk_in_corridor()
{
	Tick_Counter = 0;
	_X_Step_Length =speed_corridor;
	_Y_Step_Length = 0;
	_Angular_Step_Length = 0;
	
	Body_Height_Adjust = 0;
	// f�r s�kerhets skull nollst�lls pich h�r
	Y_Pitch = 0;
	set_body_rotation();
	Current_Assignment = WALK_IN_CORRIDOR;
}
bool is_turning;
void _start_turn(int dir)
{
	_Y_Step_Length = 0;
	if(dir == TURN_LEFT_SPEED)
	{
		send_control_decision(Turn_left);
		//_start_double_check_sensor_value_left();
	}
	else
	{
		send_control_decision(Turn_right);
		//_start_double_check_sensor_value_right();
	}
	
	_Angular_Step_Length = 0;//------------------------------------
	Turn_Speed = dir;
	
	is_turning = false;
	
	_X_Step_Length = speed_corner;
	Tick_Counter = 0;
	Current_Assignment = TURN;
}
void _start_finish_turn()
{
	_X_Step_Length = speed_corridor;
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
	//G�r tills ser korridor r�knar sedan upp Tick counter
	if((Right_Sensor != 2) && (Left_Sensor != 2)) //
	{
		//_X_Step_Length=0;
		if(Tick_Counter == 0)
			Tick_Counter=1;
			
		REGULATE_STRAFE(STRAFE_K_P);
	}
	
	if(Tick_Counter > 0)
		++Tick_Counter;
	
	/*if(Tick_Counter > 0 && abs(Error) <= 2)
	{
		_start_walk_in_corridor();
	}
	else if(Tick_Counter >= FRAME_RATE/2)
	{
		_start_walk_in_corridor();
	}*/
	if(Tick_Counter >= FRAME_RATE)
	{
		send_control_decision(Turn_complete);
		_start_walk_in_corridor();
	}
	//else if (Tick_Counter > 0)
	//{
	//	_start_walk_in_corridor();
		//++Tick_Counter;
	//}
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
 		_Y_Step_Length = -SIDE_K_P*(DECIRED_DISTANCE_SIDE-_Left_Distance); 
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
 		_Y_Step_Length = -SIDE_K_P*(Right_Distance - DECIRED_DISTANCE_SIDE);
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
	#define  MAX_REGULATOR 40
	if(Regulator <= MAX_REGULATOR && Regulator >= -MAX_REGULATOR)
	{
		_Angular_Step_Length = Regulator;
	}
	else if (Regulator < -MAX_REGULATOR)
	{
		_Angular_Step_Length = -MAX_REGULATOR;
	}
	else if (Regulator > MAX_REGULATOR)
	{
		_Angular_Step_Length = MAX_REGULATOR;
	}
	#undef MAX_REGULATOR
	// Strafe reglering
	if(abs(_Error) >= STRAFE_REGULATOR_MARGIN)
	{
		REGULATE_STRAFE(STRAFE_K_P);
	}
	else
	{
		_Y_Step_Length = 0;
	}
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
		return 2;
		break;
	}
}
// k�rs efter ett kort avst�nd fram uppst�tt.
// dess uppgift �r att best�mma om det bara var en st�rning
// eller om och hur roboten ska agera
#define MS_TO_TICK(ms) ms*FRAME_RATE
void _start_determin_obstacle()
{
	//Emergency_Stop();
	//_X_Step_Length = HALT_SPEED_KORRIDOR;
	Tick_Counter = 0;
	_Y_Step_Length = 0;
	_Angular_Step_Length=0;
	Y_Pitch = - 20 * Direction;
	Current_Assignment = DETERMIN_OBSTACLE;
	
}
void _determin_obstacle()
{
	#define  X_SPEED_STEP 10
	if (Forward_Sensor > OBJECT_FRONT_MARGIN)
	{
		Y_Pitch = 0;
		Z_Yaw = 0;
		set_body_rotation();
		send_control_decision(Nothing_detected);
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
		Y_Pitch = 0;
		Z_Yaw = 0;
		set_body_rotation();
		if (Type_Front_Sensor == BARRIER_DETECTED)
		{
			//Y_Pitch = 0; kl�ttring s�tter Y_Pitch
			_start_climb();
		}
		
		else
		{
			Back_From_Dead_End = true;
			Direction = -Direction;
			send_control_decision(Switch_direction);
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
// 		// kl�tterhinder
// 		if(Type_Front_Sensor == BARRIER_DETECTED)
// 		{
// 			_start_climb();
// 		}
// 		// korsning/sv�ng eller �terv�ndsgr�nd
// 		else
// 		{
// 			//v�nd om
// 			if (Right_Sensor == 0 && Left_Sensor == 0)
// 			{
// 				Back_From_Dead_End = true;
// 				Direction = -Direction;
// 				_start_walk_in_corridor();
// 			}
// 			//rotera h�ger
// 			else if (Right_Sensor != 0)
// 			{
// 				_Y_Step_Length = 0;
// 				_X_Step_Length = 0;
// 				_Angular_Step_Length = TURN_RIGHT_SPEED;
// 				Tick_Counter = 0;
// 				Y_Pitch = 0;
// 				Current_Assignment = ROTATE;
// 			}
// 			//rotera v�nster
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
// 	// forts�tt i detta l�ge
}

void _climb()
{
	//tvingar den att g� n�gra steg fram�t
	if(Tick_Counter <= 5*FRAME_RATE)
	{
		++Tick_Counter;
	}
	// g�r tills m�nstret p� baksensorn intr�ffar
	else
	{
		if(Backward_Sensor <= MARGIN_FINISH_CLIMB && Tick_Counter > 8*FRAME_RATE)
		{
			Y_Pitch = 0;
			set_body_rotation();
			FRAME_RATE = _Autonomus_Frame_Rate;
			send_control_decision(End_climbing_mode);
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
				REGULATE_STRAFE(STRAFE_K_P);
			}
			else
			{
				_Y_Step_Length = 0;
			}
		}
	}
}


void _walk_in_corridor()
{
	if(Skip_Turn != NO_TURN)
	{
		if(Tick_Counter >= 20 * FRAME_RATE )//1.28 m
		{
			Last_Turn = NO_TURN;
			Skip_Turn = NO_TURN;
			Tick_Counter = 0;
			Back_From_Dead_End = false;
		}
		else
		{
			Tick_Counter++;
		}
	}
	
	/*if(Right_Sensor == 3 && Skip_Turn == RIGHT_TURN)
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
	}*/
	//-----------------------------------------------------------------
	// Hinder framf�r robot
	//-----------------------------------------------------------------
	if(Forward_Sensor <= OBJECT_FRONT_MARGIN)
	{
		_start_determin_obstacle();
	}
	//-----------------------------------------------------------------
	//	Korsning
	//-----------------------------------------------------------------
	// Just nu l�gre prioritet �n hinder framf�r
	
	// Vanlig reglering
	else                                                                     
	{
		if(Right_Sensor == 0 && Left_Sensor == 0)
		{
			_regulate();
		}
		// st�ngt till v�nster
		else if( Skip_Turn == RIGHT_TURN && Left_Sensor != 2)
		{
			_Angular_Step_Length = 0.5*(DECIRED_DISTANCE_SIDE - _Left_Distance);
			if(_Angular_Step_Length > 40)
			{
				_Angular_Step_Length= 40;
			}
			
			if(_Angular_Step_Length < -40)
			{
				_Angular_Step_Length= -40;
			}
		}
		else if (Skip_Turn == LEFT_TURN && Right_Sensor != 2)
		{	
			_Angular_Step_Length = 0.5*(_Right_Distance - DECIRED_DISTANCE_SIDE);
			if(_Angular_Step_Length > 40)
			{
				_Angular_Step_Length= 40;
			}
			else if(_Angular_Step_Length <-40)
			{
				_Angular_Step_Length = -40;
			}
		}
			
			/*_Y_Step_Length = -SIDE_K_P*(DECIRED_DISTANCE_SIDE-_Left_Distance);
			if (_Y_Step_Length < -MAX_STRAFE_REGULATION)
			{
				_Y_Step_Length = -MAX_STRAFE_REGULATION;
			}
			else if (_Y_Step_Length > MAX_STRAFE_REGULATION)
			{
				_Y_Step_Length = MAX_STRAFE_REGULATION;
			}*/
		/*}
		
		// Om g�r f�rbi korridor p� v�nster sida
		else if ((Object_Right == 0 || Object_Left != 0)  && Skip_Turn != NO_TURN) // 2014-05-14 lagt till extra villkor f�r att s�kerst�lla kravet
		{
			_Angular_Step_Length = 0.5*(Right_Distance - DECIRED_DISTANCE_SIDE)*Direction;
			if(_Angular_Step_Length > 40)
			{
				_Angular_Step_Length= 40;
			}
			else if(_Angular_Step_Length <-40)
			{
				_Angular_Step_Length = -40;
			}
			
			_Y_Step_Length = -SIDE_K_P*(Right_Distance - DECIRED_DISTANCE_SIDE);
			if (_Y_Step_Length < -MAX_STRAFE_REGULATION)
			{
				_Y_Step_Length = -MAX_STRAFE_REGULATION;
			}
			else if (_Y_Step_Length > MAX_STRAFE_REGULATION)
			{
				_Y_Step_Length = MAX_STRAFE_REGULATION;
			}
		//}*/
		
		else
		{
			_Angular_Step_Length = 0;
			_Y_Step_Length = 0;
		}
	}
	
	//kollar om kan sv�nga h�ger
	if (Right_Sensor == 1)
	{
		if((Last_Turn == LEFT_TURN && Back_From_Dead_End == true) || Skip_Turn == RIGHT_TURN)
		{
			if( Skip_Turn != RIGHT_TURN)
			{
				Tick_Counter = 0;
			}
			Skip_Turn = RIGHT_TURN;
			// tillbaka vid samma korsning och f�r inte sv�nga
			// h�ger
		}
		else
		{
			Last_Turn = RIGHT_TURN;
			Skip_Turn = NO_TURN;
			Back_From_Dead_End = false;
			_start_turn(TURN_RIGHT_SPEED);
		}
	}
	// kollar om kan sv�nga v�nster
	else if (Left_Sensor == 1)
	{
		//_start_go_past_crossing();
		if((Last_Turn == RIGHT_TURN && Back_From_Dead_End == true) || Skip_Turn == LEFT_TURN)
		{
			if( Skip_Turn != LEFT_TURN)
			{
				Tick_Counter = 0;
			}
			Skip_Turn = LEFT_TURN;
			// tillbaka vid samma korsning och f�r inte sv�nga
			// h�ger
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
	// kollar om den redan b�rjat sv�nga
	if(is_turning)
	{
		if (Tick_Counter >= 7*FRAME_RATE + 12/30*FRAME_RATE) // var + 12
		{
			_start_finish_turn();
		}
	}
	// kollar om g�tt tillr�cligt l�ngt fram f�r att b�rja sv�nga
	else if((Tick_Counter > (2*FRAME_RATE-FRAME_RATE/4)) && !is_turning)
	{
		
		// kollar om redo att b�rja sv�nga
		if(Frame_Counter == FRAME_RATE/2)
		{
			_Angular_Step_Length = Turn_Speed;
			is_turning = true;
			Tick_Counter = 0;
			
			if(Turn_Speed == TURN_RIGHT_SPEED)
			{
				//kollar om fortfarande �ppet
				if(Right_Sensor != 1 && Right_Sensor != 2)
				{
					_start_walk_in_corridor();
				}
				
			}
			else if(Turn_Speed == TURN_LEFT_SPEED)
			{
				//kollar om fortfarande �ppet
				if(Left_Sensor != 1 && Left_Sensor != 2)
				{
					_start_walk_in_corridor();
				}
			}
		}	
	}		
}

void _rotate()
{
	++Tick_Counter;
	if (Tick_Counter > 6*FRAME_RATE)
	{
		_start_walk_in_corridor();
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
	}
	
	//S�tter hastigheter
	X_Step_Length = _X_Step_Length * Direction;
	Y_Step_Length = _Y_Step_Length;
	Angular_Step_Length = _Angular_Step_Length;
}

void init_autonomous_operation()
{
	Forward_Sensor_High = UINT8_MAX;
	Last_Turn = NO_TURN;
	Back_From_Dead_End = false;
	Skip_Turn = NO_TURN;
	
	_Autonomus_Frame_Rate = FRAME_RATE_S1;
	_Climb_Frame_Rate = 48;
	FRAME_RATE = _Autonomus_Frame_Rate;
	
	Error = 0;
	Accumulated_Error = 0;
	Diff_Error = 0;
	K_P = 1;
	K_D = 4;
	
	_Error = 0;
	_Diff_Error = 0;
	
	speed_corridor = SPEED_CORRIDOR_S1;
	speed_corner = SPEED_CORNER_S1;
	
	X_Step_Length = speed_corridor;
	Angular_Step_Length = 0;
	Y_Step_Length = 0;
	
	_X_Step_Length = speed_corridor;
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

	Front_Sensor_High = UINT8_MAX;
	Back_sensor_High = UINT8_MAX;
	
	Object_Front = 0;

	Object_Back = 0;
}
void reset_autonomus_operation(uint8_t autionomus_speed)
{
	Forward_Sensor_High = UINT8_MAX;
	Last_Turn = NO_TURN;
	Back_From_Dead_End = false;
	Skip_Turn = NO_TURN;
	
	_Climb_Frame_Rate = 48;
	FRAME_RATE = _Autonomus_Frame_Rate;
	
	Error = 0;
	Accumulated_Error = 0;
	Diff_Error = 0;
	K_P = 1;
	K_D = 4;
	
	_Error = 0;
	_Diff_Error = 0;
	
	X_Step_Length = speed_corridor;
	Angular_Step_Length = 0;
	Y_Step_Length = 0;
	
	_X_Step_Length = speed_corridor;
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

	Front_Sensor_High = UINT8_MAX;
	Back_sensor_High = UINT8_MAX;
	
	Object_Front = 0;

	Object_Back = 0;
	switch(autionomus_speed)
	{
		case 1:
			speed_corridor = SPEED_CORRIDOR_S1;
			speed_corner = SPEED_CORNER_S1;
			_Autonomus_Frame_Rate = FRAME_RATE_S1;
		break;
		
		case 2:
			speed_corridor = SPEED_CORRIDOR_S2;
			speed_corner = SPEED_CORNER_S2;
			_Autonomus_Frame_Rate = FRAME_RATE_S2;
		break;
		
		case 3:
			speed_corridor = SPEED_CORRIDOR_S3;
			speed_corner = SPEED_CORNER_S3;
			_Autonomus_Frame_Rate = FRAME_RATE_S3;
		break;
		
		case 4:
			speed_corridor = SPEED_CORRIDOR_S3;
			speed_corner = SPEED_CORNER_S3;
			_Autonomus_Frame_Rate = FRAME_RATE_S3;
		break;
	}
}