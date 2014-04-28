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

volatile signed char Error; // Avvikelse fr�n mittlinje

volatile signed char Diff_Error; //speed_side_ways i ollan o robbe program

volatile signed char Accumulated_Error; // integraldelen/summering av tidigare fel

volatile signed char Object_Right; // ger ett v�rde 0 d� b�da sensorerna p� h�ger sida ser en v�gg. 1 d� en av sensorerna ser en v�gg och den andra inte, och 2 d� ingen ser.

volatile signed char Object_Left; // motsvarande fast f�r v�nster sida.

volatile signed char Front_Sensor; // fr�mre sensorn

int Last_Front_Sensor; // tidigare v�rde p� front sensorn

#endif /* WISH_REGULATOR_H_ */