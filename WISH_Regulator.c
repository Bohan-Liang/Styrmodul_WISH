/*
 * WISH_Regulator.c
 *
 * Created: 2014-04-15 14:28:38
 *  Author: Bohan
 */ 
#include "WISH_Regulator.h"

void test_regulator(int KP)
{
	X_Step_Length = 20;
	int P_reg = -KP*Error;
	if(P_reg <= 100 && P_reg >= -100)
	{
		Angular_Step_Length = P_reg;
	}
	else if (P_reg < -100)
	{
		Angular_Step_Length = -100;
	}
	else if (P_reg > 100)
	{
		Angular_Step_Length = 100;
	}
}