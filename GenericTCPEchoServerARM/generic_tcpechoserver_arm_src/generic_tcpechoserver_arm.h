/*
 * generic_tcpechoserver_arm.h
 *
 *  Created on: Sep 12, 2015
 *      Author: adrian
 */

#ifndef GENERIC_TCPSERVER_ARM_H_
#define GENERIC_TCPSERVER_ARM_H_



#endif /* GENERIC_TCPSERVER_ARM_H_ */

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

#define BUFSIZE 512
#define MAX_WIRE_SIZE 1000
#define MAGIC 0x5400
#define READ_CONFIGURATION_FLAG 0x0200
#define CHANGE_CONFIGURATION_FLAG 0x0100
#define SUBSYSTEM_IDENTIFIER_MASK 0x0000FFFF
#define ENDPOINT_IDENTIFIER_MASK 0x0000FFFF
#define ENDPOINT_CURRENT_STATUS_MASK 0xFFFF0000

typedef struct{
	uint16_t subsystem_identifier;
	uint16_t endpoint_identifier;
	uint16_t configuration_value;
	uint16_t endpoint_current_status;
	bool read_configuration;
	bool change_configuration;
}HomeAutomationWorkOrder_t;

typedef struct{
	uint16_t header;
	uint16_t endpoint_configuration_setting;
	uint32_t subsystem_identifier;
	uint32_t endpoint_indentifier_and_endpoint_current_status;
}HomeAutomationWorkOrderBinaryEncode_t;

int GetNextMsg(FILE *in, uint8_t *buf, size_t bufSize) {
  uint16_t mSize = 0;
  uint16_t extra = 0;

  if (fread(&mSize, sizeof(uint16_t), 1, in) != 1)
    return -1;
  mSize = ntohs(mSize);
  if (mSize > bufSize) {
    extra = mSize - bufSize;
    mSize = bufSize; // Truncate
  }
  if (fread(buf, sizeof(uint8_t), mSize, in) != mSize) {
    fprintf(stderr, "Framing error: expected %d, read less\n", mSize);
    return -1;
  }
  if (extra > 0) { // Message was truncated
    uint8_t waste[BUFSIZE];
    fread(waste, sizeof(uint8_t), extra, in); // Try to flush the channel
    return -(mSize + extra); // Negation of indicated size
  } else
    return mSize;
}

/* Write the given message to the output stream, followed by
 * the delimiter.  Precondition: buf[] is at least msgSize.
 * Returns -1 on any error.
 */
int PutMsg(uint8_t buf[], size_t msgSize, FILE *out) {
  if (msgSize > UINT16_MAX)
    return -1;
  uint16_t payloadSize = htons(msgSize);
  if ((fwrite(&payloadSize, sizeof(uint16_t), 1, out) != 1) || (fwrite(buf,
      sizeof(uint8_t), msgSize, out) != msgSize))
    return -1;
  fflush(out);
  return msgSize;
}

void Decode(uint8_t *inBuf, HomeAutomationWorkOrder_t *inputWorkOrderPtr){
	HomeAutomationWorkOrderBinaryEncode_t *hawPtr = (HomeAutomationWorkOrderBinaryEncode_t *) inBuf;
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
void Encode(HomeAutomationWorkOrder_t *inputWorkOrderPtr, uint8_t *outBuf){
	HomeAutomationWorkOrderBinaryEncode_t *hawPtr = (HomeAutomationWorkOrderBinaryEncode_t*)outBuf;
	memset(outBuf, 0, sizeof(HomeAutomationWorkOrderBinaryEncode_t));
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
