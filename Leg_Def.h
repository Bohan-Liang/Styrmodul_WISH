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

// COXA, FEMUR, TIBIA längd
#define COXA_AXIS_DISTANCE		52
#define FEMUR_AXIS_DISTANCE		66
#define TIBIA_AXIS_TO_ENDPOINT	133

// Lägg till justering av koordinater så att (0,0,z) är ändpunkten
double X_OFFSET_FRONT_BACK_LEG;
double Y_OFFSET_FRONT_BACK_LEG;
double Y_OFFSET_MIDDLE_LEG;

// Monteringsvinklar
#define FEMUR_MOUNT_ANGLE		13.5
#define TIBIA_MOUNT_ANGLE		45

// Vi måste ta hänsyn till kroppens(plattan) dimension
#define LEG_MOUNT_X_FRONT_BACK	120
#define LEG_MOUNT_Y_FRONT_BACK	60
#define LEG_MOUNT_Y_MIDDLE		100

// Användbara matematiska konstanter
#define PI						3.141592654
#define SQRT_OF_TWO				1.414213562

// Gruppera lederna
typedef struct
{
	double COXA;
	double FEMUR;
	double TIBIA;
	
	int leg_number;
	
	double init_x;
	double init_y;
	double init_z;
	
	double x_from_center; // x från robotens origo till ändpunkten
	double y_from_center; // y från robotens origo till ändpunkten
}leg_angles;

leg_angles left_front;
leg_angles left_middle;
leg_angles left_back;

leg_angles right_front;
leg_angles right_middle;
leg_angles right_back;

#endif /* LEG_DEF_H_ */