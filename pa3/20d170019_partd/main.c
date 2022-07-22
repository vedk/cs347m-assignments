#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_STUDENTS 20
#define KNOWN_MODULO 5
#define PRESENT 1
#define ABSENT 0

#define GFID 1
#define BFID 2

pthread_mutex_t rmmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t rmcv = PTHREAD_COND_INITIALIZER;

pthread_mutex_t smutex = PTHREAD_MUTEX_INITIALIZER;
int state[MAX_STUDENTS];

int count_students() {
	printf("cs/l\n");
	pthread_mutex_lock(&smutex);
	int n = 0;
	for (int i = 0; i < MAX_STUDENTS; i++)
		if (state[i] == PRESENT)
			n++;
	pthread_mutex_unlock(&smutex);
	printf("cs/u\n");

	return n+1;
}

int fav_state() {
	printf("fs/l\n");
	pthread_mutex_lock(&smutex);
	int retval = 1;
	if (state[GFID] == PRESENT && state[BFID] == PRESENT) {
		for (int i = 0; i < MAX_STUDENTS; i += KNOWN_MODULO) {
			if (state[i] == PRESENT) {
				retval = 0;
				break;
			}
		}
	} else {
		retval = 0;
	}
	pthread_mutex_unlock(&smutex);
	printf("fs/u\n");

	return retval;
}

void *send_student(void *args) {
adds_head:
	if (count_students() == MAX_STUDENTS)
		pthread_cond_signal(&rmcv);

	printf("ss/l\n");
	pthread_mutex_lock(&smutex);
	int sid = rand() % MAX_STUDENTS;
	while (state[sid] == PRESENT)
		sid = rand() % MAX_STUDENTS;
	state[sid] = PRESENT;

	printf("adds/state = ");
	for (int i = 0; i < MAX_STUDENTS; i++) {
		printf("%d ", state[i]);
	}
	printf("\n");
	sleep(1);
	pthread_mutex_unlock(&smutex);
	printf("ss/u\n");
	goto adds_head;
}

void *executor(void *args) {
	int tid = *((int *) args);
	pthread_t rmt;

	if (tid == GFID || tid == BFID) {
		printf("ex/l\n");
		pthread_mutex_lock(&smutex);
		if ((tid == GFID && state[BFID] == PRESENT) || (tid == BFID && state[GFID] == PRESENT)) {
			state[GFID] = state[BFID] = ABSENT;
			printf("walkie talkie...\n");
		}
		pthread_mutex_lock(&smutex);
		printf("ex/u\n");
	}
}

void *remove_students(void *args) {
	printf("Removing students...\n");

	printf("rs/l\n");
	pthread_mutex_lock(&smutex);
	int sid;
	printf("removed ");
	for (int i = 0; i < MAX_STUDENTS/2; i++) {
		sid = rand() % MAX_STUDENTS;
		state[sid] = ABSENT;
		printf("%d ", sid);
	}
	printf("\n");
	sleep(1);
	pthread_mutex_unlock(&smutex);
	printf("rs/u\n");
}

int main() {
	pthread_t addt, mont, rmt;

	srand(time(NULL));
	for (int i = 0; i < MAX_STUDENTS; i++)
		state[i] = 0;

	pthread_create(&addt, NULL, &send_student, NULL);
	pthread_create(&mont, NULL, &moniter_state, NULL);
	pthread_create(&rmt, NULL, &remove_students, NULL);

	pthread_join(addt, NULL);
	pthread_join(mont, NULL);
	pthread_join(rmt, NULL);

	pthread_mutex_destroy(&smutex);
	pthread_mutex_destroy(&rmmutex);
	pthread_cond_destroy(&rmcv);

	return 0;
}