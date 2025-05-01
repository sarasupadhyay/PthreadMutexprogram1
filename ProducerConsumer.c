#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5  // Size of the shared buffer
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

// Shared buffer
int buffer[BUFFER_SIZE];
int in = 0;  // Index for the next item to be produced
int out = 0; // Index for the next item to be consumed

// Semaphores
sem_t empty;   // Semaphore to count empty slots in buffer
sem_t full;    // Semaphore to count full slots in buffer
sem_t mutex;   // Mutex for mutual exclusion (protects the buffer)

void* producer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sleep(rand() % 3);  // Simulate time taken to produce an item

        int item = rand() % 100;  // Produce a random item

        sem_wait(&empty);  // Wait for an empty slot
        sem_wait(&mutex);  // Enter critical section

        // Produce an item into the buffer
        buffer[in] = item;
        printf("Producer %d produced item %d at index %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);  // Exit critical section
        sem_post(&full);   // Signal that there's a new item to consume
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sleep(rand() % 3);  // Simulate time taken to consume an item

        sem_wait(&full);  // Wait for a full slot
        sem_wait(&mutex); // Enter critical section

        // Consume an item from the buffer
        int item = buffer[out];
        printf("Consumer %d consumed item %d from index %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);  // Exit critical section
        sem_post(&empty);  // Signal that there's a free slot for producers
    }
    return NULL;
}

int main() {
    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially all slots are empty
    sem_init(&full, 0, 0);  // No items are in the buffer initially
    sem_init(&mutex, 0, 1);  // Mutex initialized to 1 (binary semaphore)

    // Create producer and consumer threads
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS] = {1, 2};
    int consumer_ids[NUM_CONSUMERS] = {1, 2};

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    // Wait for all threads to finish (in this case, they'll run indefinitely)
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Clean up semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
