/*
 * linked_list_more.c
 *
 *  Created on: Aug 17, 2015
 *      Author: adrian Nwajuobi
 */

#include<stdlib.h>
#include<stdio.h>

typedef struct{
	int num;
	struct Node *next;
}Node;

/**
 * delete_node_head -- always deletes the first element if the linked list
 * Parameters
 * 			head_ptr_copy --  a copy of the pointer to the head of the list
 * Returns
 * 			void
 */
void delete_node_head(Node *head_ptr_copy){
	if(head_ptr_copy == NULL){
		return;
	}
	/*
	 * Obtain a reference to the current head of the list.
	 * This reference will be used later in the function to free the top of the list
	 */
	Node *old_top_ptr = head_ptr_copy;
	/*
	 * Advance the head_ptr_copy to point to the next element in the list after
	 * the current top element in the list. At the end of the function this will
	 * be the new head-element of the list
	 */
	head_ptr_copy = head_ptr_copy -> next;
	/*
	 * free the current top of the list(the deletion step)pointed to by old_top_ptr
	 */
	free(old_top_ptr);
}

void delete_node_random(Node *node_prev){
	if(node_prev == NULL){
		return;
	}
	/*
	 * Assign a pointer for the node to be deleted. This node is the node element
	 * that follows immediately after the node pointed to by node_prev.
	 */
	Node *node_to_delete_ptr = node_prev -> next;
	/*
	 * Set the next pointer of node_prev to point to the node element immediately
	 * following the node pointed to by node_to_delete_ptr. This "logical deletion"
	 * step will ensure that freeing node_to_delete_ptr properly deletes the node
	 */
	node_prev -> next = node_to_delete_ptr -> next;
	/*
	 * finally node_to_delete_ptr is freed(thus properly deleting the desired node
	 */
	free(node_to_delete_ptr);
}

void insert_sorted_ascending_order(Node *head_ptr_copy, Node *node_to_insert){
	/*
	 * If the list is empty, trivially insert set the head pointer(head_ptr_copy) to point to the node
	 * pointed to by node_to_insert.
	 */
	if(head_ptr_copy == NULL){
		head_ptr_copy = node_to_insert;
		return;
	}
	/*
	 * at this point, the list is not empty at least one element is in the list
	 */
	Node *prev_node_ptr = NULL;
	Node *current_node_ptr = head_ptr_copy;
	/*
	 * when we want to insert a new node to a sorted list, the inserted node is either less than or equal to the node
	 * currently being pointed to, or greater than it. Thus it will either be inserted to the left of the current
	 * node or the right of it(logically speaking).
	 * This while loop takes care of the latter case(greater than or equal to)
	 */
	while(current_node_ptr != NULL && node_to_insert->num > current_node_ptr->num){
		prev_node_ptr = current_node_ptr;
		current_node_ptr = current_node_ptr->next;
	}
	/*
	 * at this point one of two things has happened: we have reached the end of the list, in which case, the new node
	 * has the largest value so far, OR the new node is less than or equal to the current value being pointed to.
	 */
	if(prev_node_ptr == NULL){// prev = NULL and node_to_insert < current_node_ptr
		node_to_insert->next = current_node_ptr;
		return;
	}else{ // prev_node_ptr is not null
		if(current_node_ptr == NULL){// prev -> next = node_to_insert, since current is null and prev is the last element
			prev_node_ptr->next = node_to_insert;
			return;
		}else{ //prev_node_ptr is not null and current_node_ptr is not null: prev < node_to_insert < current
			node_to_insert->next = current_node_ptr;
			prev_node_ptr->next = node_to_insert;
			return;
		}
	}

}


