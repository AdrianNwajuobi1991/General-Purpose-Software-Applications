/*
 * generic_tcpechoserver_arm.c
 *
 *  Created on: Sep 12, 2015
 *      Author: adrian
 */

#include "generic_tcpechoserver_arm.h"
#include "thread_pool_utilities.h"
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

	puts("Starting server:  Hit return to shutdown");
	bool running = true; // true if server should continue running
	fd_set sockSet;      // Set of socket descriptors for select()
	ClientWorkorder_t *clientWorkOrderPtr; // a pointer to a client work order pointer for a new thread
	//pthread_t *serverWorkerThreadPtr; //a pointer to a server worker thread
	while (running) {
		/* Zero socket descriptor vector and set for server sockets
	     This must be reset every time select() is called */
		FD_ZERO(&sockSet);
		// Add keyboard to descriptor vector
		FD_SET(STDIN_FILENO, &sockSet);
		int port;
		for (port = 0; port < noPorts; port++)
			FD_SET(servSock[port], &sockSet);

		// Timeout specification; must be reset every time select() is called
		struct timeval selTimeout;   // Timeout for select()
		selTimeout.tv_sec = timeout; // Set timeout (secs.)
		selTimeout.tv_usec = 0;      // 0 microseconds

		// Suspend program until descriptor is ready or timeout
		if (select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout) == 0)
			printf("No echo requests for %ld secs...Server still alive\n", timeout);
		else {
			if (FD_ISSET(0, &sockSet)) { // Check keyboard
				puts("Shutting down server");
				getchar();
				running = false;
			}
			// Process connection requests
			int port;
			for (port = 0; port < noPorts; port++){
				if (FD_ISSET(servSock[port], &sockSet)) {
					printf("Request on port %d:  ", port);
					clientWorkOrderPtr = (ClientWorkorder_t *)malloc(sizeof(ClientWorkorder_t));
					clientWorkOrderPtr->clientSocket = AcceptTCPConnection(servSock[port]);
					/*
					 * spawn a thread to process this request
					 */
					/*serverWorkerThreadPtr = (pthread_t *)malloc(sizeof(pthread_t));
					pthread_create(serverWorkerThreadPtr, NULL, (void *)process_request, (void *)clientWorkOrderPtr);
					pthread_detach(*serverWorkerThreadPtr);
					free(serverWorkerThreadPtr);
					 */
					/*
					 * use thread pool to process request: add work to the thread pool
					 */
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
	unsigned int thread_id = rand() % 65535;
	DEBUG("in thread id %u\n", thread_id);
	HandleTCPClient(clientWorkOrderPtrActual->clientSocket);
	free(clientWorkOrderPtr);
	DEBUG("completion reached in thread id %u\n", thread_id);
}
