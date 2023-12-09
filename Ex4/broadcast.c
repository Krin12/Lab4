#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 3

pthread_mutex_t mutex;
int sum;
int status = 0;
int caller = 0;

pthread_t tid[5];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *child_thread(void *arg) {
    while (1) {
        sleep(1);
        pthread_mutex_lock(&mutex);
        int random = rand() % 100;
        if (random < 10) {
            printf("\n%d Child : message request\n", *((int *)arg));
            status = 1;
            caller = *((int *)arg);
        }
        pthread_cond_wait(&cond, &mutex);
        printf("%d Child : message from parent\n", *((int *)arg));
        pthread_mutex_unlock(&mutex);
    }
}

void *parent_thread(void *arg) {
    while (1) {
        sleep(1);
        if (status == 1) {
            pthread_mutex_lock(&mutex);
            pthread_cond_broadcast(&cond);
            printf("\nParent: Call message request\n\n");
            status = 0;
            pthread_mutex_unlock(&mutex);
        }
    }
}

int main() {
    srand(time(NULL));
    int i;
    int id[5];

    pthread_create(&tid[0], NULL, parent_thread, (void *)&id[0]);
    for (i = 1; i < 5; i++) {
        id[i] = i;
        pthread_create(&tid[i], NULL, child_thread, (void *)&id[i]);
        pthread_detach(tid[i]);
    }

    while (1) {
        sleep(1);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
