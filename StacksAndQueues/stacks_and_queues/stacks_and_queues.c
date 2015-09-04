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
