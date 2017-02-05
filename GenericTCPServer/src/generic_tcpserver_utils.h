/*
 * generic_tcpserver_utils.h
 *
 *  Created on: Aug 28, 2016
 *      Author: adrian
 */

#ifndef GENERIC_TCPSERVER_UTILS_H_
#define GENERIC_TCPSERVER_UTILS_H_



#endif /* GENERIC_TCPSERVER_UTILS_H_ */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define DEBUG(fmt, ...)	fprintf(stderr, fmt, __VA_ARGS__)

#define CLIENT_REQUEST_SIZE 512


extern void DieWithUserMessage(const char* msg, const char* detail);

extern void DieWithSystemMessage(const char* msg);

extern void PrintSocketAddress(const struct sockaddr *address, FILE *stream);

extern bool SockAddrsEqual(const struct sockaddr *addr1, const struct sockaddr *addr2);

extern int SetupTCPServerSocket(const char *service);

extern int AcceptTCPConnection(int servSock);
