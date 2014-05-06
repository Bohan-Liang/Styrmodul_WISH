/*
 * WISH_Gait_Engine.h
 *
 * Skapad: 2014-03-28 21:11:47
 *  Inneh�ll: Funktioner och globala variabler f�r g�ngstilen
 */ 


#ifndef WISH_GAIT_ENGINE_H_
#define WISH_GAIT_ENGINE_H_

#include "Servo_control.h"

// Definiera benens tillst�nd under g�ngstilen
#define TAKE_OFF_AND_LAND	1
#define SWIM_BACK			2

volatile int FRAME_RATE; // Ange hur m�nga del-r�relser en r�relseperiod best�r av

volatile int Frame_Counter; // R�knare f�r del-r�relse

// Stegl�ngder f�r olika r�relseriktningar
volatile float X_Step_Length; // mm
volatile float Y_Step_Length; // mm
volatile int Angular_Step_Length; // grader*10 i heltal

// Tempor�ra parametrar fr�n bussen
 volatile float X_Speed; // mm
// volatile float X_Step_Length_temp; // mm
// volatile float Y_Step_Length_temp; // mm
// volatile int Angular_Step_Length_temp; // grader i heltal

// Vinklar f�r kroppens rotation
volatile int Y_Pitch; // grader*10
volatile int X_Roll; // grader*10
volatile int Z_Yaw; // grader*10

// Tabellerad SIN och COS v�rden
volatile float SIN_TABLE[401];
volatile float COS_TABLE[401];

//Tabellerad frame_factor
volatile float COS_FRAME_FACTOR[60];
volatile float SIN_FRAME_FACTOR[60];

void init_gait(int frame_rate); // Initiera g�ngstilen

// G� till initial l�get DIREKT!
void Emergency_Stop();

void set_frame(leg_info* leg, float x_local, float y_local); // Ber�kna benets position under en del-r�relse
void tripod_gait(); // Sekvensiera benr�relse
void body_rotate(leg_info* leg, int x_angle, int y_angle, int z_angle); // Rotera kroppen

void tripod_climb_gait(float z_height);
void set_frame_climb(leg_info* leg, float x_local, float y_local, float z_height);



#endif /* WISH_GAIT_ENGINE_H_ */