/*
 * generic_tcpechoserver_arm.h
 *
 *  Created on: Sep 12, 2015
 *      Author: adrian
 */

#ifndef GENERIC_TCPSERVER_ARM_H_
#define GENERIC_TCPSERVER_ARM_H_



#endif /* GENERIC_TCPECHOSERVER_ARM_H_ */

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

#define BUFSIZE 512

static const int MAXPENDING = 5;

void DieWithUserMessage(const char* msg, const char* detail){
	fputs(msg, stderr);
	fputs(": ", stderr);
	fputs(detail, stderr);
	fputc('\n', stderr);
	exit(1);
}

void DieWithSystemMessage(const char* msg){
	perror(msg);
	exit(1);
}

void PrintSocketAddress(const struct sockaddr *address, FILE *stream) {
	// Test for address and stream
	if (address == NULL || stream == NULL)
		return;

	void *numericAddress; // Pointer to binary address
	// Buffer to contain result (IPv6 sufficient to hold IPv4)
	char addrBuffer[INET6_ADDRSTRLEN];
	in_port_t port; // Port to print
	// Set pointer to address based on address family
	switch (address->sa_family) {
	case AF_INET:
		numericAddress = &((struct sockaddr_in *) address)->sin_addr;
		port = ntohs(((struct sockaddr_in *) address)->sin_port);
		break;
	case AF_INET6:
		numericAddress = &((struct sockaddr_in6 *) address)->sin6_addr;
		port = ntohs(((struct sockaddr_in6 *) address)->sin6_port);
		break;
	default:
		fputs("[unknown type]", stream);    // Unhandled type
		return;
	}
	// Convert binary to printable address
	if (inet_ntop(address->sa_family, numericAddress, addrBuffer,
			sizeof(addrBuffer)) == NULL)
		fputs("[invalid address]", stream); // Unable to convert
	else {
		fprintf(stream, "%s", addrBuffer);
		if (port != 0)                // Zero not valid in any socket addr
			fprintf(stream, "-%u", port);
	}
}

bool SockAddrsEqual(const struct sockaddr *addr1, const struct sockaddr *addr2) {
	if (addr1 == NULL || addr2 == NULL)
		return addr1 == addr2;
	else if (addr1->sa_family != addr2->sa_family)
		return false;
	else if (addr1->sa_family == AF_INET) {
		struct sockaddr_in *ipv4Addr1 = (struct sockaddr_in *) addr1;
		struct sockaddr_in *ipv4Addr2 = (struct sockaddr_in *) addr2;
		return ipv4Addr1->sin_addr.s_addr == ipv4Addr2->sin_addr.s_addr
				&& ipv4Addr1->sin_port == ipv4Addr2->sin_port;
	} else if (addr1->sa_family == AF_INET6) {
		struct sockaddr_in6 *ipv6Addr1 = (struct sockaddr_in6 *) addr1;
		struct sockaddr_in6 *ipv6Addr2 = (struct sockaddr_in6 *) addr2;
		return memcmp(&ipv6Addr1->sin6_addr, &ipv6Addr2->sin6_addr,
				sizeof(struct in6_addr)) == 0 && ipv6Addr1->sin6_port
				== ipv6Addr2->sin6_port;
	} else
		return false;
}

int SetupTCPServerSocket(const char *service) {
	// Construct the server address structure
	struct addrinfo addrCriteria;                   // Criteria for address match
	memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
	addrCriteria.ai_family = AF_UNSPEC;             // Any address family
	addrCriteria.ai_flags = AI_PASSIVE;             // Accept on any address/port
	addrCriteria.ai_socktype = SOCK_STREAM;         // Only stream sockets
	addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

	struct addrinfo *servAddr; // List of server addresses
	int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
	if (rtnVal != 0)
		DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

	int servSock = -1;
	struct addrinfo *addr;
	for (addr = servAddr; addr != NULL; addr = addr->ai_next) {
		// Create a TCP socket
		servSock = socket(addr->ai_family, addr->ai_socktype,
				addr->ai_protocol);
		if (servSock < 0)
			continue;       // Socket creation failed; try next address

		// Bind to the local address and set socket to listen
		if ((bind(servSock, addr->ai_addr, addr->ai_addrlen) == 0) &&
				(listen(servSock, MAXPENDING) == 0)) {
			// Print local address of socket
			struct sockaddr_storage localAddr;
			socklen_t addrSize = sizeof(localAddr);
			if (getsockname(servSock, (struct sockaddr *) &localAddr, &addrSize) < 0)
				DieWithSystemMessage("getsockname() failed");
			fputs("Binding to ", stdout);
			PrintSocketAddress((struct sockaddr *) &localAddr, stdout);
			fputc('\n', stdout);
			break;       // Bind and listen successful
		}

		close(servSock);  // Close and try again
		servSock = -1;
	}

	// Free address list allocated by getaddrinfo()
	freeaddrinfo(servAddr);

	return servSock;
}

int AcceptTCPConnection(int servSock) {
	struct sockaddr_storage clntAddr; // Client address
	// Set length of client address structure (in-out parameter)
	socklen_t clntAddrLen = sizeof(clntAddr);

	// Wait for a client to connect
	int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
	if (clntSock < 0)
		DieWithSystemMessage("accept() failed");

	// clntSock is connected to a client!

	fputs("Handling client ", stdout);
	PrintSocketAddress((struct sockaddr *) &clntAddr, stdout);
	fputc('\n', stdout);

	return clntSock;
}

void HandleTCPClient(int clntSocket) {
	char buffer[BUFSIZE]; // Buffer for echo string

	// Receive message from client
	ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
	if (numBytesRcvd < 0)
		DieWithSystemMessage("recv() failed");

	// Send received string and receive again until end of stream
	while (numBytesRcvd > 0) { // 0 indicates end of stream
		// Echo message back to client
		ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
		if (numBytesSent < 0)
			DieWithSystemMessage("send() failed");
		else if (numBytesSent != numBytesRcvd)
			DieWithUserMessage("send()", "sent unexpected number of bytes");

		// See if there is more data to receive
		numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
		if (numBytesRcvd < 0)
			DieWithSystemMessage("recv() failed");
	}

	close(clntSocket); // Close client socket
}

