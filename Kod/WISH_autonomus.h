/*
 * WISH_Regulator.h
 *
 * Created: 2014-04-15 14:28:09
 *  Author: Bohan
 */ 


#ifndef WISH_REGULATOR_H_
#define WISH_REGULATOR_H_
#include "WISH_Gait_Engine.h"

//Styrbeslut
#define TURNING_LEFT 1
#define TURNING_RIGHT 2
#define FINISHED_TURNING 3
#define REVERSES 4
#define ROTATING_LEFT 5
#define ROTATING_RIGHT 6
#define DETERMIN_OBSTACLE 7
#define GO_PAST_CROSSING 8
#define DOUBLE_CHECK_SENSOR_VALUE_LEFT 9
#define DOUBLE_CHECK_SENSOR_VALUE_RIGHT 10


// Styrbeslut
#define Turn_left					1
#define Turn_right					2
#define Turn_complete				3
#define Scan_surroundings			4
#define Nothing_detected			5
#define Activate_climbing_mode		6
#define End_climbing_mode			7
#define Switch_direction			8
#define Ignore_open_right_hand_side 9
#define Ignore_open_left_hand_side	10
#define Rotate_right				11
#define Rotate_left					12

void autonomous_operation();
void init_autonomous_operation();
void reset_autonomus_operation();

// kostanter i regler algoritmen
#define Error_Band_Middle 50
//#define Error_Band_Side 10

//#define K_P			1
//#define K_I			0
//#define K_D			4

volatile signed char K_P;
volatile signed char K_D;

volatile signed char Error; // Avvikelse från mittlinje

volatile signed char Diff_Error; //speed_side_ways i ollan o robbe program

volatile signed char Accumulated_Error; // integraldelen/summering av tidigare fel

volatile unsigned char Object_Right; // ger ett värde 0 då båda sensorerna på höger sida ser en vägg. 1 då en av sensorerna ser en vägg och den andra inte, och 2 då ingen ser.

volatile unsigned char Object_Left; // motsvarande fast för vänster sida.

volatile unsigned char Front_Sensor; // främre sensorn

volatile unsigned char Back_Sensor;

volatile unsigned char Object_Front;

volatile unsigned char Object_Back;

volatile unsigned char Left_Distance;
volatile unsigned char Right_Distance;

volatile unsigned char Front_Sensor_High;
volatile unsigned char Back_sensor_High;

float Direction;

unsigned char Forward_Sensor;

unsigned char Backward_Sensor;

unsigned char Type_Front_Sensor;

unsigned char Type_Back_Sensor;

unsigned char Right_Sensor;

unsigned char Left_Sensor;


uint16_t turn_counter;

uint8_t Current_Assignment;
uint16_t Tick_Counter;

#define TURN_RIGHT_SPEED 50
#define TURN_LEFT_SPEED -50

#define WALK_IN_CORRIDOR 0
#define TURN 1

#define FINISH_TURN 3
#define ROTATE 4

#define CLIMB 6


#define FORWARD 1
#define BACKWARD -1

#define MARGIN 1
#define abs(a) ((a < 0) ? -a : a)
#define sign(a) ((a < 0) ? -1 : 1)


#endif /* WISH_REGULATOR_H_ */