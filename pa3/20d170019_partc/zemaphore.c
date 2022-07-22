#include "zemaphore.h"

void zem_init(zem_t *z, int x) {
	z->value = x;
	pthread_cond_init(&z->cv, NULL);
	pthread_mutex_init(&z->lock, NULL);
}

void zem_up(zem_t *z) {
	pthread_mutex_lock(&z->lock);
	z->value++;
	pthread_cond_broadcast(&z->cv);
	pthread_mutex_unlock(&z->lock);
}

void zem_down(zem_t *z) {
	pthread_mutex_lock(&z->lock);
	while (z->value <= 0)
		pthread_cond_wait(&z->cv, &z->lock);
	z->value--;
	pthread_mutex_unlock(&z->lock);
}