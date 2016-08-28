/*
 * encode_decode.c
 *
 *  Created on: Aug 20, 2016
 *      Author: adrian
 */

#include "encode_decode.h"

void Decode(uint8_t *inBuf, GeneralWorkOrder_t *inputWorkOrderPtr){
	GeneralWorkOrderBinaryEncode_t *hawPtr = (GeneralWorkOrderBinaryEncode_t *) inBuf;
	uint16_t header = ntohs(hawPtr->header);
	printf("\nheader data in host byte order is %04x", header);
	inputWorkOrderPtr->read_configuration = header & READ_CONFIGURATION_FLAG;
	inputWorkOrderPtr->change_configuration = header & CHANGE_CONFIGURATION_FLAG;
	if(inputWorkOrderPtr->read_configuration == true){
		printf("\nThis work item consisted of a read of configuration data.");
	}
	if(inputWorkOrderPtr->change_configuration == true){
		printf("\nThis work item consisted of a change of configuration data.");
	}
	inputWorkOrderPtr->configuration_value = ntohs(hawPtr->endpoint_configuration_setting);
	printf("\nThe end_point_configuration setting in host byte order is %04x", inputWorkOrderPtr->configuration_value);
	uint32_t endpoint_identifier_and_current_status_setting = ntohl(hawPtr->endpoint_indentifier_and_endpoint_current_status);
	inputWorkOrderPtr->endpoint_identifier = 0x0000FFFF & endpoint_identifier_and_current_status_setting;
	printf("\nThe endpoint_identifier in host byte order is %04x", inputWorkOrderPtr->endpoint_identifier);
	inputWorkOrderPtr->endpoint_current_status = (0xFFFF0000 & endpoint_identifier_and_current_status_setting) >> 16;
	printf("\nThe endpoint_current_status in host byte order is %04x", inputWorkOrderPtr->endpoint_current_status);
	inputWorkOrderPtr->subsystem_identifier = 0x0000FFFF & (ntohl(hawPtr->subsystem_identifier));
	printf("\nThe subsystem_identifier in host byte order is %04x", inputWorkOrderPtr->subsystem_identifier);

}
void Encode(GeneralWorkOrder_t *inputWorkOrderPtr, uint8_t *outBuf){
	GeneralWorkOrderBinaryEncode_t *hawPtr = (GeneralWorkOrderBinaryEncode_t*)outBuf;
	memset(outBuf, 0, sizeof(GeneralWorkOrderBinaryEncode_t));
	hawPtr->header = MAGIC;
	if(inputWorkOrderPtr->read_configuration == true){
		hawPtr->header |= READ_CONFIGURATION_FLAG;
	}
	if(inputWorkOrderPtr->change_configuration == true){
		hawPtr->header |= CHANGE_CONFIGURATION_FLAG;
	}
	hawPtr->header = htons(hawPtr->header);
	printf("\nheader data in network byte order is %04x", hawPtr->header);
	hawPtr->endpoint_configuration_setting = htons(inputWorkOrderPtr->configuration_value);
	printf("\nend-point configuration data in network byte order is %04x", hawPtr->endpoint_configuration_setting);
	hawPtr->subsystem_identifier = htonl(inputWorkOrderPtr->subsystem_identifier);
	printf("\nsubsystem identifier data in network byte order is %08x", hawPtr->subsystem_identifier);
	uint32_t endpoint_identifier_and_current_status_setting = inputWorkOrderPtr->endpoint_current_status;
	endpoint_identifier_and_current_status_setting = endpoint_identifier_and_current_status_setting << 16;
	endpoint_identifier_and_current_status_setting |= inputWorkOrderPtr->endpoint_identifier;
	hawPtr->endpoint_indentifier_and_endpoint_current_status = htonl(endpoint_identifier_and_current_status_setting); // current status
	printf("\nend-point current status data in network byte order is %08x", (0x0000FFFF & hawPtr->endpoint_indentifier_and_endpoint_current_status));
	printf("\nend-point identifier data in network byte order is %08x", (0xFFFF0000 & hawPtr->endpoint_indentifier_and_endpoint_current_status));
}
