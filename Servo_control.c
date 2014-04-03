/*
 * Servo_control.c
 *
 * Created: 2014-03-24 21:00:53
 *  Author: Bohan
 */ 

#include "Servo_control.h"


void init_USART(long baud_rate)
{
	UBRR0H = ((F_CPU/16) / baud_rate - 1) >> 8;
	UBRR0L = ((F_CPU/16) / baud_rate - 1);
}

void USART_receive_mode()
{ 
	// Vänta tills buffer blir tomt
	while(BIT_IS_CLEAR(UCSR0A,UDRE0));

	BIT_CLEAR(UCSR0B,TXEN0);
	
	// läsning av inkommande data från servon sker med avbrott
	BIT_SET(UCSR0B,RXCIE0);
	
	BIT_SET(UCSR0B,RXEN0);
	
	buffer_index = 0;
}

void USART_transmit_mode()
{
	BIT_CLEAR(UCSR0B,RXEN0);

	BIT_SET(UCSR0B,TXEN0);
		
	// stäng avbrott från mottagare
	BIT_CLEAR(UCSR0B,RXCIE0);
}

void send_to_servo(unsigned char data)
{
	// Vänta tills buffer blir tomt
	while(BIT_IS_CLEAR(UCSR0A,UDRE0));
	UDR0 = data; // Sända!
}

// Avbrottsrutin för mottagning av data
ISR(USART0_RX_vect)
{
	// lagra i buffert
	reception_buffer[buffer_index++] = UDR0;
}

int angle_to_data(double angle)
{
	return round(1024*(angle + 150)/300);
}

void translate_leg_angle()
{
	ANGLE_FRONT_LEFT_COXA = round(1024*(left_front.COXA + 150)/300);
	ANGLE_FRONT_LEFT_FEMUR = round(1024*(left_front.FEMUR + 150)/300);
	ANGLE_FRONT_LEFT_TIBIA = round(1024*(left_front.TIBIA + 150)/300);

	ANGLE_MIDDLE_LEFT_COXA = round(1024*(left_middle.COXA + 150)/300);
	ANGLE_MIDDLE_LEFT_FEMUR = round(1024*(left_middle.FEMUR + 150)/300);
	ANGLE_MIDDLE_LEFT_TIBIA = round(1024*(left_middle.TIBIA + 150)/300);

	ANGLE_BACK_LEFT_COXA = round(1024*(left_back.COXA + 150)/300);
	ANGLE_BACK_LEFT_FEMUR = round(1024*(left_back.FEMUR + 150)/300);
	ANGLE_BACK_LEFT_TIBIA = round(1024*(left_back.TIBIA + 150)/300);
	//-----------------------------
	ANGLE_FRONT_RIGHT_COXA = round(1024*(right_front.COXA + 150)/300);
	ANGLE_FRONT_RIGHT_FEMUR = round(1024*(right_front.FEMUR + 150)/300);
	ANGLE_FRONT_RIGHT_TIBIA = round(1024*(right_front.TIBIA + 150)/300);

	ANGLE_MIDDLE_RIGHT_COXA = round(1024*(right_middle.COXA + 150)/300);
	ANGLE_MIDDLE_RIGHT_FEMUR = round(1024*(right_middle.FEMUR + 150)/300);
	ANGLE_MIDDLE_RIGHT_TIBIA = round(1024*(right_middle.TIBIA + 150)/300);

	ANGLE_BACK_RIGHT_COXA = round(1024*(right_back.COXA + 150)/300);
	ANGLE_BACK_RIGHT_FEMUR = round(1024*(right_back.FEMUR + 150)/300);
	ANGLE_BACK_RIGHT_TIBIA = round(1024*(right_back.TIBIA + 150)/300);
}

