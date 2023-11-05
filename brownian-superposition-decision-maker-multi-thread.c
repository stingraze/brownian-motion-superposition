#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define STEPS 1000
#define NUM_THREADS 8
//Created with the help of ChatGPT (GPT-4)
//(C)Tsubasa Kato 11/5/2023 10:03AM - Inspire Search Corporation
//Website: https://www.inspiresearch.io/en
pthread_mutex_t mutex_output;

typedef struct {
    int id;
    double difficulty;
    double foreseen_future;
    double disturbance;
} ThreadData;

// Thread-safe random double generation
double random_double(unsigned int *seedp) {
    return 2.0 * ((double)rand_r(seedp) / RAND_MAX) - 1.0;
}

// Weighted decision that is also thread-safe
int weighted_decision(double weight, unsigned int *seedp) {
    double threshold = (1.0 - weight) * ((double)rand_r(seedp) / RAND_MAX);
    return threshold < 0.5 ? 1 : 0;
}

// Thread function for simulating Brownian motion
void *simulate_brownian_motion(void *threadarg) {
    ThreadData *my_data = (ThreadData *) threadarg;
    int id = my_data->id;
    double difficulty = my_data->difficulty;
    double foreseen_future = my_data->foreseen_future;
    double disturbance = my_data->disturbance;
    unsigned int seed = (unsigned int)time(NULL) ^ (id << 16);

    double x = 0.0, y = 0.0;

    pthread_mutex_lock(&mutex_output);
    printf("Thread #%d: Starting position of the particle: (%f, %f)\n", id + 1, x, y);
    pthread_mutex_unlock(&mutex_output);

    for (int i = 0; i < STEPS; i++) {
        double move_probability = foreseen_future * (1 - difficulty) * (1 - disturbance);

        double dx = weighted_decision(move_probability, &seed) * random_double(&seed);
        double dy = weighted_decision(move_probability, &seed) * random_double(&seed);

        x += dx;
        y += dy;

        pthread_mutex_lock(&mutex_output);
        printf("Thread #%d: Step %d: position of the particle: (%f, %f)\n", id, i + 1, x, y);
        pthread_mutex_unlock(&mutex_output);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int rc;
    long t;

    // Initialize the mutex
    pthread_mutex_init(&mutex_output, NULL);

    // Create threads
    for (t = 0; t < NUM_THREADS; t++) {
        thread_data[t].id = t;
        thread_data[t].difficulty = 0.5;
        thread_data[t].foreseen_future = 0.7;
        thread_data[t].disturbance = 0.3;

        rc = pthread_create(&threads[t], NULL, simulate_brownian_motion, (void *)&thread_data[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Clean up and exit
    pthread_mutex_destroy(&mutex_output);
    pthread_exit(NULL);
}
