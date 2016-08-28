/*
 * generic_tcpechoserver_arm.c
 *
 *  Created on: Sep 12, 2015
 *      Author: adrian
 */

#include "generic_tcpserver.h"

#include "thread_pool_utilities.h"
#include "encode_decode.h"
#include "framer.h"
#include <pthread.h>

#define DEBUG(fmt, ...)	fprintf(stderr, fmt, __VA_ARGS__)
#define SERVER_MAX_THREADS	10
#define SERVER_MAX_QUEUE	10

typedef struct{
	int clientSocket;
}client_connection_details_t;

static void init_server(int argc, char* argv[], int ** server_sockets_array, int * maxDescriptorPtr, long * timeoutPtr, int * noPortsPtr);
pthread_mutex_t server_shutdown_indicator_mutex = PTHREAD_MUTEX_INITIALIZER;

void server_thread_pool_task(void *clientConnectionDetailsPtr);
void server_shutdown_service_task(void * server_shutdown_indicator);

int main(int argc, char*argv[]){

	tpool_t server_thread_pool;
	fd_set server_file_descriptor_set;
	int * server_sockets_array = NULL;
	int maxDescriptor = -1;
	long timeout = 0;
	int noPorts = 0;
	int server_shutdown_indicator = 0;
	pthread_t server_shutdown_listener_thread;
	/*
	 * setup and initialize main server parameters. If server does not initialize properly due to complications,
	 * The program is aborted immediately and not allowed to proceed further.
	 */
	init_server(argc, argv, &server_sockets_array, &maxDescriptor, &timeout, &noPorts);
	/*
	 * initialize a separate server shutdown thread. This thread is used to listen for shutdown requests to signal the server to shutdown
	 */
	DEBUG("%s", "creating server shutdown listener thread ..\n");
	pthread_attr_t server_shutdown_listener_attr;
	pthread_attr_init(&server_shutdown_listener_attr);
	pthread_attr_setdetachstate(&server_shutdown_listener_attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&server_shutdown_listener_thread, &server_shutdown_listener_attr, (void *)server_shutdown_service_task, (void *)&server_shutdown_indicator);
	DEBUG("%s", "creation of server shutdown listener thread completed ..\n");
	/*
	 * initialize the main server thread pool, that handles incoming connections
	 */
	tpool_init(&server_thread_pool, SERVER_MAX_THREADS, SERVER_MAX_QUEUE, 0, server_thread_pool_task);
	client_connection_details_t *clientWorkOrderPtr; // a pointer to a client work order pointer for a new thread
	while (1) {
		DEBUG("%s", "Beginning main server loop ..\n");
		if(pthread_mutex_trylock(&server_shutdown_indicator_mutex) == 0){
			DEBUG("%s", "server_shutdown_indicator_mutex lock acquired, evaluating shutdown indicator.. \n");
			if(server_shutdown_indicator){
				break;
			}
		}
		DEBUG("%s", "server_shutdown_indicator_mutex lock not acquired, proceeding to processing incoming connections.. \n");
		/* Zero socket descriptor vector and set for server sockets
	     This must be reset every time select() is called */
		FD_ZERO(&server_file_descriptor_set);
		DEBUG("%s", "rebuilt server file descriptor set.. \n");
		int port;
		for (port = 0; port < noPorts; port++)
			FD_SET(server_sockets_array[port], &server_file_descriptor_set);
		DEBUG("%s", "completed re-populating server file descriptor set.. \n");
		// Timeout specification; must be reset every time select() is called
		struct timeval selTimeout;   // Timeout for select()
		selTimeout.tv_sec = timeout; // Set timeout (secs.)
		selTimeout.tv_usec = 0;      // 0 microseconds

		// Suspend program until descriptor is ready or timeout
		if (select(maxDescriptor + 1, &server_file_descriptor_set, NULL, NULL, &selTimeout) == 0)
			printf("No requests for %ld secs...Server still alive\n", timeout);
		else {
			// Process connection requests
			int port;
			for (port = 0; port < noPorts; port++){
				if (FD_ISSET(server_sockets_array[port], &server_file_descriptor_set)) {
					printf("Request on port %d:  ", port);
					/*
					 * create a client work order and add work order to the thread pool
					 */
					clientWorkOrderPtr = (client_connection_details_t *)malloc(sizeof(client_connection_details_t));
					clientWorkOrderPtr->clientSocket = AcceptTCPConnection(server_sockets_array[port]);
					tpool_add_work(server_thread_pool, (void *)clientWorkOrderPtr);
				}
			}
		}

	}
	/*
	 * destroy the thread pool
	 */
	tpool_destroy(server_thread_pool, 1);
	// Close sockets
	int port;
	for (port = 0; port < noPorts; port++)
		close(server_sockets_array[port]);
	/*
	 * unlock the server shutdown indicator mutex
	 */
	pthread_mutex_unlock(&server_shutdown_indicator_mutex);
	exit(0);
}

static void init_server(int argc, char* argv[], int **server_sockets_array,int * maxDescriptorPtr, long * timeout, int *noPortsPtr){
	printf("Starting initialization of main server parameters...\n");
	if (argc < 3) // Test for correct number of arguments
		DieWithUserMessage("Parameter(s)", "<Timeout (secs.)> <Port/Service1> ...");

	*timeout = atol(argv[1]); // First arg: Timeout
	*noPortsPtr = argc - 2;       // Number of ports is argument count minus 2

	// Allocate list of sockets for incoming connections
	int * sockets_arr = (int *)malloc((*(noPortsPtr)) * sizeof(int));
	if(sockets_arr == NULL){
		DieWithUserMessage("sockets array", "The array of server sockets could not be allocated...");
	}
	// Initialize maxDescriptor for use by select()
	//int maxDescriptor = -1;

	// Create list of ports and sockets to handle ports
	int port;
	for (port = 0; port < (*(noPortsPtr)); port++) {
		// Create port socket
		sockets_arr[port] = SetupTCPServerSocket(argv[port + 2]);

		// Determine if new descriptor is the largest
		if (sockets_arr[port] > *maxDescriptorPtr)
			*maxDescriptorPtr = sockets_arr[port];
	}
	*server_sockets_array = sockets_arr;
	printf("Finished initialization of main server parameters...\n");
}

void server_thread_pool_task(void *clientConnectionDetailsPtr){
	client_connection_details_t *clientConnectionDetailsPtrActual = (client_connection_details_t *)clientConnectionDetailsPtr;
	unsigned int client_id = clientConnectionDetailsPtrActual->clientSocket;
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
	free(clientConnectionDetailsPtr);
	DEBUG("completion reached in thread id %u\n", client_id);
}

void server_shutdown_service_task(void * server_shutdown_indicator){
	int * shutdown_indicator_actualPtr = (int *)server_shutdown_indicator;
	/*
	 * acquire the mutex for the shutdown flag
	 */
	pthread_mutex_lock(&server_shutdown_indicator_mutex);
	/*
	 * in this loop we infinitely evaluate whether a user has shutdown the server by typing "shutdown" at the console.
	 * if so, we break out of the loop, assert the shutdown flag, relinquish the mutex and return from the function
	 * there by exiting the the the thread;
	 */
	char shut_down_request[10];
	while(1){
		scanf("%s", shut_down_request);
		if(strcmp("shutdown", shut_down_request) == 0){
			break;
		}
	}
	*shutdown_indicator_actualPtr = 1;
	pthread_mutex_unlock(&server_shutdown_indicator_mutex);
}