void write_to_all()
{
	int sum = 0xfe + 58 + SYNC_WRITE + GOAL_POSITION_L + 2 + ID_FRONT_LEFT_COXA + 
				(ANGLE_FRONT_LEFT_COXA & 0xff) + (ANGLE_FRONT_LEFT_COXA >> 8) 
				+ ID_FRONT_LEFT_FEMUR + (ANGLE_FRONT_LEFT_FEMUR & 0xff) + 
				(ANGLE_FRONT_LEFT_FEMUR >> 8) + ID_FRONT_LEFT_TIBIA + 
				(ANGLE_FRONT_LEFT_TIBIA & 0xff) + (ANGLE_FRONT_LEFT_TIBIA >> 8) +
				ID_MIDDLE_LEFT_COXA + (ANGLE_MIDDLE_LEFT_COXA & 0xff) +
				(ANGLE_MIDDLE_LEFT_COXA >> 8) + ID_MIDDLE_LEFT_FEMUR + 
				(ANGLE_MIDDLE_LEFT_FEMUR & 0xff) + (ANGLE_MIDDLE_LEFT_FEMUR >> 8) +
				ID_MIDDLE_LEFT_TIBIA + (ANGLE_MIDDLE_LEFT_TIBIA & 0xff) + 
				(ANGLE_MIDDLE_LEFT_TIBIA >> 8) + ID_BACK_LEFT_COXA + 
				(ANGLE_BACK_LEFT_COXA & 0xff) + (ANGLE_BACK_LEFT_COXA >> 8) +
				
				ID_BACK_LEFT_FEMUR +
				(ANGLE_BACK_LEFT_FEMUR & 0xff) + (ANGLE_BACK_LEFT_FEMUR >> 8) +
				
				ID_BACK_LEFT_TIBIA +
				(ANGLE_BACK_LEFT_TIBIA & 0xff) + (ANGLE_BACK_LEFT_TIBIA >> 8) +
				
				ID_FRONT_RIGHT_COXA +
				(ANGLE_FRONT_RIGHT_COXA & 0xff) + (ANGLE_FRONT_RIGHT_COXA >> 8)
				+ ID_FRONT_RIGHT_FEMUR + (ANGLE_FRONT_RIGHT_FEMUR & 0xff) +
				(ANGLE_FRONT_RIGHT_FEMUR >> 8) + ID_FRONT_RIGHT_TIBIA +
				(ANGLE_FRONT_RIGHT_TIBIA & 0xff) + (ANGLE_FRONT_RIGHT_TIBIA >> 8) +
				ID_MIDDLE_RIGHT_COXA + (ANGLE_MIDDLE_RIGHT_COXA & 0xff) +
				(ANGLE_MIDDLE_RIGHT_COXA >> 8) + ID_MIDDLE_RIGHT_FEMUR +
				(ANGLE_MIDDLE_RIGHT_FEMUR & 0xff) + (ANGLE_MIDDLE_RIGHT_FEMUR >> 8) +
				ID_MIDDLE_RIGHT_TIBIA + (ANGLE_MIDDLE_RIGHT_TIBIA & 0xff) +
				(ANGLE_MIDDLE_RIGHT_TIBIA >> 8) + ID_BACK_RIGHT_COXA +
				(ANGLE_BACK_RIGHT_COXA & 0xff) + (ANGLE_BACK_RIGHT_COXA >> 8) +
				
				ID_BACK_RIGHT_FEMUR +
				(ANGLE_BACK_RIGHT_FEMUR & 0xff) + (ANGLE_BACK_RIGHT_FEMUR >> 8) +
				
				ID_BACK_RIGHT_TIBIA +
				(ANGLE_BACK_RIGHT_TIBIA & 0xff) + (ANGLE_BACK_RIGHT_TIBIA >> 8);
				
				
				
	int checksum = ~(sum % 256);
	USART_transmit_mode();
	send_to_servo(0xff);
	send_to_servo(0xff);
	
	send_to_servo(0xfe); // Broadcast ID
	send_to_servo(58);
	send_to_servo(SYNC_WRITE);
	send_to_servo(GOAL_POSITION_L);
	send_to_servo(2);
	
	send_to_servo(ID_FRONT_LEFT_COXA);
	send_to_servo(ANGLE_FRONT_LEFT_COXA & 0xff);
	send_to_servo(ANGLE_FRONT_LEFT_COXA >> 8);
	
	send_to_servo(ID_FRONT_LEFT_FEMUR);
	send_to_servo(ANGLE_FRONT_LEFT_FEMUR & 0xff);
	send_to_servo(ANGLE_FRONT_LEFT_FEMUR >> 8);
	
	send_to_servo(ID_FRONT_LEFT_TIBIA);
	send_to_servo(ANGLE_FRONT_LEFT_TIBIA & 0xff);
	send_to_servo(ANGLE_FRONT_LEFT_TIBIA >> 8);
	
	send_to_servo(ID_MIDDLE_LEFT_COXA);
	send_to_servo(ANGLE_MIDDLE_LEFT_COXA & 0xff);
	send_to_servo(ANGLE_MIDDLE_LEFT_COXA >> 8);
	
	send_to_servo(ID_MIDDLE_LEFT_FEMUR);
	send_to_servo(ANGLE_MIDDLE_LEFT_FEMUR & 0xff);
	send_to_servo(ANGLE_MIDDLE_LEFT_FEMUR >> 8);
	
	send_to_servo(ID_MIDDLE_LEFT_TIBIA);
	send_to_servo(ANGLE_MIDDLE_LEFT_TIBIA & 0xff);
	send_to_servo(ANGLE_MIDDLE_LEFT_TIBIA >> 8);
	
	send_to_servo(ID_BACK_LEFT_COXA);
	send_to_servo(ANGLE_BACK_LEFT_COXA & 0xff);
	send_to_servo(ANGLE_BACK_LEFT_COXA >> 8);
	
	send_to_servo(ID_BACK_LEFT_FEMUR);
	send_to_servo(ANGLE_BACK_LEFT_FEMUR & 0xff);
	send_to_servo(ANGLE_BACK_LEFT_FEMUR >> 8);
	
	send_to_servo(ID_BACK_LEFT_TIBIA);
	send_to_servo(ANGLE_BACK_LEFT_TIBIA & 0xff);
	send_to_servo(ANGLE_BACK_LEFT_TIBIA >> 8);
	
	send_to_servo(ID_FRONT_RIGHT_COXA);
	send_to_servo(ANGLE_FRONT_RIGHT_COXA & 0xff);
	send_to_servo(ANGLE_FRONT_RIGHT_COXA >> 8);
	
	send_to_servo(ID_FRONT_RIGHT_FEMUR);
	send_to_servo(ANGLE_FRONT_RIGHT_FEMUR & 0xff);
	send_to_servo(ANGLE_FRONT_RIGHT_FEMUR >> 8);
	
	send_to_servo(ID_FRONT_RIGHT_TIBIA);
	send_to_servo(ANGLE_FRONT_RIGHT_TIBIA & 0xff);
	send_to_servo(ANGLE_FRONT_RIGHT_TIBIA >> 8);
	
	send_to_servo(ID_MIDDLE_RIGHT_COXA);
	send_to_servo(ANGLE_MIDDLE_RIGHT_COXA & 0xff);
	send_to_servo(ANGLE_MIDDLE_RIGHT_COXA >> 8);
	
	send_to_servo(ID_MIDDLE_RIGHT_FEMUR);
	send_to_servo(ANGLE_MIDDLE_RIGHT_FEMUR & 0xff);
	send_to_servo(ANGLE_MIDDLE_RIGHT_FEMUR >> 8);
	
	send_to_servo(ID_MIDDLE_RIGHT_TIBIA);
	send_to_servo(ANGLE_MIDDLE_RIGHT_TIBIA & 0xff);
	send_to_servo(ANGLE_MIDDLE_RIGHT_TIBIA >> 8);
	
	send_to_servo(ID_BACK_RIGHT_COXA);
	send_to_servo(ANGLE_BACK_RIGHT_COXA & 0xff);
	send_to_servo(ANGLE_BACK_RIGHT_COXA >> 8);
	
	send_to_servo(ID_BACK_RIGHT_FEMUR);
	send_to_servo(ANGLE_BACK_RIGHT_FEMUR & 0xff);
	send_to_servo(ANGLE_BACK_RIGHT_FEMUR >> 8);
	
	send_to_servo(ID_BACK_RIGHT_TIBIA);
	send_to_servo(ANGLE_BACK_RIGHT_TIBIA & 0xff);
	send_to_servo(ANGLE_BACK_RIGHT_TIBIA >> 8);
	
	send_to_servo(checksum);
	//USART_receive_mode();
}

