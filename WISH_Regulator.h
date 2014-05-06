/*
 * WISH_Regulator.h
 *
 * Created: 2014-04-15 14:28:09
 *  Author: Bohan
 */ 


#ifndef WISH_REGULATOR_H_
#define WISH_REGULATOR_H_
#include "WISH_Gait_Engine.h"

#define TURNING_LEFT 1
#define TURNING_RIGHT 2
#define REVERSES 3


void PID_regulator();
void init_regulator();
void turning_corners(int corner);

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

float Direction;

unsigned char Forward_Sensor;

unsigned char Type_Front_Sensor;

unsigned char Type_Back_Sensor;

unsigned char Right_Sensor;

unsigned char Left_Sensor;


volatile char MODE;

volatile char turn;

uint16_t turn_counter;

#define TURN_RIGHT 51
#define TURN_LEFT -51

#define FORWARD 1
#define BACKWARD -1



#endif /* WISH_REGULATOR_H_ */