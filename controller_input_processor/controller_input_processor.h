/*
 * controller_input_processor.h
 *
 *  Created on: Jun 25, 2016
 *      Author: adrian
 */

#ifndef CONTROLLER_INPUT_PROCESSOR_H_
#define CONTROLLER_INPUT_PROCESSOR_H_



#endif /* CONTROLLER_INPUT_PROCESSOR_H_ */

#include <stdint.h>

typedef struct application_data_packet{
	uint8_t start_of_packet;
	uint16_t controller_event_type;
	uint16_t controller_event_code;
	int32_t controller_event_value;
}application_data_packet_t;