void init_leg(double init_x, double init_y, double init_z)
{
	X_OFFSET_FRONT_BACK_LEG = 0; // Ignorera X_offset för att ta fram initialpositioner
	
	left_front.leg_number = 0;
	left_middle.leg_number = 1;
	left_back.leg_number = 2;
	
	right_front.leg_number = 3;
	right_middle.leg_number = 4;
	right_back.leg_number = 5;
	
	left_front.init_x = init_x/SQRT_OF_TWO + init_y/SQRT_OF_TWO;
	left_front.init_y = init_y/SQRT_OF_TWO - init_x/SQRT_OF_TWO;
	left_front.init_z = init_z;
	
	left_middle.init_x = init_x;
	left_middle.init_y = init_y;
	left_middle.init_z = init_z;
	
	left_back.init_x = init_x/SQRT_OF_TWO - init_y/SQRT_OF_TWO;
	left_back.init_y = init_y/SQRT_OF_TWO + init_x/SQRT_OF_TWO;
	left_back.init_z = init_z;
	
	right_front.init_x = init_x/SQRT_OF_TWO + init_y/SQRT_OF_TWO;
	right_front.init_y = init_y/SQRT_OF_TWO - init_x/SQRT_OF_TWO;
	right_front.init_z = init_z;
	
	right_middle.init_x = init_x;
	right_middle.init_y = init_y;
	right_middle.init_z = init_z;
	
	right_back.init_x = init_x/SQRT_OF_TWO - init_y/SQRT_OF_TWO;
	right_back.init_y = init_y/SQRT_OF_TWO + init_x/SQRT_OF_TWO;
	right_back.init_z = init_z;

	set_pos_leg(&left_front, left_front.init_x, left_front.init_y ,left_front.init_z);
	set_pos_leg(&left_middle,left_middle.init_x, left_middle.init_y, left_middle.init_z);
	set_pos_leg(&left_back, left_back.init_x, left_back.init_y, left_back.init_z);
	
	set_pos_leg(&right_front, right_front.init_x, -right_front.init_y ,right_front.init_z);
	set_pos_leg(&right_middle, right_middle.init_x, -right_middle.init_y, right_middle.init_z);
	set_pos_leg(&right_back, right_back.init_x, -right_back.init_y, right_back.init_z);
	
	X_OFFSET_FRONT_BACK_LEG = init_y/SQRT_OF_TWO;
	Y_OFFSET_FRONT_BACK_LEG = init_y/SQRT_OF_TWO;
	Y_OFFSET_MIDDLE_LEG		= init_y;
	Z_OFFSET_LEG			= init_z;
	
	// Benens koordinat relativt kroppen är monteringspunktens koordinat + ändpunktens koordinat
	// Kroppens origo sitter i mitten
	left_front.x_from_center = init_y/SQRT_OF_TWO + LEG_MOUNT_X_FRONT_BACK;
	left_front.y_from_center = init_y/SQRT_OF_TWO + LEG_MOUNT_Y_FRONT_BACK;
	left_front.z_from_center = init_z;
	
	left_middle.x_from_center = 0;
	left_middle.y_from_center = init_y + LEG_MOUNT_Y_MIDDLE;
	left_middle.z_from_center = init_z;
	
	left_back.x_from_center = - init_y/SQRT_OF_TWO - LEG_MOUNT_X_FRONT_BACK;
	left_back.y_from_center = init_y/SQRT_OF_TWO + LEG_MOUNT_Y_FRONT_BACK;
	left_back.z_from_center = init_z;
	
	right_front.x_from_center = init_y/SQRT_OF_TWO + LEG_MOUNT_X_FRONT_BACK;
	right_front.y_from_center = - init_y/SQRT_OF_TWO - LEG_MOUNT_Y_FRONT_BACK;
	right_front.z_from_center = init_z;
	
	right_middle.x_from_center = 0;
	right_middle.y_from_center = - init_y - LEG_MOUNT_Y_MIDDLE;
	right_middle.z_from_center = init_z;
	
	right_back.x_from_center = - init_y/SQRT_OF_TWO - LEG_MOUNT_X_FRONT_BACK;
	right_back.y_from_center = - init_y/SQRT_OF_TWO - LEG_MOUNT_Y_FRONT_BACK;
	right_back.z_from_center = init_z;
}

