/*
 * echoserver.h
 *
 *  Created on: Aug 19, 2015
 *      Author: adrian
 */

#ifndef ECHOSERVER_H_
#define ECHOSERVER_H_



#endif /* ECHOSERVER_H_ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const in_port_t serverPort = 5667;

const int MAXPENDING = 5;

extern void createScoket(int *serverSocket);

extern void constructLocalAddressStructure(struct sockaddr_in *servAddrPtr);

extern void bindToLocalAddressAndListen(int *serverSocket, struct sockaddr *sockaddrptr);

extern void HandleTCPClient(int clntSock);
