/*
 * Control_Table.h
 *
 * Skapad: 2014-03-24 21:01:47
 * Innehåll:
 * Kontroll-parametrar hos AX-12A servo
 */ 


#ifndef CONTROL_TABLE_H_
#define CONTROL_TABLE_H_


//EEPROM
#define MODEL_NUMBER_L		0
#define MODOEL_NUMBER_H		1
#define VERSION				2
#define ID					3
#define BAUD_RATE			4
#define RETURN_DELAY_TIME	5
#define CW_ANGLE_LIMIT_L	6
#define CW_ANGLE_LIMIT_H	7
#define CCW_ANGLE_LIMIT_L	8
#define CCW_ANGLE_LIMIT_H	9
#define SYSTEM_DATA2		10
#define LIMIT_TEMPERATURE	11
#define DOWN_LIMIT_VOLTAGE	12
#define UP_LIMIT_VOLTAGE	13
#define MAX_TORQUE_L		14
#define MAX_TORQUE_H		15
#define RETURN_LEVEL		16
#define ALARM_LED			17
#define ALARM_SHUTDOWN		18

//RAM
#define TORQUE_ENABLE			24
#define LED						25
#define CW_COMPLIANCE_MARGIN	26
#define CCW_COMPLIANCE_MARGIN	27
#define CW_COMPLIANCE_SLOPE		28
#define CCW_COMPLIANCE_SLOPE	29
#define GOAL_POSITION_L			30
#define GOAL_POSITION_H			31
#define GOAL_SPEED_L			32
#define GOAL_SPEED_H			33
#define TORQUE_LIMIT_L			34
#define TORQUE_LIMIT_H			35
#define PRESENT_POSITION_L		36
#define PRESENT_POSITION_H		37
#define PRESENT_SPEED_L			38
#define PRESENT_SPEED_H			39
#define PRESENT_LOAD_L			40
#define PRESENT_LOAD_H			41
#define PRESENT_VOLTAGE			42
#define PRESENT_TEMPERATURE		43
#define REGISTERED_INSTRUCTION	44
#define MOVING					46
#define LOCK					47
#define PUNCH_L					48
#define PUNCH_H					49

//Instructioner
#define PING			0x01
#define READ_DATA		0x02
#define WRITE_DATA		0x03
#define REG_WRITE		0x04
#define ACTION			0x05
#define RESET			0x06
#define SYNC_WRITE		0x83

//Felkod
#define NO_ERROR			0b00000001
#define INSTRUCTION_ERROR	0b00000010
#define OVERLOAD_ERROR		0b00000100
#define CHECKSUM_ERROR		0b00001000
#define RANGE_ERROR			0b00010000
#define OVERHEATING_ERROR	0b00100000
#define ANGLE_LIMIT_ERROR	0b01000000
#define INPUT_VOLTAGE_ERROR	0b10000000

#endif /* CONTROL_TABLE_H_ */