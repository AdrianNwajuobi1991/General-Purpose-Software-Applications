/*
 * echoserver.c
 *
 *  Created on: Aug 20, 2015
 *      Author: adrian nwajuobi
 */

#include "echoserver.h"

void createSocket(int *serverSocket){
	*serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void constructLocalAddressStructure(struct sockaddr_in *servAddrPtr){
	memset(servAddrPtr, 0, sizeof(*servAddrPtr));
	servAddrPtr ->sin_family = AF_INET;
	servAddrPtr ->sin_addr.s_addr = htonl(INADDR_ANY);
	servAddrPtr ->sin_port = htons(serverPort);
}

void bindToLocalAddressAndListen(int *serverSocket, struct sockaddr *sockaddrptr){
	bind(*serverSocket, sockaddrptr, sizeof(*sockaddrptr));
	listen(*serverSocket, MAXPENDING);
}

void HandleTCPClient(int clntSock){
	char buffer[10000];
	ssize_t numBytesRcvd  = recv(clntSock, buffer, 10000, 0);
	while(numBytesRcvd > 0){
		send(clntSock, buffer, numBytesRcvd, 0);
		numBytesRcvd  = recv(clntSock, buffer, 10000, 0);
	}
	close(clntSock);
}

int main(){
	int serverSocket;
	struct sockaddr_in servAddr;

	createSocket(&serverSocket);
	constructLocalAddressStructure(&servAddr);
	bindToLocalAddressAndListen(&serverSocket, (struct sockaddr*)&servAddr);

	for(;;){
		struct sockaddr_in clntAddr;

		socklen_t clntAddrLen = sizeof(clntAddr);

		int clntSock = accept(serverSocket, (struct sockaddr*)&clntAddr, &clntAddrLen);

		HandleTCPClient(clntSock);
	}
	return 0;
}
