/*
 * thread_pool_utilities.c
 *
 *  Created on: Oct 3, 2015
 *      Author: adrian
 */

#include "thread_pool_utilities.h"
#define DEBUG(fmt, ...)	fprintf(stderr, fmt, __VA_ARGS__)


void tpool_thread(tpool_t tpool){
	/*
	 * declare a work item, request queue pointer for this particular thread
	 */
	tpool_work_t *workItemPtr;
	/*
	 * the infinite loop that makes all threads in the pool reusable
	 */
	for(;;){
		/*
		 * first lock the work queue to obtain access to it
		 */
		pthread_mutex_lock(&(tpool->queue_lock));
		/*
		 * while the current queue size is zero(queue is empty), and the shutdown flag is false(not asserted true)
		 * suspend this thread until the queue_not_empty condition is signaled as true(the condition queue_not_empty has been met)
		 */
		while((tpool->curr_queue_size == 0) && (!tpool->shutdown)){
			pthread_cond_wait(&(tpool->queue_not_empty), &(tpool->queue_lock));
		}
		/*
		 * outside this while loop, a shut down signal has either been given or the queue is full
		 */
		if(tpool->shutdown){
			/*
			 * if a shutdown flag has been asserted true, release the mutex and exit. This thread has ended.
			 */
			pthread_mutex_unlock(&(tpool->queue_lock));
			pthread_exit(NULL);
		}
		/*
		 * at this point a shutdown signal was not given, but the queue is not empty (the queue size is greater than 0)
		 */
		workItemPtr = tpool->queue_head;
		/*
		 * decrement queue size because this thread has dequeued a work request at the head of the list
		 */
		tpool->curr_queue_size--;
		if(tpool->curr_queue_size == 0){
			/*
			 * if this thread empties the queue, the queue_head and queue_tail are set to null
			 */
			tpool->queue_head = tpool->queue_tail = NULL;
		}else{
			/*
			 * this thread does not empty the queue, set the next queue element to be the new queue_head
			 */
			tpool->queue_head = workItemPtr->next;
		}
		/*
		 * if it is true that do_not_block_when_full is false and the current queue size is one less than the maximum
		 * broadcast the condition queue_not_full to waiting threads
		 */
		if((!tpool->do_not_block_when_full) && (tpool->curr_queue_size == (tpool->max_queue_size-1))){
			pthread_cond_broadcast(&(tpool->queue_not_full));
		}
		/*
		 * if the queue is empty(current size is 0), signal the only other thread that the queue is empty so
		 * that more work will be added
		 */
		if(tpool->curr_queue_size == 0){
			pthread_cond_signal(&(tpool->queue_empty));
		}
		/*
		 * this thread, at this point will release the work queue mutex and execute the routine in its work item.
		 * then free the work item pointer.
		 */
		pthread_mutex_unlock(&(tpool->queue_lock));
		(*(workItemPtr->routine)) (workItemPtr->arg);
		/*
		 * this call to free has the effect of deleting the current head of the list: a proper dequeue of a work item
		 */
		free(workItemPtr);
	}
}

void tpool_init(tpool_t *tpoolp, int num_worker_threads, int max_queue_size, int do_not_block_when_full){
	int i, rtn;
	tpool_t tpool;
	DEBUG("%s", "Initializing Thread Pool\n");
	/*
	 * allocate space for the thread pool
	 */
	if((tpool = (tpool_t)malloc(sizeof(struct tpool))) == NULL){
		exit(-1);
	}
	/*
	 * if malloc succeeded then initialize the thread pool
	 */
	tpool->num_threads = num_worker_threads;
	tpool->max_queue_size = max_queue_size;
	tpool->do_not_block_when_full = do_not_block_when_full;
	/*
	 * allocate contiguous space to store the threads of the thread pool. The actual threads are yet to be created
	 */
	if((tpool->threads = (pthread_t *)malloc(sizeof(pthread_t) * tpool->num_threads)) == NULL){
		exit(-1);
	}
	tpool->curr_queue_size = 0;
	tpool->queue_head = NULL;
	tpool->queue_tail = NULL;
	tpool->queue_closed = 0;
	tpool->shutdown = 0;
	if((rtn = pthread_mutex_init(&(tpool->queue_lock), NULL)) != 0){
		exit(-1);
	}
	if((rtn = pthread_cond_init(&(tpool->queue_not_empty), NULL)) != 0){
		exit(-1);
	}
	if((rtn = pthread_cond_init(&(tpool->queue_not_full), NULL)) != 0){
		exit(-1);
	}
	if((rtn = pthread_cond_init(&(tpool->queue_empty), NULL)) != 0){
		exit(-1);
	}
	/*
	 * create the worker threads
	 */
	for(i = 0; i < tpool->num_threads; i++){
		if((rtn = pthread_create(&(tpool->threads[i]), NULL, (void *) tpool_thread, (void *)tpool)) != 0){
			exit(-1);
		}
	}
	/*
	 * assign the pointer in the main server program the value of the address if the initialized thread pool.
	 */
	*tpoolp = tpool;
	DEBUG("%s", "Thread Pool Initialization Complete\n");
}

