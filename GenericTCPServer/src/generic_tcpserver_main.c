/*
 * generic_tcpserver_main.c
 *
 *  Created on: Aug 28, 2016
 *      Author: adrian
 */


#include "generic_tcpserver.h"
#include "generic_tcpserver_utils.h"
#include <pthread.h>

static server_service_threads_ptr server_service_threads;

int main(int argc, char*argv[]){
	int server_shutdown_indicator = 0;
	/*
	 * initialize the main service threads of this server
	 */
	init_server_service_threads(&server_service_threads);
	/*
	 * initialize the structure data for the server local client listener thread. If server does not initialize properly due to complications,
	 * The program is aborted immediately and not allowed to proceed further.
	 */
	DEBUG("%s", "creating local client listener parameters");
	local_client_listener_parameters_ptr local_client_listener_dataPtr;
	init_local_client_listener_server(argc, argv, &local_client_listener_dataPtr);
	DEBUG("%s", "finished creating local client listener parameters");
	/*
	 * setup and initialize remote client listener server parameters. If server does not initialize properly due to complications,
	 * The program is aborted immediately and not allowed to proceed further.
	 */
	DEBUG("%s", "creating remote client listener parameters");
	remote_client_listener_parameters_ptr remote_client_listener_dataPtr;
	init_remote_client_listener_server(argc, argv, &remote_client_listener_dataPtr);
	DEBUG("%s", "finished creating remote client listener data");
	/*
	 * initialize the local client listener thread for listening for connections on the sockets
	 */
	DEBUG("%s", "creating local client listener thread ..\n");
	pthread_attr_t local_client_listener_attr;
	start_server_service_thread(server_service_threads->local_client_listener_thread, (void *)local_client_listener_dataPtr, &local_client_listener_attr, (void *)local_client_listener_task);
	DEBUG("%s", "creation of local client listener thread completed ..\n");
	/*
	 * initialize the remote client listener thread for listening for connections of remote clients
	 */
	DEBUG("%s", "creating remote client listener thread ..\n");
	pthread_attr_t remote_client_listener_attr;
	start_server_service_thread(server_service_threads->remote_client_listener_thread, (void *)remote_client_listener_dataPtr, &remote_client_listener_attr, (void *)remote_client_listener_task);
	DEBUG("%s", "creation of remote client listener thread completed ..\n");

	pthread_mutex_lock(&server_thread_setup_mutex);
	while(server_thread_setup_count != 0){
		pthread_cond_wait(&server_thread_setup_cond,&server_thread_setup_mutex);
	}
	pthread_mutex_unlock(&server_thread_setup_mutex);
	/*
	 * initialize a separate server shutdown thread. This thread is used to listen for shutdown requests to signal the server to shutdown.
	 */
	DEBUG("%s", "creating server shutdown listener thread ..\n");
	pthread_attr_t server_shutdown_listener_attr;
	start_server_service_thread(server_service_threads->server_shutdown_listener_thread, (void *)&server_shutdown_indicator, &server_shutdown_listener_attr, (void *)server_shutdown_service_task);
	DEBUG("%s", "creation of server shutdown listener thread completed ..\n");
	/*
	 * begin main shutdown and application server control loop, it listens for shutdown requests and can handle other requests
	 * such as communicating to a load balancer
	 */
	DEBUG("%s", "Beginning to listen for shutdown requests ..\n");
	pthread_mutex_lock(&server_shutdown_mutex);
	while(!server_shutdown_indicator){
		pthread_cond_wait(&server_shutdown_cond,&server_shutdown_mutex);
	}
	pthread_mutex_unlock(&server_shutdown_mutex);
	/*
	 * cancel the local client listener and remote client listener threads
	 */
	DEBUG("%s", "canceling local client listener thread ..\n");
	pthread_cancel(*server_service_threads->local_client_listener_thread);
	DEBUG("%s", "cancellation of local client listener thread completed..\n");
	DEBUG("%s", "canceling remote client listener thread ..\n");
	pthread_cancel(*server_service_threads->remote_client_listener_thread);
	DEBUG("%s", "cancellation of remote client listener thread completed..\n");
	/*
	 * run routine to shutdown the server
	 */
	server_shutdown(local_client_listener_dataPtr, remote_client_listener_dataPtr, server_service_threads);
	exit(0);
}
