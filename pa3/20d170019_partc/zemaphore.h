#ifndef ZEMAPHORE_H
#define ZEMAPHORE_H
#include <pthread.h>

typedef struct {
	int value;
	pthread_cond_t cv;
	pthread_mutex_t lock;
} zem_t;

void zem_init(zem_t *, int);
void zem_up(zem_t *);
void zem_down(zem_t *);

#endif /*ZEMAPHORE_H*/