/*
 * WISH_Regulator.h
 *
 * Created: 2014-04-15 14:28:09
 *  Author: Bohan
 */ 


#ifndef WISH_REGULATOR_H_
#define WISH_REGULATOR_H_
#include "WISH_Gait_Engine.h"

void PID_regulator();
void init_regulator();
void turning_corners(int corner);

// kostanter i regler algoritmen
#define Error_Band_Middle 50
//#define Error_Band_Side 10

#define K_P			1
#define K_I			0
#define K_D			4

volatile signed char Error; // Avvikelse från mittlinje

volatile signed char Diff_Error; //speed_side_ways i ollan o robbe program

volatile signed char Accumulated_Error; // integraldelen/summering av tidigare fel

volatile signed char Object_Right; // ger ett värde 0 då båda sensorerna på höger sida ser en vägg. 1 då en av sensorerna ser en vägg och den andra inte, och 2 då ingen ser.

volatile signed char Object_Left; // motsvarande fast för vänster sida.

volatile signed char Front_Sensor; // främre sensorn

int Last_Front_Sensor; // tidigare värde på front sensorn

#endif /* WISH_REGULATOR_H_ */