/*
 * WISH_Regulator.h
 *
 * Created: 2014-04-15 14:28:09
 *  Author: Bohan
 */ 


#ifndef WISH_REGULATOR_H_
#define WISH_REGULATOR_H_
#include "WISH_Gait_Engine.h"

void test_regulator(int KP);
void init_regulator();

// kostanter i regler algoritmen
#define Error_Band_Middle 50
//#define Error_Band_Side 10

#define K_P			1
#define K_I			1
#define K_D			1

volatile signed char Error; // Avvikelse från mittlinje



float Accumulated_Error;





#endif /* WISH_REGULATOR_H_ */