double SQ(double num)
{
	return num*num;
}

void set_pos_leg(leg_info* leg, double x, double y, double z)
{
	double x_temp;
	double y_temp;
	
	z = z + Z_OFFSET_LEG;
	
	if (leg->leg_number >= 3)
	{
		y = -y;
	}
	
	// Kompenserar monteringsvinklar på fram-/bakben
	if(leg->leg_number == 0 || leg->leg_number == 3)
	{
		x_temp = (x + X_OFFSET_FRONT_BACK_LEG - y - Y_OFFSET_FRONT_BACK_LEG)/SQRT_OF_TWO;
		
		y_temp = (x + X_OFFSET_FRONT_BACK_LEG + y + Y_OFFSET_FRONT_BACK_LEG)/SQRT_OF_TWO;
		
		x = x_temp;
		y = y_temp;
	}
	else if (leg->leg_number == 2 || leg->leg_number == 5)
	{
		x_temp = (x - X_OFFSET_FRONT_BACK_LEG + y + Y_OFFSET_FRONT_BACK_LEG)/SQRT_OF_TWO;
		y_temp = (y + Y_OFFSET_FRONT_BACK_LEG - x + X_OFFSET_FRONT_BACK_LEG)/SQRT_OF_TWO;
		
		x = x_temp;
		y = y_temp;
	}
	else
	{
		y = y + Y_OFFSET_MIDDLE_LEG;
	}
	// Inverterad kinematik för ben
	double L1 = sqrt(SQ(x) + SQ(y));
	double L = sqrt(SQ(L1 - COXA_AXIS_DISTANCE) + SQ(z));
	
	// GAMMA
	double gamma = 180*atan(x/y)/PI;
	
	double alpha_1 = 180*acos(z/L)/PI;
	double alpha_2 = 180*acos((SQ(FEMUR_AXIS_DISTANCE) + SQ(L) - SQ(TIBIA_AXIS_TO_ENDPOINT))/(2*FEMUR_AXIS_DISTANCE*L))/PI;
	
	// ALPHA
	double alpha = alpha_1 + alpha_2;
	
	// BETA
	double beta = 180*acos((SQ(TIBIA_AXIS_TO_ENDPOINT) + SQ(FEMUR_AXIS_DISTANCE) -SQ(L))/(2*FEMUR_AXIS_DISTANCE*TIBIA_AXIS_TO_ENDPOINT))/PI;
	
	if (leg->leg_number < 3)
	{
		leg->COXA =  - gamma;
		leg->FEMUR = - (alpha - 90 + FEMUR_MOUNT_ANGLE);
		leg->TIBIA = 180 - TIBIA_MOUNT_ANGLE - beta + FEMUR_MOUNT_ANGLE;
	}
	else
	{
		leg->COXA = gamma;
		leg->FEMUR = alpha - 90 + FEMUR_MOUNT_ANGLE;
		leg->TIBIA = -(180 - TIBIA_MOUNT_ANGLE - beta + FEMUR_MOUNT_ANGLE);
	}
}


