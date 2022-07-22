#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>

#include "zemaphore.h"

#define NUM_THREADS 3
zem_t z[NUM_THREADS];

#define NUM_ITER 10

void *justprint(void *data) {
    int thread_id = *((int *)data);

    
    for (int i = 0; i < NUM_ITER; i++) {
        zem_down(&z[thread_id]);
        printf("This is thread %d\n", thread_id);
        zem_up(&z[(thread_id+1) % NUM_THREADS]);
    }

    return 0;
}

int main(int argc, char *argv[]) {

    pthread_t mythreads[NUM_THREADS];
    int mythread_id[NUM_THREADS];

    zem_init(&z[0], 1);
    zem_init(&z[1], 0);
    zem_init(&z[2], 0);

    for (int i = 0; i < NUM_THREADS; i++) {
        mythread_id[i] = i;
        pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(mythreads[i], NULL);
    }

    return 0;
}
