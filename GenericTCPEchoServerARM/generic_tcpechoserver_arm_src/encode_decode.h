/*
 * encode_decode.h
 *
 *  Created on: Aug 20, 2016
 *      Author: adrian
 */

#ifndef ENCODE_DECODE_H_
#define ENCODE_DECODE_H_



#endif /* ENCODE_DECODE_H_ */

#include <stdbool.h>
#include "application_data_format.h"
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#define CLIENT_REQUEST_SIZE 512
#define MAX_WIRE_SIZE 1000
#define MAGIC 0x5400
#define READ_CONFIGURATION_FLAG 0x0200
#define CHANGE_CONFIGURATION_FLAG 0x0100
#define SUBSYSTEM_IDENTIFIER_MASK 0x0000FFFF
#define ENDPOINT_IDENTIFIER_MASK 0x0000FFFF
#define ENDPOINT_CURRENT_STATUS_MASK 0xFFFF0000

extern void Decode(uint8_t *inBuf, GeneralWorkOrder_t *inputWorkOrderPtr);

extern void Encode(GeneralWorkOrder_t *inputWorkOrderPtr, uint8_t *outBuf);
