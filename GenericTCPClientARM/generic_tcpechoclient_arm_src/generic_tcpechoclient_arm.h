/*
 * generic_tcpechoclient_arm.h
 *
 *  Created on: Oct 23, 2015
 *      Author: adrian
 */

#ifndef GENERIC_TCPECHOCLIENT_ARM_H_
#define GENERIC_TCPECHOCLIENT_ARM_H_



#endif /* GENERIC_TCPECHOCLIENT_ARM_H_ */
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
#include <stdint.h>

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

int SetupTCPClientSocket(const char *host, const char *service) {
  // Tell the system what kind(s) of address info we want
  struct addrinfo addrCriteria;                   // Criteria for address match
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
  addrCriteria.ai_family = AF_UNSPEC;             // v4 or v6 is OK
  addrCriteria.ai_socktype = SOCK_STREAM;         // Only streaming sockets
  addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

  // Get address(es)
  struct addrinfo *servAddr; // Holder for returned list of server addrs
  int rtnVal = getaddrinfo(host, service, &addrCriteria, &servAddr);
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

  int sock = -1;
  struct addrinfo *addr;

  for (addr = servAddr; addr != NULL; addr = addr->ai_next) {
    // Create a reliable, stream socket using TCP
    sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (sock < 0)
      continue;  // Socket creation failed; try next address

    // Establish the connection to the echo server
    if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0)
      break;     // Socket connection succeeded; break and return socket

    close(sock); // Socket connection failed; try next address
    sock = -1;
  }

  freeaddrinfo(servAddr); // Free addrinfo allocated in getaddrinfo()
  return sock;
}
