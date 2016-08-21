/*
 * generic_tcpechoserver_arm.c
 *
 *  Created on: Sep 12, 2015
 *      Author: adrian
 */

#include "generic_tcpechoserver_arm.h"
#include "thread_pool_utilities.h"
#include "encode_decode.h"
#include "framer.h"
#include <pthread.h>

#define DEBUG(fmt, ...)	fprintf(stderr, fmt, __VA_ARGS__)
#define SERVER_MAX_THREADS	10
#define SERVER_MAX_QUEUE	10

static tpool_t server_thread_pool;


typedef struct{
	int clientSocket;
}ClientWorkorder_t;

void process_request(void *clientWorkOrderPtr);

int main(int argc, char*argv[]){

	if (argc < 3) // Test for correct number of arguments
		DieWithUserMessage("Parameter(s)", "<Timeout (secs.)> <Port/Service1> ...");

	long timeout = atol(argv[1]); // First arg: Timeout
	int noPorts = argc - 2;       // Number of ports is argument count minus 2

	// Allocate list of sockets for incoming connections
	int servSock[noPorts];
	// Initialize maxDescriptor for use by select()
	int maxDescriptor = -1;

	// Create list of ports and sockets to handle ports
	int port;
	for (port = 0; port < noPorts; port++) {
		// Create port socket
		servSock[port] = SetupTCPServerSocket(argv[port + 2]);

		// Determine if new descriptor is the largest
		if (servSock[port] > maxDescriptor)
			maxDescriptor = servSock[port];
	}
	//Initialize the thread pool
	tpool_init(&server_thread_pool, SERVER_MAX_THREADS, SERVER_MAX_QUEUE, 0);
	fd_set sockSet;      // Set of socket descriptors for select()
	ClientWorkorder_t *clientWorkOrderPtr; // a pointer to a client work order pointer for a new thread
	while (1) {
		/* Zero socket descriptor vector and set for server sockets
	     This must be reset every time select() is called */
		FD_ZERO(&sockSet);
		int port;
		for (port = 0; port < noPorts; port++)
			FD_SET(servSock[port], &sockSet);
		// Timeout specification; must be reset every time select() is called
		struct timeval selTimeout;   // Timeout for select()
		selTimeout.tv_sec = timeout; // Set timeout (secs.)
		selTimeout.tv_usec = 0;      // 0 microseconds

		// Suspend program until descriptor is ready or timeout
		if (select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout) == 0)
			printf("No requests for %ld secs...Server still alive\n", timeout);
		else {
			// Process connection requests
			int port;
			for (port = 0; port < noPorts; port++){
				if (FD_ISSET(servSock[port], &sockSet)) {
					printf("Request on port %d:  ", port);
					/*
					 * create a client work order and add work order to the thread pool
					 */
					clientWorkOrderPtr = (ClientWorkorder_t *)malloc(sizeof(ClientWorkorder_t));
					clientWorkOrderPtr->clientSocket = AcceptTCPConnection(servSock[port]);
					tpool_add_work(server_thread_pool, (void *)process_request, (void *)clientWorkOrderPtr);
				}
			}
		}

	}
	/*
	 * destroy the thread pool
	 */
	tpool_destroy(server_thread_pool, 1);
	// Close sockets
	for (port = 0; port < noPorts; port++)
		close(servSock[port]);
	exit(0);
}

void process_request(void *clientWorkOrderPtr){
	ClientWorkorder_t *clientWorkOrderPtrActual = (ClientWorkorder_t *)clientWorkOrderPtr;
	unsigned int client_id = clientWorkOrderPtrActual->clientSocket;
	DEBUG("in thread for client number %u\n", client_id);
	/* add construct here to prevent server thread from closing connection client prematurely. */
		FILE *channel = fdopen(client_id, "r+");
		if(channel == NULL){
			DieWithSystemMessage("fdopen() failed.");
		}
		uint8_t inputBuf[MAX_WIRE_SIZE];
		GeneralWorkOrder_t homeAutomationWorkOrder;
		while((GetNextMsg(channel, inputBuf, MAX_WIRE_SIZE)) > 0){
			printf("\nSuccessfully received encoded information from client.\n");
			memset(&homeAutomationWorkOrder, 0, sizeof(homeAutomationWorkOrder));
			printf("\nBeginning decoding sequence.\n");
			Decode(inputBuf, &homeAutomationWorkOrder);
			printf("\nSuccessfully decoded client work request.\n");
			/*
			 * do main processing logic here
			 */
			uint8_t outBuf[MAX_WIRE_SIZE];
			printf("\nbeginning encoding sequence.\n");
			Encode(&homeAutomationWorkOrder, outBuf);
			printf("\nsuccessfully encoded client response.\n");
			if((PutMsg(outBuf, sizeof(outBuf), channel)) < 0){
				fputs("Error framing/outputting message\n", stderr);
				break;
			}
			fflush(channel);
			printf("\nsuccessfully sent client response.\n");
		}
		puts("closing server-side connection....");
		fclose(channel);
		puts("server-side connection closed successfully.");
	free(clientWorkOrderPtr);
	DEBUG("completion reached in thread id %u\n", client_id);
}

