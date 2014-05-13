/*
 * routing.c
 *
 * Created: 5/9/2014 1:37:14 PM
 *  Author: ollho731
 */ 
#include "routing.h"
#include <stdlib.h>

uint8_t most_right_korridor(uint8_t krossing)
{
	uint8_t rval = 1;
	if(krossing)
	{
		while(!(krossing & rval))
		rval = (rval << 1);
	}
	else
	{
		return 0;
	}
	return rval;
}

struct node
{
	uint8_t data;
	struct node* next;	
};

typedef struct  
{
	struct node* head;
} stack;
#define init_stack() {NULL};


void insert(stack* dest,uint8_t entry)
{
	struct node* temp = (struct node*)malloc(sizeof(struct node));
	temp->data = entry;
	temp->next = dest->head;
	dest->head = temp;
}
uint8_t pop(stack* dest)
{
	if(dest->head != NULL)
	{
		uint8_t rval;
		struct node* temp = dest->head;
		dest->head = dest->head->next;
		
		rval = temp->data;
		free(temp);
		return rval; 
	}
	else
	return 0;
}


stack crossing_stack;

void new_crossing(uint8_t crossing)
{
	struct node* temp = (struct node*)malloc(sizeof(struct node));
	temp->data = crossing;
	temp->next = crossing_stack.head;
	
	crossing_stack.head = temp;
}
void add_opening_on_current(uint8_t corridor)
{
	crossing_stack.head->data |= corridor;
}
