/*
 * framer.h
 *
 *  Created on: Aug 20, 2016
 *      Author: adrian
 */

#ifndef FRAMER_H_
#define FRAMER_H_

#include <stdint.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>


#endif /* FRAMER_H_ */

extern int GetNextMsg(FILE *in, uint8_t *buf, size_t bufSize);

extern int PutMsg(uint8_t buf[], size_t msgSize, FILE *out);