int tpool_add_work(tpool_t tpool, void *routine, void *arg){
	/*
	 * declare a pointer to a new workorder to be added to the work request queue
	 */
	tpool_work_t *workOrderPtr;
	/*
	 * lock the mutex for the work request queue to obtain access to it
	 */
	pthread_mutex_lock(&(tpool->queue_lock));
	/*
	 * if the queue is full(is of maximum size) and do_not_block_when_full is true, then release the mutex and return
	 * in this case this thread will not suspend itself if the queue is full
	 */
	if((tpool->curr_queue_size == tpool->max_queue_size) && tpool->do_not_block_when_full){
		pthread_mutex_unlock(&(tpool->queue_lock));
		return -1;
	}
	/*
	 * since this thread did not return -1 from above, do_not_block_when_full is false(which means this thread will
	 * block when the queue is full)
	 * while the queue is full and neither the shutdown nor the queue_closed flags are set, this thread will supspend
	 * execution on the condition queue_not_full, until signaled on it(the condition has been met)
	 */
	while((tpool->curr_queue_size == tpool->max_queue_size) && (!(tpool->shutdown || tpool->queue_closed))){
		pthread_cond_wait(&(tpool->queue_not_full), &(tpool->queue_lock));
	}
	/*
	 * outside this loop, the current size of the queue is less than max_queue_size or either the shutdown or queue_closed signal
	 * is given
	 */
	if(tpool->shutdown || tpool->queue_closed){
		/*
		 * a shutdown or queue_closed has been given. release mutex and return
		 */
		pthread_mutex_unlock(&(tpool->queue_lock));
		return -1;
	}
	/*
	 * at this point neither signal has been issued, but the current size is either zero or less than the max queue size
	 * but greater than zero
	 */
	workOrderPtr = (tpool_work_t *)malloc(sizeof(tpool_work_t));
	workOrderPtr->routine = routine;
	workOrderPtr->arg = arg;
	workOrderPtr->next = NULL;
	if(tpool->curr_queue_size == 0){
		/*
		 * the queue size was zero(queue was empty) but is no longer empty, since exactly one element is in the queue
		 */
		tpool->queue_tail = tpool->queue_head = workOrderPtr;
		pthread_cond_broadcast(&tpool->queue_not_empty);

	}else{
		/*
		 * the queue size is less than the max queue size but greater than zero
		 */
		tpool->queue_tail->next = workOrderPtr;
		tpool->queue_tail = workOrderPtr;
	}
	tpool->curr_queue_size++;
	pthread_mutex_unlock(&(tpool->queue_lock));
	return 1;
}

int tpool_destroy(tpool_t tpool, int finish){
	DEBUG("%s", "Thread Pool destruction beginning\n");
	int          i,rtn;
	tpool_work_t *cur_nodep;


	if ((rtn = pthread_mutex_lock(&(tpool->queue_lock))) != 0)
		fprintf(stderr,"pthread_mutex_lock %d",rtn), exit(1);

	/* Is a shutdown already in progress? */
	if (tpool->queue_closed || tpool->shutdown) {
		if ((rtn = pthread_mutex_unlock(&(tpool->queue_lock))) != 0)
			fprintf(stderr,"pthread_mutex_unlock %d",rtn), exit(1);
		return 0;
	}

	tpool->queue_closed = 1;

	/* If the finish flag is set, wait for workers to
	     drain queue */
	if (finish == 1) {
		while (tpool->curr_queue_size != 0) {
			if ((rtn = pthread_cond_wait(&(tpool->queue_empty),
					&(tpool->queue_lock))) != 0)
				fprintf(stderr,"pthread_cond_wait %d",rtn), exit(1);
		}
	}

	tpool->shutdown = 1;

	if ((rtn = pthread_mutex_unlock(&(tpool->queue_lock))) != 0)
		fprintf(stderr,"pthread_mutex_unlock %d",rtn), exit(1);


	/* Wake up any workers so they recheck shutdown flag */
	if ((rtn = pthread_cond_broadcast(&(tpool->queue_not_empty))) != 0)
		fprintf(stderr,"pthread_cond_broadcast %d",rtn), exit(1);
	if ((rtn = pthread_cond_broadcast(&(tpool->queue_not_full))) != 0)
		fprintf(stderr,"pthread_cond_broadcast %d",rtn), exit(1);


	/* Wait for workers to exit */
	for(i=0; i < tpool->num_threads; i++) {
		if ((rtn = pthread_join(tpool->threads[i],NULL)) != 0)
			fprintf(stderr,"pthread_join %d",rtn), exit(1);
	}

	/* Now free pool structures */
	free(tpool->threads);
	while(tpool->queue_head != NULL) {
		cur_nodep = tpool->queue_head->next;
		tpool->queue_head = tpool->queue_head->next;
		free(cur_nodep);
	}
	free(tpool);
	DEBUG("%s", "Thread Pool destruction Complete\n");
	return 0;
}
