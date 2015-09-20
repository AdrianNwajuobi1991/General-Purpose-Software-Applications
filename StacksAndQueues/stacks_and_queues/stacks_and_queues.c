/*
 * stacks_and_queues.c
 *
 *  Created on: Aug 22, 2015
 *      Author: adrian
 */

#include "stacks_and_queues.h"

Stack_t* initStack(void){
	Stack_t* stackPtr = (Stack_t*)malloc(sizeof(Stack_t));
	stackPtr ->top = -1;
	return stackPtr;
}

int isEmpty(Stack_t* stackPtr){
	return(stackPtr ->top == -1);
}

void push(Stack_t* stackPtr, int data_to_push){
	/*
	 * if the top of the stack is at MAXSTACK - 1 the underlying
	 * stack array is full.
	 */
	if(stackPtr ->top == MAXSTACK - 1){
		return;
	}
	/*
	 * compute current top of stack: set the current indicator index for the top of the stack in ST
	 */
	stackPtr ->top += 1;
	/*
	 * use the previously set top-of-stack index push data item to top of stack
	 */
	stackPtr ->ST[stackPtr->top] = data_to_push;
	return;
}

int pop(Stack_t* stackPtr){
	if(stackPtr == NULL || isEmpty(stackPtr)){
		return rouge_value;
	}
	/*
	 * Retrieve the current top of the stack, whose index is stored in stackPtr->top
	 */
	int top_of_stack_value  = stackPtr->ST[stackPtr->top];
	return top_of_stack_value;
}

StackList_t* initStackList(void){
	StackList_t *stackListPtr = (StackList_t*)malloc(sizeof(StackList_t));
	stackListPtr->top_of_stack_element = NULL;
	return stackListPtr;
}

int emptyStackList(StackList_t* stackListPtr){
	return (stackListPtr->top_of_stack_element == NULL);
}

void pushStackList(StackList_t *stackListPtr, int data_to_push){
	Node *stackElement = (Node *)malloc(sizeof(Node));
	stackElement->num = data_to_push;
	/*
	 * set the next pointer of stackElement to point to the current node(or NULL) pointed to by top_of_stack_element
	 *
	 */
	stackElement->next=stackListPtr->top_of_stack_element;
	/*
	 * set top_of_stack_element to point to node stackElement as the new top of the stack
	 */
	stackListPtr->top_of_stack_element = stackElement;
	return;
}

int popStackList(StackList_t *stackListPtr){
	if(emptyStackList(stackListPtr)){
		return rouge_value;
	}
	/*
	 * retrieve the value at the top of the stack and store it.
	 */
	int top_of_stack_value = stackListPtr->top_of_stack_element->num;
	/*
	 * declare a current top pointer to point to the current top of the stack element
	 */
	Node *current_top_of_stack = stackListPtr->top_of_stack_element;
	/*
	 * set the top_of_stack_element to point to the next node pointed to by current_top_of_stack->next
	 */
	stackListPtr->top_of_stack_element = current_top_of_stack->next;
	/*
	 * free current_top_of_stack, thus popping the top element
	 */
	free(current_top_of_stack);
	return top_of_stack_value;
}

Queue_t* initQueue(void){
	Queue_t * QP = (Queue_t*)malloc(sizeof(Queue_t));
	QP->head_index = 0;
	QP->tail_index = 0;
	return QP;
}

int isEmptyQueue(Queue_t* Queue_Ptr){
	return (Queue_Ptr ->head_index == Queue_Ptr->tail_index);
}

void enQueue(Queue_t* Queue_Ptr, int data_to_enqueue){
	/*
	 * check tail_index to see if tail_index = MAXQUEUE - 1
	 */
	if(Queue_Ptr->tail_index == MAXQUEUE-1){
		/*
		 * set tail_index to 0.
		 */
		Queue_Ptr->tail_index = 0;
		return;
	}
	/*
	 * increment for the next slot to enqueue. At this point tail_index is not
	 * equal to MAXQUEUE-1
	 */
	Queue_Ptr->tail_index += 1;
	/*
	 * since this is a circular queue, check to see if tail_index = head_index
	 * if so the queue is full
	 */
	if(Queue_Ptr->tail_index == Queue_Ptr->head_index){
		return;
	}
	/*
	 * queue is not full yet. So add element in tail_index
	 */
	Queue_Ptr->QA[Queue_Ptr->tail_index] = data_to_enqueue;
}

int deQueue(Queue_t* Queue_Ptr){
	/*
	 * isEmpty will indicate whether head_index = tail_index.
	 */
	if(isEmpty(Queue_Ptr)){
		exit(1);
	}
	/*
	 * at this point the last element has been dequeued.
	 * set head to point to the index "just before" the beginning element in the queue
	 */
	if(Queue_Ptr->head_index == MAXQUEUE -1){
		Queue_Ptr->head_index = 0;
	}else{
		/*
		 * increment head_index to the current top element index
		 */
		Queue_Ptr->head_index += 1;
	}
	return Queue_Ptr->QA[Queue_Ptr->head_index];

}
