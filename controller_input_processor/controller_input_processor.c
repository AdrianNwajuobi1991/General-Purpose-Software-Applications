/*
 * controller_input_processor.c
 *
 *  Created on: Jun 19, 2016
 *      Author: adrian
 */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include "controller_input_processor.h"
#include "thread_pool_utilities.h"

#define FinishBlock(X) (*code_ptr = (X), code_ptr = dst++, code = 0x01)
#define MAX_THREADS	1
#define MAX_QUEUE	150

static int controller_init(const char* controller_device);
static int end_point_comms_init(const char* end_point_comms_device, struct termios *options);
static void COBS_Stuff_Data(const uint8_t *ptr, unsigned long length, uint8_t *dst);
static void process_data_packet_left_analog_stick(void * input_event);
static void process_data_packet_right_analog_stick(void * input_event);

static tpool_t thread_pool_left_analog_stick;
static tpool_t thread_pool_right_analog_stick;
static int fd_comms;

int main (void) {
	const char *dev_controller = "/dev/input/event25";
	const char *dev_comms = "/dev/ttyUSB0";
	struct input_event *controller_event_one;
	struct input_event *controller_event_two;
	struct input_event capture;
	struct termios options;
	ssize_t read_return_value;
	int fd_controller = controller_init(dev_controller);
	if(fd_controller < 0){
		return EXIT_FAILURE;
	}
	/*
	fd_comms = end_point_comms_init(dev_comms, &options);
	if(fd_comms < 0){
		return EXIT_FAILURE;
	}
	 */
	printf("All file descriptors initialized well. Ready to begin input processing.\n");
	tpool_init(&thread_pool_left_analog_stick, MAX_THREADS, MAX_QUEUE, 0);
	tpool_init(&thread_pool_right_analog_stick, MAX_THREADS, MAX_QUEUE, 0);
	while(1){
		controller_event_one = (struct input_event *)malloc(sizeof(struct input_event));
		controller_event_two = (struct input_event *)malloc(sizeof(struct input_event));
		if((controller_event_one == NULL) || (controller_event_two == NULL)){
			continue;
		}
		read_return_value = read(fd_controller, &capture, sizeof(struct input_event));
		if((read_return_value == (ssize_t)-1)){
			if(controller_event_one != NULL){
				free(controller_event_one);
				controller_event_one = NULL;
			}
			if(controller_event_two != NULL){
				free(controller_event_two);
				controller_event_two = NULL;
			}
			continue;
		}
		if(capture.code == BTN_MODE){
			break; // end the application close all file descriptors, and shutdown the thread pool
		}
		memcpy(controller_event_one, &capture, sizeof(struct input_event));
		memcpy(controller_event_two, &capture, sizeof(struct input_event));
		tpool_add_work(thread_pool_left_analog_stick, (void *)process_data_packet_left_analog_stick, (void *)controller_event_one);
		tpool_add_work(thread_pool_right_analog_stick, (void *)process_data_packet_right_analog_stick, (void *)controller_event_two);
	}
	printf("Shutting down application....\n");
	close(fd_controller);
	tpool_destroy(thread_pool_left_analog_stick, 1);
	tpool_destroy(thread_pool_right_analog_stick, 1);
	printf("Shutdown complete.\n");
	//close(fd_comms);
	return 0;
}

static int controller_init(const char* controller_device){
	int fd_controller;
	fd_controller = open(controller_device, O_RDONLY|O_NDELAY);
	if((fd_controller < 0)){
		printf("Could not open controller device.\n");
		return -1;
	}
	return fd_controller;
}

static int end_point_comms_init(const char* end_point_comms_device, struct termios *options){
	int fd_end_point_comms;
	fd_end_point_comms = open(end_point_comms_device, O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd_end_point_comms < 0){
		printf("Could not open comms device.\n");
		return -1;
	}
	tcgetattr(fd_end_point_comms, options);
	options->c_cflag = B115200 | CS8 | CREAD |CLOCAL;
	options->c_iflag = IGNPAR | ICRNL;
	tcflush(fd_end_point_comms, TCIFLUSH);
	tcsetattr(fd_end_point_comms, TCSANOW, options);
	return fd_end_point_comms;
}

static void COBS_Stuff_Data(const uint8_t *ptr, unsigned long length, uint8_t *dst){
	const uint8_t *end = ptr + length;
	uint8_t *code_ptr = dst++;
	uint8_t code = 0x01;
	while (ptr < end)
	{
		if (*ptr == 0)
			FinishBlock(code);
		else
		{
			*dst++ = *ptr;
			if (++code == 0xFF)
				FinishBlock(code);
		}
		ptr++;
	}
	FinishBlock(code);
}

static void process_data_packet_left_analog_stick(void * input_event){
	struct input_event *input_event_ptr = (struct input_event *)input_event;
	if((input_event_ptr->type == EV_SYN)){/* how should application handle EV_SYN ?*/
		printf("-------------- EV_SYN ------------\n");
	}else{
		printf("event type = %d, event code = %d, event value = %d\n", input_event_ptr->type, input_event_ptr->code, input_event_ptr->value);
		if(input_event_ptr->type == EV_ABS){
			/*
			 * Left analog stick activities
			 */
			if(input_event_ptr->code == ABS_X){

			}
			if(input_event_ptr->code == ABS_Y){

			}
		}
		/*
		 * write the COBS encoded packet to the end point commms device for transmission.
		 */
	}
	free(input_event_ptr);
}

static void process_data_packet_right_analog_stick(void * input_event){
	struct input_event *input_event_ptr = (struct input_event *)input_event;
	if((input_event_ptr->type == EV_SYN)){/* how should application handle EV_SYN ?*/
		printf("-------------- EV_SYN ------------\n");
	}else{
		printf("event type = %d, event code = %d, event value = %d\n", input_event_ptr->type, input_event_ptr->code, input_event_ptr->value);
		if(input_event_ptr->type == EV_ABS){
			/*
			 * Right analog stick activities
			 */
			if(input_event_ptr->code == ABS_RX){

			}
			if(input_event_ptr->code == ABS_RY){

			}
		}
		/*
		 * write the COBS encoded packet to the end point commms device for transmission.
		 */
	}
	free(input_event_ptr);
}
