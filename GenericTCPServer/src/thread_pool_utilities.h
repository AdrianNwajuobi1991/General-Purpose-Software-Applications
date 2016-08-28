/*
 * thread_pool_utilities.h
 *
 *  Created on: Sep 30, 2015
 *      Author: adrian
 */

#ifndef THREAD_POOL_UTILITIES_H_
#define THREAD_POOL_UTILITIES_H_



#endif /* THREAD_POOL_UTILITIES_H_ */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>

typedef struct tpool_work{
	void *arg;
	struct tpool_work *next;
}tpool_work_t;

typedef struct tpool{
	/* pool characteristics */
	int num_threads;
	int max_queue_size;
	int do_not_block_when_full;
	/* pool state */
	pthread_t *threads;
	int curr_queue_size;
	tpool_work_t *queue_head;
	tpool_work_t *queue_tail;
	int queue_closed;
	int shutdown;
	/* pool synchronization */
	pthread_mutex_t queue_lock;
	pthread_cond_t queue_not_empty;
	pthread_cond_t queue_not_full;
	pthread_cond_t queue_empty;
	void (*thread_pool_task) (void *);
} *tpool_t; // pointer type

void tpool_init(tpool_t *tpoolp, int num_worker_threads, int max_queue_size, int do_not_block_when_full, void (*thread_pool_task) (void *));
int tpool_add_work(tpool_t tpool, void *arg);
int tpool_destroy(tpool_t tpool, int finish);
