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

// servo vinkel data OBS: data, inte vinkel!
// Globala variabler för lätt åtkomst
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

// COXA, FEMUR, TIBIA längd i mm
#define COXA_AXIS_DISTANCE		52
#define COXA_AXIS_DISTANCE_SQ   2704

#define FEMUR_AXIS_DISTANCE		66
#define FEMUR_AXIS_DISTANCE_SQ  4356

#define TIBIA_AXIS_TO_ENDPOINT	133
#define TIBIA_AXIS_TO_ENDPOINT_SQ 17689

#define FEMUR_TIMES_TIBIA       8778

// Lägg till justering av koordinater så att (0,0,0) är ändpunkten
float X_OFFSET_FRONT_BACK_LEG;
float Y_OFFSET_FRONT_BACK_LEG;
float Y_OFFSET_MIDDLE_LEG;
float Z_OFFSET_LEG;
volatile float Body_Height_Adjust; // Höjd över initial höjden, mm

// Monteringsvinklar
#define FEMUR_MOUNT_ANGLE		13.5
#define TIBIA_MOUNT_ANGLE		45

// Vi måste ta hänsyn till kroppens(plattans) dimension för att vidare utveckla rotation map kroppen
#define LEG_MOUNT_X_FRONT_BACK	120
#define LEG_MOUNT_Y_FRONT_BACK	60
#define LEG_MOUNT_Y_MIDDLE		100

// Användbara matematiska konstanter
#define PI						3.141592654
#define SQRT_OF_TWO				1.414213562

// Gruppera lederna
typedef struct
{
	float COXA; // Leden som monteras på kroppen
	float FEMUR; // Leden i mitten
	float TIBIA; // Leden som har benens ändpunkt
	
	int leg_number;
	
	float init_x; // Initial x-koord map benens monteringsplats
	float init_y; // Initial y-koord map benens monteringsplats
	float init_z; // Initial z-koord map benens monteringsplats
	
	float x_from_center; // x från robotens origo till ändpunkten
	float y_from_center; // y från robotens origo till ändpunkten
	float z_from_center; // z från robotens origo till ändpunkten
	
	// Benens "förföyttning" när kroppen roteras runt sin egna axlar
	volatile float rotate_offset_x; // mm
	volatile float rotate_offset_y; // mm
	volatile float rotate_offset_z; // mm
	
	volatile int Gait_State; // Vid gångstilen änvänds för att bestämma benens beteende
}leg_info;

// Robotens samtliga 6 ben, i form av globala variabler för lätt åtkomst
leg_info left_front;
leg_info left_middle;
leg_info left_back;

leg_info right_front;
leg_info right_middle;
leg_info right_back;

#endif /* LEG_DEF_H_ */