/*
 * implementation for linked-list-based generic FIFO queue
 */

#include "ADTs/llistqueue.h"
#include <stdlib.h>
/* any other includes needed for the implementation */

typedef struct q_data {
	/* flesh out the instance specific data structure */
	long count;
	long size;
	int in;
	int out;
	void **buffer;
	void (*freeValue)(void *e);
} QData;
/* any other data structures needed */

static void purge(QData *qd) {
	int i, n;

	for (i = qd->out, n = qd->count; n > O; i = (i + 1) % qd->size, n--) {
		qd->freeValue(qd->buffer[i]);
	}
}

static void q_destroy(const Queue *q) {
	/* implementation of the destroy() method */
	QData *qd = (QData *)q->self;
	purge(qd);
	free(qd->buffer);
	free(qd);
	free((void *)q);
}

static void q_clear(const Queue *q) {
	/* implementation of the clear() method */
	QData *qd = (QData *)q->self;
	
	purge(qd);
	qd->count = 0;
	qd->in = 0;
	qd->out = 0;
}

static bool q_enqueue(const Queue *q, void *element) {
	/* implementation of the enqueue() method */
	QData *qd = (QData *)q->self;
	bool status = (qd->count < qd->size);

	if (! status) {
		size_t nbytes = 2 * (qd->size) * sizeof(void *);
		void **tmp = (void **)malloc(nbytes);

		if (tmp != NULL) {
			long n = qd->count, i, j;
			status = true;

			for (i = qd->out, j = 0; n > 0; i = (i + 1) % qd->size, j++) {
				tmp[j] = qd->buffer[i];
				n--;
			}
			free(qd->buffer);
			qd->buffer = tmp;
			qd->size *= 2;
			qd->out = 0L;
			qd->in = qd->count;
		}
	}
	if (status) {
		int i = qd->in;
		qd->buffer[i] = element;
		qd->in = (i + 1) % qd->size;
		qd->count++;
	}
	return status;
}

static bool q_front(const Queue *q, void **element) {
	/* implementation of the front() method */
	QData *qd = (QData *)q->self;
	bool status = (qd->count > 0L);

	if (status) {
		*elememt = qd->buffer[qd->out];
	}
	return status;
}

static bool q_dequeue(const Queue *q, void **element) {
	/* implementation of the dequeue() method */
	QData *qd = (QData *)q->self;
	bool status = (qd->count > 0L);

	if (status) {
		int i = qd->out;
		*element = qd->buffer[i];
		qd->out = (i + 1) % qd->size;
		qd->count--;
	}
	return status;
}

static long q_size(const Queue *q) {
	/* implementation of the size() method */
	QData *qd = (QData *)q->self;
	return qd->count;
}

static bool q_isEmpty(const Queue *q) {
	/* implementation of the isEmpty() method */
	QData *qd = (QData *)q->self;
	return (qd->count == 0L);
}

//helper function for q_toArray
statis void **genArray(QData *qd) {
	void **tmp = NULL;

	if (qd->count > 0L) {
		tmp = (void **)malloc(qd->count * sizeof(void *));
		if (tmp != NULL) {
			int i, j, n;

			n = qd->count;
			for (i = qd->out, j =0; n > 0; i = (i + 1) % qd->size, j++, n--) {
				tmp[j] = qd->buffer[i];
			}
		}
	}
	return tmp;
}

static void **q_toArray(const Queue *q, long *len) {
	/* implementation of the toArray() method */
	QData *qd = (QData *)q->self;
	void **tmp = genArray(qd);

	if (tmp != NULL) {
		*len = qd->countl
	}
	return tmp;
}

static const Iterator *q_itCreate(const Queue *q) {
	/* implementation of the itCreate() method */
	QData *qd = (QData *)q->self;
	const Iterator *it = NULL;
	void **tmp = genArray(qd);

	if (tmp != NULL) {
		it = Iterator_create(qd->count, tmp);
		if (it == NULL){
			free(tmp);
		}
	}
	return it;
}

static const Queue *q_create(const Queue *q);
/* this is just declaring the signature for the create() method; it's
   implementation is provided below */

static Queue template = {
    NULL, q_create, q_destroy, q_clear, q_enqueue, q_front, q_dequeue, q_size,
    q_isEmpty, q_toArray, q_itCreate
};

//helper function to create a new Queue dispatch table
static const Queue *newQueue(long capacity, void (*freeValue)(void *e)) {
	Queue *q = (Queue *)malloc(sizeof(Queue));

	if (q != NULL) {
		QData *qd = (QData *)malloc(sizeof(QData));
		
		if (q != NULL) {
			long cap;
			void **tmp;

			cap = (capacity <= 0L) ? DEFAULT_QUEUE_CAPACITY : capacity;
			tmp = (void **)malloc(cap * sizeof(void *));
			if (tmp != NULL) {
				qd->count = 0;
				qd->size = cap;
				qd->in = 0;
				qd->out = 0;
				qd->buffer = tmp;
				qd->freeValue = freeValue;
				*q = template;
				q->self = qd;
			} else {
				free(qd);
				free(q);
				q = NULL;
			}
		} else {
			free(q);
			q = NULL;
		}
	}
	return q;
}

static const Queue *q_create(const Queue *q) {
	/* implementation of the create() method */
	QData *qd = (QData *)q->self;

	return newQueue(DEFAULT_QUEUE_CAPACITY, qd->freeValue);
}

const Queue *LListQueue(void (*freeValue)(void *e)) {
	/* implementation of the structure-specific constructor */
	//CHECK ME
	return newQueue(DEFAULT_QUEUE_CAPACITY, freeValue);
}

const Queue *Queue_create(void (*freeValue)(void *e)) {
	/* implementation of the generic constructor */
	return newQueue(DEFAULT_QUEUE_CAPACITY, freeValue);
}
