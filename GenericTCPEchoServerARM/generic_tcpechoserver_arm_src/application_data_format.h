/*
 * application_data_format.h
 *
 *  Created on: Aug 20, 2016
 *      Author: adrian
 */

#ifndef APPLICATION_DATA_FORMAT_H_
#define APPLICATION_DATA_FORMAT_H_



#endif /* APPLICATION_DATA_FORMAT_H_ */


#include <stdbool.h>
#include <stdint.h>

typedef struct{
	uint16_t subsystem_identifier;
	uint16_t endpoint_identifier;
	uint16_t configuration_value;
	uint16_t endpoint_current_status;
	bool read_configuration;
	bool change_configuration;
}GeneralWorkOrder_t;

typedef struct{
	uint16_t header;
	uint16_t endpoint_configuration_setting;
	uint32_t subsystem_identifier;
	uint32_t endpoint_indentifier_and_endpoint_current_status;
}GeneralWorkOrderBinaryEncode_t;
