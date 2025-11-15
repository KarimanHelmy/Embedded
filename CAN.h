/*
 * CAN.h
 *
 * Created: 13.10.2025
 *  Author: Mohamed Abosreea , Kariman Helmy , Marwa Elbadawy
 */


#ifndef CAN_H_
#define CAN_H_

#include "CAN_controller_driver.h"

typedef struct {
	unsigned int id;
	uint8_t length;
	int8_t data[8];
} can_message;


void can_init(uint8_t mode);
void can_message_send(can_message* message);
uint8_t can_interrupt(void);
can_message can_handle_messages();
int can_transmit_complete(int);
void can_message_receive(int, can_message* message);
void can_int_vect(int*);

#endif /* CAN_H_ */