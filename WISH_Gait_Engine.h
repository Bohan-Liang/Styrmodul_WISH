/*
 * WISH_Gait_Engine.h
 *
 * Created: 2014-03-28 21:11:47
 *  Author: Bohan
 */ 


#ifndef WISH_GAIT_ENGINE_H_
#define WISH_GAIT_ENGINE_H_

#include "Servo_control.h"

// Del-rörelse av ben
#define TAKE_OFF_AND_LAND	1
#define SWIM_BACK			2

//int UPDATE_INTERVAL; // uppdaterings intervall i ms
int FRAME_RATE;

volatile int Frame_Counter;
volatile double X_Step_Length; // mm
volatile double Y_Step_Length; // mm
volatile double Angular_Step_Length; // grader

volatile double Y_Pitch; // grader
volatile double X_Roll; // grader
volatile double Z_Yaw; // grader

void init_gait(int frame_rate);
void set_frame(leg_info* leg, double x_local, double y_local);
void tripod_gait();
void body_rotate(leg_info* leg, double x_angle, double y_angle, double z_angle);


#endif /* WISH_GAIT_ENGINE_H_ */