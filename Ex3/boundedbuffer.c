#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
sem_t empty, full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    int item;
    while (1) {
        item = rand() % 100;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        for (int i = 0; i < BUFFER_SIZE; ++i) {
            if (buffer[i] == -1) {
                buffer[i] = item;
                printf("생산: %d\n", item);
                break;
            }
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(rand() % 3);
    }
    return NULL;
}

void* consumer(void* arg) {
    int item;
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int idx = rand() % BUFFER_SIZE;
        if (buffer[idx] != -1) {
            item = buffer[idx];
            buffer[idx] = -1;
            printf("소비: %d\n", item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(rand() % 3);
    }
    return NULL;
}

int main() {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = -1;
    }
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
