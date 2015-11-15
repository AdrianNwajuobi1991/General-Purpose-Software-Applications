/*
 * generic_tcpechoclient_arm.c
 *
 *  Created on: Oct 23, 2015
 *      Author: adrian
 */



#include "generic_tcpechoclient_arm.h"



void validate_and_initialize_inputs(int argc);
void validate_server_socket(int serverSocket);
void process_input_workorder(HomeAutomationWorkOrder_t *inputWorkOrderPtr, FILE *fd_sock_ptr);

int main(int argc, char *argv[]){
	//server parameters
	char *server =  NULL;
	char *serverPort = NULL;
	int serverSocket;
	bool clientStopped;
	// application parameters
	HomeAutomationWorkOrder_t *inputWorkOrderPtr = NULL;
	unsigned int subsystem_identifier;
	unsigned int endpoint_identifier;
	unsigned int configuration_value;
	char answer_to_question[7];

	FILE *fd_sock_ptr = NULL;

	validate_and_initialize_inputs(argc);
	server = argv[1];
	serverPort = argv[2];
	serverSocket = SetupTCPClientSocket(server, serverPort);
	validate_server_socket(serverSocket);

	fd_sock_ptr = fdopen(serverSocket, "r+");
	if(fd_sock_ptr == NULL){
		DieWithSystemMessage("fdopen() failed");
	}

	clientStopped = false;

	while(clientStopped == false){
		/*
		subsystem_identifier = 10;
		endpoint_identifier = 12;
		configuration_value = 14;
		strcpy(answer_to_question, "yes");
		inputWorkOrderPtr = (HomeAutomationWorkOrder_t *)malloc(sizeof(HomeAutomationWorkOrder_t));
		if(inputWorkOrderPtr == NULL){
			DieWithUserMessage("malloc() failed", "cannot create a work order request");
		}
		inputWorkOrderPtr->subsystem_identifier = subsystem_identifier;
		inputWorkOrderPtr->endpoint_identifier = endpoint_identifier;
		inputWorkOrderPtr->configuration_value = configuration_value;
		if(strcmp("yes", answer_to_question) == 0){
			inputWorkOrderPtr->read_configuration = true;
			inputWorkOrderPtr->change_configuration = false;
		}else{
			inputWorkOrderPtr->read_configuration = false;
			inputWorkOrderPtr->change_configuration = true;
		}
		process_input_workorder(inputWorkOrderPtr, fd_sock_ptr);
		free(inputWorkOrderPtr);
		clientStopped = true;
		 */

		printf("\n\nEnter the subsystem root identifier. To shutdown this application enter the number zero at any of the first"
				" three prompts in the application(this one included): ");
		//scanf here to recieve token
		scanf("%u", &subsystem_identifier);
		//then validation step
		if(subsystem_identifier == 0){
			clientStopped = true;
			continue;
		}
		printf("\nEnter the identifier of the end-point within the above subsystem: ");
		//scanf here to recieve token
		scanf("%u", &endpoint_identifier);
		//then validation step
		if(endpoint_identifier == 0){
			clientStopped = true;
			continue;
		}
		printf("\nEnter the configuration value for the particular end-point: ");
		//scanf here to recieve token
		scanf("%u", &configuration_value);
		//then validation while step
		if(configuration_value == 0){
			clientStopped = true;
			continue;
		}
		printf("\nIndicate Whether the above configuration indicates a read of the current "
				"status of the end-point or a change of configuration (yes/no) or enter 'quit' at this prompt to "
				"terminate the application: ");
		//scanf here to recieve token
		scanf("%s", answer_to_question);
		//then validation step
		while((strcmp("yes", answer_to_question) != 0) && (strcmp("no", answer_to_question) != 0) && (strcmp("quit", answer_to_question) != 0) ){
			printf("wrong answer. Type either 'yes' or 'no' or 'quit'. Case sensitivity is enforced: ");
			scanf("%s", answer_to_question);
		}
		if(strcmp("quit", answer_to_question) == 0){
			clientStopped = true;
			continue;
		}
		inputWorkOrderPtr = (HomeAutomationWorkOrder_t *)malloc(sizeof(HomeAutomationWorkOrder_t));
		if(inputWorkOrderPtr == NULL){
			DieWithUserMessage("malloc() failed", "cannot create a work order request");
		}
		inputWorkOrderPtr->subsystem_identifier = subsystem_identifier;
		inputWorkOrderPtr->endpoint_identifier = endpoint_identifier;
		inputWorkOrderPtr->configuration_value = configuration_value;
		if(strcmp("yes", answer_to_question) == 0){
			inputWorkOrderPtr->read_configuration = true;
			inputWorkOrderPtr->change_configuration = false;
		}else{
			inputWorkOrderPtr->read_configuration = false;
			inputWorkOrderPtr->change_configuration = true;
		}
		process_input_workorder(inputWorkOrderPtr, fd_sock_ptr);
		free(inputWorkOrderPtr);

	}
	fputs("client shutting down....\n", stderr);
	fclose(fd_sock_ptr);
	//close(serverSocket);
	exit(0);
}

void validate_and_initialize_inputs(int argc){
	if (argc != 3) // Test for correct number of arguments
		DieWithUserMessage("Parameter(s)", "<Server Address/Name>  [<Server Port/Service>]");
}

void validate_server_socket(int serverSocket){
	if (serverSocket < 0)
		DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
}

void process_input_workorder(HomeAutomationWorkOrder_t *inputWorkOrderPtr, FILE *fd_sock_ptr){
	printf("\nyou entered the subsystem identifier as %u\n", inputWorkOrderPtr->subsystem_identifier);
	printf("you entered the end-point identifier as %u\n", inputWorkOrderPtr->endpoint_identifier);
	printf("you entered the configuration value as %u\n", inputWorkOrderPtr->configuration_value);
	if(inputWorkOrderPtr->change_configuration == true){
		printf("your work item request is a change of end-point configuration.\n");
	}else{
		printf("your work item request is a read of the current end-point configuration.\n");
	}
	uint8_t outBuf[MAX_WIRE_SIZE];
	Encode(inputWorkOrderPtr, outBuf);
	printf("\nEnd of encoding.\n");
	printf("sending message to server\n");
	PutMsg(outBuf, sizeof(outBuf), fd_sock_ptr);
	printf("successfully sent message\n\n");
	uint8_t inputBuf[MAX_WIRE_SIZE];
	GetNextMsg(fd_sock_ptr, inputBuf, MAX_WIRE_SIZE);
	printf("successfully read from server.\n");
	printf("beginning decoding sequence.\n");
	Decode(inputBuf, inputWorkOrderPtr);
	printf("\nEnd of decoding");
}
