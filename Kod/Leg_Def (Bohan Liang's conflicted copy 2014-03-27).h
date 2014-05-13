/*
 * Leg_Def.h
 *
 * Created: 2014-03-26 09:29:49
 *  Author: Bohan
 */ 


#ifndef LEG_DEF_H_
#define LEG_DEF_H_

// Servo ID
//-----------------------------
#define ID_FRONT_LEFT_COXA		1
#define ID_FRONT_LEFT_FEMUR		3
#define ID_FRONT_LEFT_TIBIA		5

#define ID_MIDDLE_LEFT_COXA		13
#define ID_MIDDLE_LEFT_FEMUR	15
#define ID_MIDDLE_LEFT_TIBIA	17

#define	ID_BACK_LEFT_COXA		7
#define	ID_BACK_LEFT_FEMUR		9
#define	ID_BACK_LEFT_TIBIA		11
//-----------------------------
#define ID_FRONT_RIGHT_COXA		2
#define ID_FRONT_RIGHT_FEMUR	4
#define ID_FRONT_RIGHT_TIBIA	6

#define ID_MIDDLE_RIGHT_COXA	14
#define ID_MIDDLE_RIGHT_FEMUR	16
#define ID_MIDDLE_RIGHT_TIBIA	18

#define ID_BACK_RIGHT_COXA		8
#define ID_BACK_RIGHT_FEMUR		10
#define ID_BACK_RIGHT_TIBIA		12
//-----------------------------

/* Ben nummer
#define FRONT_LEFT		0
#define MIDDLE_LEFT		1
#define BACK_LEFT		2

#define FRONT_RIGHT		3
#define MIDDLE_RIGHT	4
#define BACK_RIGHT		5
*/

// servo vinkel data OBS: data, inte vinkel!
volatile int ANGLE_FRONT_LEFT_COXA;
volatile int ANGLE_FRONT_LEFT_FEMUR;
volatile int ANGLE_FRONT_LEFT_TIBIA;

volatile int ANGLE_MIDDLE_LEFT_COXA;
volatile int ANGLE_MIDDLE_LEFT_FEMUR;
volatile int ANGLE_MIDDLE_LEFT_TIBIA;

volatile int ANGLE_BACK_LEFT_COXA;
volatile int ANGLE_BACK_LEFT_FEMUR;
volatile int ANGLE_BACK_LEFT_TIBIA;
//-----------------------------
volatile int ANGLE_FRONT_RIGHT_COXA;
volatile int ANGLE_FRONT_RIGHT_FEMUR;
volatile int ANGLE_FRONT_RIGHT_TIBIA;

volatile int ANGLE_MIDDLE_RIGHT_COXA;
volatile int ANGLE_MIDDLE_RIGHT_FEMUR;
volatile int ANGLE_MIDDLE_RIGHT_TIBIA;

volatile int ANGLE_BACK_RIGHT_COXA;
volatile int ANGLE_BACK_RIGHT_FEMUR;
volatile int ANGLE_BACK_RIGHT_TIBIA;

// COXA, FEMUR, TIBIA längd: mäta!
#define COXA_AXIS_DISTANCE		52
#define FEMUR_AXIS_DISTANCE		66
#define TIBIA_AXIS_TO_ENDPOINT	132


// Gruppera lederna
typedef struct
{
	double COXA;
	double FEMUR;
	double TIBIA;
	int leg_number;
}leg_angles;

leg_angles left_front;
leg_angles left_middle;
leg_angles left_back;

leg_angles right_front;
leg_angles right_middle;
leg_angles right_back;

#endif /* LEG_DEF_H_ */