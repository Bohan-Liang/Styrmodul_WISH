/*
 * routing.h
 *
 * Created: 5/9/2014 1:36:09 PM
 *  Author: ollho731
 */ 


#ifndef ROUTING_H_
#define ROUTING_H_
#include <inttypes.h>
// Vägval
#define KORRIDOR_1 (1<<0) //bakåt
#define KORRIDOR_2 (1<<1) //höger
#define KORRIDOR_3 (1<<2) //framåt
#define KORRIDOR_4 (1<<3) //vänster

uint8_t most_right_korridor(uint8_t krossing);

#endif /* ROUTING_H_ */