/*
void servo_test(double angle)
{
	int servo_angle_data = round(1024*(angle + 150)/300);
	int servo_angle_data_H = servo_angle_data >> 8;
	int servo_angle_data_L = servo_angle_data & 0xff;
	int sum = 0xFE + 7 + SYNC_WRITE + GOAL_POSITION_L + 2 + 1 + servo_angle_data_L + servo_angle_data_H;
	int checksum = ~(sum % 256);
	USART_transmit_mode();
	send_to_servo(0xFF);
	send_to_servo(0xFF);
	send_to_servo(0xFE);
	send_to_servo(7);
	send_to_servo(SYNC_WRITE);
	send_to_servo(GOAL_POSITION_L);
	send_to_servo(2);
	send_to_servo(1);
	send_to_servo(servo_angle_data_L);
	send_to_servo(servo_angle_data_H);
	send_to_servo(checksum);
}
void set_ID(int id)
{
	// Sätt ID till 1
	int sum = 0xFE + 4 + WRITE_DATA + ID + id;
	int checksum = 255 - (sum % 256);
	USART_transmit_mode();
	send_to_servo(0xFF);
	send_to_servo(0xFF);
	send_to_servo(0xFE);
	send_to_servo(4);
	send_to_servo(WRITE_DATA);
	send_to_servo(ID);
	send_to_servo(id);
	send_to_servo(checksum);
}

void servo_test2(double angle)
{
	int servo_angle_data = round(1024*(angle + 150)/300);
	int servo_angle_data_H = servo_angle_data >> 8;
	int servo_angle_data_L = servo_angle_data % 256;
	int sum = 1 + 7 + WRITE_DATA + GOAL_POSITION_L + servo_angle_data_L + servo_angle_data_H;
	int checksum = ~(sum % 256);
	USART_transmit_mode();
	send_to_servo(0xFF);
	send_to_servo(0xFF);
	send_to_servo(1);
	send_to_servo(7);
	send_to_servo(WRITE_DATA);
	send_to_servo(GOAL_POSITION_L);
	send_to_servo(servo_angle_data_L);
	send_to_servo(servo_angle_data_H);
	send_to_servo(checksum);
	USART_receive_mode();
}

void reset_factory()
{
	int sum = 1 + 2 + RESET;
	int checksum = ~(sum % 256);
	USART_transmit_mode();
	send_to_servo(0xFF);
	send_to_servo(0xFF);
	send_to_servo(1);
	send_to_servo(2);
	send_to_servo(RESET);
	send_to_servo(checksum);
}
*/