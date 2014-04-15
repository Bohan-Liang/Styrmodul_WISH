/*
 * WISH_Gait_Engine.h
 *
 * Skapad: 2014-03-28 21:11:47
 *  Innehåll: Funktioner och globala variabler för gångstilen
 */ 


#ifndef WISH_GAIT_ENGINE_H_
#define WISH_GAIT_ENGINE_H_

#include "Servo_control.h"

// Definiera benens tillstånd under gångstilen
#define TAKE_OFF_AND_LAND	1
#define SWIM_BACK			2

int FRAME_RATE; // Ange hur många del-rörelser en rörelseperiod består av

volatile int Frame_Counter; // Räknare för del-rörelse

// Steglängder för olika rörelseriktningar
volatile float X_Step_Length; // mm
volatile float Y_Step_Length; // mm
volatile int Angular_Step_Length; // grader*10 i heltal

// Temporära parametrar från bussen
volatile float X_Speed; // mm
// volatile float X_Step_Length_temp; // mm
// volatile float Y_Step_Length_temp; // mm
// volatile int Angular_Step_Length_temp; // grader i heltal

// Vinklar för kroppens rotation
volatile float Y_Pitch; // grader
volatile float X_Roll; // grader
volatile float Z_Yaw; // grader

// Tabellerad SIN och COS värden
volatile float SIN_TABLE[401];
volatile float COS_TABLE[401];

//Tabellerad frame_factor
volatile float COS_FRAME_FACTOR[60];
volatile float SIN_FRAME_FACTOR[60];

void init_gait(int frame_rate); // Initiera gångstilen

// Gå till initial läget DIREKT!
void Emergency_Stop();

void set_frame(leg_info* leg, float x_local, float y_local); // Beräkna benets position under en del-rörelse
void tripod_gait(); // Sekvensiera benrörelse
void body_rotate(leg_info* leg, float x_angle, float y_angle, float z_angle); // Rotera kroppen

void tripod_climb_gait(float z_height);
void set_frame_climb(leg_info* leg, float x_local, float y_local, float z_height);



#endif /* WISH_GAIT_ENGINE_H_ */