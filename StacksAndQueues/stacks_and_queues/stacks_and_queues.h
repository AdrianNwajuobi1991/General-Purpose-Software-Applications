/*
 * stacks_and_queues.h
 *
 *  Created on: Aug 22, 2015
 *      Author: adrian
 */

#ifndef STACKS_AND_QUEUES_H_
#define STACKS_AND_QUEUES_H_



#endif /* STACKS_AND_QUEUES_H_ */

#include <stdio.h>
#include <stdlib.h>

#define MAXSTACK  50000
#define rouge_value 0xFFFF

typedef struct{
	int top;
	int ST[MAXSTACK];
}Stack_t;

typedef struct{
	int num;
	Node * next;
}Node;

typedef struct{
	Node *top_of_stack_element;
}StackList_t;

/**
 * initStack -- initializes a new empty Stack and returns a pointer to it.
 * Parameters
 * 				void
 * Returns
 * 				Stack_t* -- a pointer the new empty stack of type Stack_t
 */
extern Stack_t* initStack(void);

/**
 * isEmpty -- returns true(1) if the stack is empty or false (0) otherwise
 *
 * Parameters
 * 				stackPtr -- pointer to the stack structure
 * Returns
 * 				int
 */
extern int isEmpty(Stack_t* stackPtr);

/**
 * push -- adds data to the top of the stack
 *
 * Parameters
 * 				stackPtr -- pointer to the stack
 * 				data_to_push -- data to be added to the current top of the stack
 * Returns
 * 				void
 *
 */
extern void push(Stack_t* stackPtr, int data_to_push);

/*
 * pop -- returns the value of the current top of the stack
 *
 * Parameters
 * 				stackPtr -- pointer to the stack
 * Returns
 * 				int
 */
extern int pop(Stack_t* stackPtr);

extern StackList_t* initStackList(void);

extern int emptyStackList(StackList_t* stackListPtr);

extern void pushStackList(StackList_t *stackListPtr, int data_to_push);

extern int popStackList(StackList_t *stackListPtr);
